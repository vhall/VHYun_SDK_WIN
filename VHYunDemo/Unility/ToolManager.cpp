#include "ToolManager.h"
#include <QCoreApplication>
#include "DataManager.h"
#include "ConfigManager.h"
#include "CustomEventType.h"
#include <QJsonDocument>
#include <QJsonObject>
//#include "WebRtcLiveWdg.h"

using namespace vlive;
ToolManager *ToolManager::mpInstance = nullptr;
std::mutex ToolManager::mInstanceMutex;

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
{
   if (mLibManager == nullptr) {
      mLibManager = new LibManager(this);
   }
   if (mWndManager == nullptr) {
      mWndManager = new WndManager;
   } 
   if (mDataManager == nullptr) {
       mDataManager = new DataManager;
   }
   if (mConfigManagerInterface == nullptr) {
       mConfigManagerInterface = new ConfigManager();
   }
   if (mpVHPaasInteractionRoom == nullptr) {
       mpVHPaasInteractionRoom = GetPaasSDKInstance();
       if (mpVHPaasInteractionRoom) {
           mpVHPaasInteractionRoom->RegisterVHRoomMonitor(this);
       }
   }
}

ToolManager::~ToolManager()
{
   SaveRelease(mLibManager);
   SaveRelease(mWndManager);
   SaveRelease(mDataManager);
   SaveRelease(mConfigManagerInterface);
   DestoryPaasSDKInstance();
}

ToolManager* ToolManager::GetInstance() {
   std::unique_lock<std::mutex> lock(mInstanceMutex);
   if (mpInstance == nullptr) {
      mpInstance = new ToolManager;  
   }
   return mpInstance;
}

void ToolManager::Init() {
    ToolManager::GetInstance()->GetDataManager()->Init();
    if (mpVHPaasInteractionRoom) {
        mpVHPaasInteractionRoom->InitSDK();
    }
}

void ToolManager::Release() {
    ToolManager::GetInstance()->GetWndManager()->DestoryAllWnd();
}

void ToolManager::RegisterQEventReciver(QObject* obj) {
    std::unique_lock<std::mutex> lock(mEventRecvMutex);
    bool bFind = false;
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        if (*iter == obj) {
            bFind = true;
            break;
        }
        iter++;
    }
    if (!bFind) {
        mEventReciver.push_back(obj);
    }
}

void ToolManager::RemoveQEventReciver(QObject* obj) {
    std::unique_lock<std::mutex> lock(mEventRecvMutex);
    bool bFind = false;
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        if (*iter == obj) {
            mEventReciver.erase(iter);
            break;
        }
        iter++;
    }
}

void ToolManager::PostEventToMainThread(QEvent* event) {
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        QCoreApplication::postEvent(*iter, event);
        iter++;
    }
}

void ToolManager::AppExec() {
    mQuitLoop.exec();
}
void ToolManager::AppQuit() {
    mQuitLoop.quit();
    QCoreApplication::quit();
}

void ToolManager::ReleaseInstance() {
   std::unique_lock<std::mutex> lock(mInstanceMutex);
   if (mpInstance != nullptr) {
      SaveRelease(mpInstance);
   }
}

LibManager* ToolManager::GetLibManager() {
   return mLibManager;
}

WndManager* ToolManager::GetWndManager() {
   return mWndManager;
}

DataManagerInterface* ToolManager::GetDataManager() {
   return mDataManager;
}

ConfigSettingInterface* ToolManager::GetConfigSettingInterface() {
    return mConfigManagerInterface;
}

VHPaasInteractionRoom* ToolManager::GetPaasSDK() {
    return mpVHPaasInteractionRoom;
}

void ToolManager::OnRoomSuccessedEvent(RoomEvent roomEvent, std::string userData) {
    QString id;
    int type;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(userData.c_str(), userData.length()));
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        id = obj["process_id"].toString();
        type = obj["type"].toInt();
    }

    if (roomEvent == RoomEvent_Login) {
        PostEventToMainThread(new QEventRoomEvent(CustomEvent_Login, roomEvent, id.toStdString(), type));
    }
    else {
        PostEventToMainThread(new QEventRoomEvent(CustomEvent_OnSuccessRoomEvent, roomEvent, id.toStdString(), type));
    }
}

