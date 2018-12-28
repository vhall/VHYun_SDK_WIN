#pragma once

#include <QObject>
#include <mutex>  
#include <QEvent>
#include <QEventLoop>
#include "WndManager.h"
#include "LibManager.h"
#include "DataManagerInterface.h"
#include "ConfigManagerInterface.h"
#include "VHPaasInteractionRoom.h"
#include "callback_monitor.h"
#include <mutex>

using namespace vlive;
#define SaveRelease(a)\
if(a){            \
   delete a;      \
   a = nullptr;   \
}                 \

class  ToolManager : public QObject,public VHRoomMonitor
{
   Q_OBJECT

public:
   static ToolManager* GetInstance();
   static void ReleaseInstance();

   void Init();
   void Release();
   void RegisterQEventReciver(QObject* obj);
   void RemoveQEventReciver(QObject* obj);
   void PostEventToMainThread(QEvent* event);
   void AppExec();
   void AppQuit();

    /*监听互动房间内所有成功事件*/
    virtual void OnRoomSuccessedEvent(RoomEvent code, std::string userData = std::string());
    virtual void OnRoomFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string());

    virtual void OnGetVHRoomMembers(const std::string&, std::list<VHRoomMember>&);

    virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&);
    /*
    *   本地网络重连成功之后重推流，流ID发生改变通知
    **/
    virtual void OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid);

    /*收到申请上麦消息 ,消息为广播消息，收到消息后，通过进入互动房间时获取的权限列表，判断用户是否有审核权限*/
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id) ;
    /*收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作*/
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
    /*邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息*/
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& third_party_user_id);
    /*踢出流消息  消息为广播消息，收到消息后，执行踢出流*/
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id);
    /*踢出互动房间 , 消息为广播消息，收到消息后，判断是当前用户后，执行踢出房间操作*/
    virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id);
    /*上/下/拒绝上麦消息 消息为广播消息*/
    virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event);
    /*互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间*/
    virtual void OnRecvInavCloseMsg();
    /*
    *   用户上下线通知
    *   online： true 用户上线/ false 用户下线
    *   user_id： 用户id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id);


    /*房间链接事件*/
    virtual void OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum/*, const std::string&*/);
    /*本地打开采集设备,包括摄像头、桌面共享、开始插播视频回调*/
    virtual void OnOpenCaptureCallback(VHStreamType streamType, VHCapture code);
    /*开始推流成功回调*/
    virtual void OnPushStreamSuc(VHStreamType streamType, std::string& streamid);
    /*推流失败回调*/
    virtual void OnPushStreamError(VHStreamType streamType, const int codeErr = 0, const std::string& msg = std::string());
    /*停止推流成功回调*/
    virtual void OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg);
    /*接收到远端的媒体流  hasVideo ：是否包含视频*/
    virtual void OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio);
    /*远端的媒体流退出了*/
    virtual void OnRemoveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type);

   LibManager* GetLibManager();
   WndManager* GetWndManager();
   DataManagerInterface* GetDataManager();
   ConfigSettingInterface* GetConfigSettingInterface();
   VHPaasInteractionRoom* GetPaasSDK();
private:
   ToolManager(QObject *parent = nullptr);
   ~ToolManager();
private:

   static ToolManager* mpInstance;
   static std::mutex mInstanceMutex;
   LibManager* mLibManager = nullptr;    //动态库/lib库管理对象
   WndManager* mWndManager = nullptr;    //窗口库管理对象
   DataManagerInterface* mDataManager = nullptr;  //数据库管理对象
   ConfigSettingInterface* mConfigManagerInterface = nullptr;
   VHPaasInteractionRoom* mpVHPaasInteractionRoom = nullptr;
   std::mutex mEventRecvMutex;
   std::list<QObject*> mEventReciver;
   QEventLoop mQuitLoop;

};  


