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

    /*�����������������гɹ��¼�*/
    virtual void OnRoomSuccessedEvent(RoomEvent code, std::string userData = std::string());
    virtual void OnRoomFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string());

    virtual void OnGetVHRoomMembers(const std::string&, std::list<VHRoomMember>&);

    virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&);
    /*
    *   �������������ɹ�֮������������ID�����ı�֪ͨ
    **/
    virtual void OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid);

    /*�յ�����������Ϣ ,��ϢΪ�㲥��Ϣ���յ���Ϣ��ͨ�����뻥������ʱ��ȡ��Ȩ���б��ж��û��Ƿ������Ȩ��*/
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id) ;
    /*�յ����������Ϣ ,��ϢΪ�㲥��Ϣ���յ�ͬ�������ִ���������*/
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
    /*����������Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ����ʾ������Ϣ*/
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& third_party_user_id);
    /*�߳�����Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ��ִ���߳���*/
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id);
    /*�߳��������� , ��ϢΪ�㲥��Ϣ���յ���Ϣ���ж��ǵ�ǰ�û���ִ���߳��������*/
    virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id);
    /*��/��/�ܾ�������Ϣ ��ϢΪ�㲥��Ϣ*/
    virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event);
    /*��������ر���Ϣ  ���ܵ�����Ϣ�������ڻ����������Ա��ȫ�������˳�����*/
    virtual void OnRecvInavCloseMsg();
    /*
    *   �û�������֪ͨ
    *   online�� true �û�����/ false �û�����
    *   user_id�� �û�id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id);


    /*���������¼�*/
    virtual void OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum/*, const std::string&*/);
    /*���ش򿪲ɼ��豸,��������ͷ�����湲����ʼ�岥��Ƶ�ص�*/
    virtual void OnOpenCaptureCallback(VHStreamType streamType, VHCapture code);
    /*��ʼ�����ɹ��ص�*/
    virtual void OnPushStreamSuc(VHStreamType streamType, std::string& streamid);
    /*����ʧ�ܻص�*/
    virtual void OnPushStreamError(VHStreamType streamType, const int codeErr = 0, const std::string& msg = std::string());
    /*ֹͣ�����ɹ��ص�*/
    virtual void OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg);
    /*���յ�Զ�˵�ý����  hasVideo ���Ƿ������Ƶ*/
    virtual void OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio);
    /*Զ�˵�ý�����˳���*/
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
   LibManager* mLibManager = nullptr;    //��̬��/lib��������
   WndManager* mWndManager = nullptr;    //���ڿ�������
   DataManagerInterface* mDataManager = nullptr;  //���ݿ�������
   ConfigSettingInterface* mConfigManagerInterface = nullptr;
   VHPaasInteractionRoom* mpVHPaasInteractionRoom = nullptr;
   std::mutex mEventRecvMutex;
   std::list<QObject*> mEventReciver;
   QEventLoop mQuitLoop;

};  