void ToolManager::OnRoomFailedEvent(RoomEvent roomEvent, int respCode, const std::string& msg, std::string userData) {
    QString id = QString();
    int type = 0;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray(userData.c_str(), userData.length()));
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        id = obj["process_id"].toString();
        type = obj["type"].toInt();
    }

    if (roomEvent == RoomEvent_Login) {
        PostEventToMainThread(new QEventRoomEvent(CustomEvent_LoginErr, roomEvent, id.toStdString(), type, respCode, msg));
    }
    else {
        PostEventToMainThread(new QEventRoomEvent(CustomEvent_OnErrorRoomEvent, roomEvent, id.toStdString(), type, respCode, msg));
    }
}


void ToolManager::OnGetVHRoomMembers(const std::string& msg, std::list<VHRoomMember>& members) {
	//if (nullptr != mWndManager) {
	//	CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);
	//	pRtcLiveWdg->ClearAllOnLineUser();
	//}
	//
	GetDataManager()->WriteLog("%s RoomMembers Count: %d", __FUNCTION__, members.size());
	std::list<VHRoomMember>::iterator iter = members.begin();
    while (iter != members.end()){
        PostEventToMainThread(new QEventVHMember(CustomEvent_GetVHMemberList,iter->thirdPartyUserId, iter->status));
        iter++;
    }
}

void ToolManager::OnGetVHRoomKickOutMembers(std::list<VHRoomMember>& members) {
	//if (nullptr != mWndManager) {
	//	CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);
	//	pRtcLiveWdg->ClearAllKickOutUSer();
	//}

	GetDataManager()->WriteLog("%s KickOutMembers Count: %d", __FUNCTION__, members.size());
	std::list<VHRoomMember>::iterator iter = members.begin();
    while (iter != members.end()) {
        PostEventToMainThread(new QEventVHMember(CustomEvent_GetVHKickOutMemberList,iter->thirdPartyUserId, iter->status));
        iter++;
    }
}

/*收到申请上麦消息 ,消息为广播消息，收到消息后，通过进入互动房间时获取的权限列表，判断用户是否有审核权限*/
void ToolManager::OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id) {
    PostEventToMainThread(new QEventVHMember(CustomEvent_OnRecvApplyInavPublishMsg, QString::fromStdWString(third_party_user_id).toStdString()));
}

/*收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作*/
void ToolManager::OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish event) {
	if (GetDataManager()->GetThridPartyUserId().compare(QString::fromStdWString(third_party_user_id))==0 )
		PostEventToMainThread(new QEventVHMember(CustomEvent_OnRecvAuditInavPublishMsg, QString::fromStdWString(third_party_user_id).toStdString(), event));
}

/*邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息*/
void ToolManager::OnRecvAskforInavPublishMsg(const std::wstring& third_party_user_id) {
	if(GetDataManager()->GetThridPartyUserId().compare(QString::fromStdWString(third_party_user_id)) == 0)
		PostEventToMainThread(new QEventPublishStream(CustomEvent_OnRecvAskforInavPublishMsg, QString::fromStdWString(third_party_user_id).toStdString()));
}

/*踢出流消息  消息为广播消息，收到消息后，执行踢出流*/
void ToolManager::OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) {
    PostEventToMainThread(new QEventPublishStream(CustomEvent_OnRecvKickInavStreamMsg, QString::fromStdWString(third_party_user_id).toStdString()));
}

/*踢出互动房间 , 消息为广播消息，收到消息后，判断是当前用户后，执行踢出房间操作*/
void ToolManager::OnRecvKickInavMsg(const std::wstring& third_party_user_id) {
    PostEventToMainThread(new QEventPublishStream(CustomEvent_OnRecvKickInavMsg,QString::fromStdWString(third_party_user_id).toStdString()));
}

/*上/下/拒绝上麦消息 消息为广播消息*/
void ToolManager::OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event) {
    PostEventToMainThread(new QEventUserPublish(CustomEvent_OnUserPublishCallback, QString::fromStdWString(third_party_user_id).toStdString(), stream_id, event));
}

/*互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间*/
void ToolManager::OnRecvInavCloseMsg() {
    PostEventToMainThread(new QEvent(CustomEvent_OnRecvInavCloseMsg));
}

void ToolManager::OnUserOnLineState(bool online, const std::string user_id) {
    int state = online == true ? 1 : 0;
    PostEventToMainThread(new QEventVHMember(CustomEvent_User_OnLine, user_id, state));
}

/*房间链接事件*/
 void ToolManager::OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum type/*, const std::string&*/) {
     switch (type)
     {
     case VHRoomConnect_ROOM_CONNECTED:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomConnetSuc));
         break;
     case VHRoomConnect_ROOM_ERROR:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomConnetErr));
         break;
     case VHRoomConnect_ROOM_RECONNECTING:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomReConneting));
         break;
     case VHRoomConnect_RE_CONNECTED:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomReConneted));
         break;
     case VHRoomConnect_ROOM_DISCONNECTED:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomDisConnected));
         break;
     case VHRoomConnect_ROOM_RECOVER:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomRecover));
         break;
     case VHRoomConnect_ROOM_NETWORKCHANGED:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomNetWorkChanged));
         break;
     case VHRoomConnect_ROOM_NETWORKRECOVER:
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomNetWorkChangedCover));
         break;
     case VHRoomConnect_ROOM_MIXED_STREAM_READY: 
         PostEventToMainThread(new QEvent(CustomEvent_WebRtcRoomMixedReady));
         break;
     default:
         break;
     }
 }

/*本地打开采集设备,包括摄像头、桌面共享、开始插播视频回调*/
 void ToolManager::OnOpenCaptureCallback(VHStreamType streamType, VHCapture code) {
	 if (nullptr != mWndManager){
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);

		 //QString join_uid = QString::number(mLocalStream->mId);
		 //TRACE6("%s  mLocalStream ACCESS_ACCEPTED\n", __FUNCTION__);
		 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_GetLocalStreamSuc, "", "", streamType, code));
	 }
	 
 }
/*推流成功回调*/
 void ToolManager::OnPushStreamSuc(VHStreamType streamType, std::string& streamid) {
	 if (nullptr != mWndManager) {
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);

		 //TRACE6("%s msg:%s\n", __FUNCTION__, id.c_str());
		 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_PushLocalStreamSuc, "", streamid, streamType, 0));
	 }
 }
/*推流失败回调*/
 void ToolManager::OnPushStreamError(VHStreamType streamType, const int codeErr, const std::string& msg ) {
	 if (nullptr != mWndManager) {
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);

		 //TRACE6("%s msg:%s\n", __FUNCTION__, id.c_str());
		 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStreamError(CustomEvent_PushLocalStreamError, streamType, codeErr, msg));
	 }
 }
/*停止推流回调*/
 void ToolManager::OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg) {
	 if (nullptr != mWndManager) {
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);
		 if (vlive::VhallLive_OK == code)
		 {
			 GetDataManager()->WriteLog("%s ：succeced streamType=%d msg=%s", __FUNCTION__, streamType, msg.c_str());
			 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_StopPushLocalStreamSuc, "", "", streamType, VHCapture_OK));
		 }
		 else
		 {
			 GetDataManager()->WriteLog("%s ：faild streamType=%d msg=%s", __FUNCTION__, streamType, msg.c_str());
			 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_StopPushLocalStreamErr, "", "", streamType, VHCapture_OK));
		 }
			 
	 }
 }

/*接收到远端的媒体流  hasVideo ：是否包含视频*/
 void ToolManager::OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio) {
	 if (nullptr != mWndManager) {
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);

		 //QString join_uid = QString::number(mLocalStream->mId);
		 //TRACE6("%s  mLocalStream ACCESS_ACCEPTED\n", __FUNCTION__);
		 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_SubScribSuc, QString::fromStdWString(user).toStdString(), stream, type, hasVideo, hasAudio));
	 }
 }

/*远端的媒体流退出了*/
 void ToolManager::OnRemoveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type) {

	 if (nullptr != mWndManager) {
		 CBaseWnd* pRtcLiveWdg = mWndManager->FindWnd(WND_ID_WEBRTC_MAIN);

		 //QString join_uid = QString::number(mLocalStream->mId);
		 //TRACE6("%s  mLocalStream ACCESS_ACCEPTED\n", __FUNCTION__);
		 QCoreApplication::postEvent(pRtcLiveWdg, new QEventStream(CustomEvent_STREAM_REMOVED, QString::fromStdWString(user).toStdString(), stream, type, 0));
	 }

 }

 void ToolManager::OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid) {

 }
