#pragma once
#include <string>
#include <list>
#include <Windows.h>
#include "callback_monitor.h"

#ifdef  VHPAASSDK_EXPORT
#define VHPAASSDK_EXPORT     __declspec(dllimport)
#else
#define VHPAASSDK_EXPORT     __declspec(dllexport)
#endif

namespace vlive {

class VHPaasInteractionRoom {
public:
    /**
    *   �ӿ�˵�����ص��¼���������ע�ᡣ�˽ӿ�Ϊ��ȡSDK����֮���һ�����õĽӿ�
    */
    virtual bool RegisterVHRoomMonitor(VHRoomMonitor* monitor) = 0;
    /*
    *   �ӿ�˵������ʼ�����ӿڱ������������е��á�
    */
    virtual void InitSDK() = 0;
    /*
    *   �ӿ�˵������½�ӿڡ���¼΢�𻥶�ҵ�����������¼�ɹ�֮�󼴿ɼ��롰����ý�巿�䡱
    *   �ӿڲ�����
    *      accesstoken��ͨ��ƽ̨��ȡ������token
    *      appid��Ӧ��id
    *      thrid_user_id���û�id
    *      inav_roomid����������id
    *   �ص�����������RoomEvent::RoomEvent_Login�¼�
    */
    virtual bool LoginHVRoom(const std::string& accesstoken, const std::string& appid, const std::string& thrid_user_id,const std::string& inav_roomid) = 0;
    /*
    *  �ӿ�˵�����ǳ�����������
    *  �ص�����������RoomEvent::RoomEvent_Logout�¼�
    */ 
    virtual void LogoutHVRoom() = 0;
    /**
    *  �ӿ�˵������ȡ��ǰ�û�Ȩ�ޡ�����Ȩ���ж���Щ���ܿ��á�
    */
    virtual Permission GetPermission() = 0;
    /**
    *  �ӿ�˵�������뻥��ý�巿��
    *  �ص�����������VHRoomConnectEnumö���¼�
    **/
    virtual void JoinVHMediaRoom() = 0;
    /*
    *  �ӿ�˵�����˳�����ý�巿��
    *  ����ֵ��true �˳��ɹ���false �˳�ʧ��
    */
    virtual bool LeaveVHMediaRoom() = 0;
    /*
    *  �ӿ�˵����SDK����������Ա�б�
    *  �ص�������
    *       �ɹ��������ص�OnGetVHRoomMembers
    *       ʧ�ܣ�OnRoomFailedEvent
    */
    virtual void AsynGetVHRoomMembers() = 0;
    /**
    *  �ӿ�˵������������
    *  �ص������¼���RoomEvent::RoomEvent_Apply_Push
    *  ����ֵ��VhallLiveErrCode 
    */
    virtual int ApplyInavPublish() = 0;
    /**
    *  �ӿ�˵���������������
    *  ����˵����
    *    audit_user_id: ������˵������û�ID 
    *    type: �Ƿ�ͬ�⣬AuditPublish
    *  �ص������¼���RoomEvent::RoomEvent_AuditInavPublish
    *  ����ֵ��VhallLiveErrCode
    **/
    virtual int AuditInavPublish(const std::string& audit_user_id, AuditPublish type) = 0; 
    /**
    *  �ӿ�˵������������
    *  ����˵����
    *    audit_user_id: ������˵������û�ID
    *  �ص������¼���RoomEvent::RoomEvent_AskforInavPublish
    *  ����ֵ��VhallLiveErrCode
    **/
    virtual int AskforInavPublish(const std::string& audit_user_id) = 0;
    /**
    *  �ӿ�˵������/��/�ܾ�����״̬��ִ,�û��ɹ���������÷����API
    *  ����˵����
    *       stream_id: ��������ID
    *       type:      PushStreamEvent
    *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void UserPublishCallback(PushStreamEvent type, const std::string& stream_id = std::string()) = 0;
    /**
    *  �ӿ�˵�����߳���/ȡ���߳���
    *  ����˵����
    *     type: 1 �߳�����Ĭ�ϣ� 2 ȡ���߳���
    *     kick_user_id	string	��	���߳��ĵ������û�ID
    *  �ص������¼���RoomEvent::RoomEvent_KickInavStream
    *  ����ֵ��VhallLiveErrCode
    **/
    virtual int KickInavStream(const std::string& kick_user_id, KickStream type) = 0;
    /**
    *  �ӿ�˵�����߳���������/ȡ���߳���������
    *  ����˵����
    *    type:1 �߳��������䣨Ĭ�ϣ� 2 ȡ���߳���������
    *    kick_user_id:���߳��ĵ������û�ID
    *  �ص������¼���RoomEvent::RoomEvent_KickInav
    *  ����ֵ��VhallLiveErrCode
    **/
    virtual int KickInav(const std::string& kick_user_id, KickStream type) = 0;
    /*
    *  �ӿ�˵������ȡ���߳������������б�
    *  �ص�������OnGetVHRoomKickOutMembers��OnRoomFailedEvent
    */
    virtual void GetKickInavList() = 0;
    /*
    *  �ӿ�˵����������·ֱ��,�˽ӿ��ǻ�����������·����Ȩ�޵��û����е��õ�,���������湲���岥ʱ�������ʾ����������Ҫ����SetMainView�ӿڣ�
                  ����Ӧ����ID��Ϊ�����������ã����ֻ����ʾһ·��ͬʱ��Ҫ���ò���ģʽ����Ҫ�����ص�
    *  ����˵����
    *     live_room_id: ֱ������id
    *     layoutMode:�����˲���ģʽ
    *     width�� �����˲��ֵĿ�
    *     height�� �����˲��ֵĸ�
    *     fps��֡��
    *     bitrate�� ����
    *     stream_id�� ������id
    *  �ص�������RoomEvent::RoomEvent_Start_PublishInavAnother
    *  ����ֵ��VhallLiveErrCode
    */
    virtual int StartPublishInavAnother(std::string live_room_id, LayoutMode layoutMode, std::string stream_id = std::string(), BROAD_CAST_DPI dip = BROAD_CAST_DPI_UHD, FRAME_FPS fps = FRAME_FPS_20, BITRATE bitrate = BITRATE_1000) = 0;
    /**
    *  �ӿ�˵������������·ֱ��֮�󣬿��Խ�ĳ���û����������ڻ�������������浱�У��˽ӿ��ǻ�����������·����Ȩ�޵��û����е���
    *  �ص�������RoomEvent::RoomEvent_SetMainView
    *  ����ֵ��VhallLiveErrCode
    **/
    virtual int SetMainView(std::string stream_id) = 0;
    /*
    *   �ӿ�˵����ֹͣ��·ֱ�����˽ӿ��ǻ�����������·����Ȩ�޵��û����е��õ�
    *   ����˵����
    *       live_room_id: ֱ������id
    *   �ص�������RoomEvent::RoomEvent_Stop_PublishInavAnother
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int StopPublishInavAnother(std::string live_room_id) = 0;
    /*
    *   �ӿ�˵�������������Ƿ��Ѿ����ӳɹ�
    *   ����ֵ��true�������������ӣ�false�����������ӶϿ�
    */
    virtual bool IsVHMediaConnected() = 0;
    /*
    *   �ӿ�˵������ȡ����ͷ�б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    */
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras) = 0;
    /*
    *   �ӿ�˵������ȡ��˷��б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    **/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) = 0;
    /**
    *   �ӿ�˵������ȡ�������б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    **/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList) = 0;
    /*
    *  �ӿ�˵��������ʹ�õ���˷�
    *  ����˵����
    *   index: GetMicDevices��ȡ���б��� VhallLiveDeviceInfo�е� devIndex
    */
    virtual void SetUseMicIndex(int index) = 0;
    /*
    *  �ӿ�˵��������ʹ�õ�����ͷ
    *  ����˵����
    *   index: GetCameraDevices��ȡ���б��� vhall::VideoDevProperty�е� Guid
    */
    virtual void SetUseCameraGuid(std::string guid) = 0;
    /*
    *  �ӿ�˵��������ʹ�õ�������
    *  ����˵����
    *   index: GetPlayerDevices��ȡ���б��� VhallLiveDeviceInfo�е� devIndex
    */
    virtual void SetUserPlayIndex(int index) = 0;
    /*
    *   �ӿ�˵�������õ�ǰʹ�õ���˷�����
    *   ����˵����
    *       vol:������Χ 0~100
    **/
    virtual bool SetCurrentMicVol(int vol) = 0;
    /*
    *   �ӿ�˵������ȡ��ǰʹ�õ���˷�����
    *   ����ֵ����ǰ����
    **/
    virtual int GetCurrentMicVol() = 0;
    /*
    *   �ӿ�˵�������õ�ǰʹ�õ�����������
    *   ����˵����
    *       vol:������Χ 0~100
    **/
    virtual bool SetCurrentPlayVol(int vol) = 0;
    /*
    *   �ӿ�˵������ȡ��ǰʹ�õ�����������
    *   ����ֵ�����ص�ǰ������ֵ
    **/
    virtual int GetCurrentPlayVol() = 0;
    /*
    *   �ӿ�˵�����ر�����ͷ
    *   ����ֵ��true �رճɹ�/ false �ر�ʧ��
    */
    virtual bool CloseCamera() = 0;
    /*
    *   �ӿ�˵�������Ѿ���ʼ�ɼ�������ͷ
    *   ����ֵ��true �򿪳ɹ�/ false ��ʧ��
    */
    virtual bool OpenCamera() = 0;
    /*
    *   �ӿ�˵��������ͷ�Ƿ�ر�
    *   ����ֵ��true �Ѿ��ر�/ false δ�ر�
    */
    virtual bool IsCameraOpen() = 0;
    /*
    *   �ӿ�˵�����ر���˷�
    *   ����ֵ��true �رճɹ�/ false �ر�ʧ��
    */
    virtual bool CloseMic() = 0;
    /*
    *   �ӿ�˵��������˷�
    *   ����ֵ��true �򿪳ɹ�/ false ��ʧ��
    */
    virtual bool OpenMic() = 0;
    /*
    *   �ӿ�˵������˷��Ƿ��
    *   ����ֵ��true �Ѿ��ر�/ false δ�ر�
    */
    virtual bool IsMicOpen() = 0;
    /*
    *   �ӿ�˵�����򿪡��ر�Զ���û�������Ƶ
    *   ����˵����
    *       user_id���û�id
    *       close��true �ر�/ false ��
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int OperateRemoteUserVideo(std::wstring user_id, bool close = false) = 0;
    /*
    *   �ӿ�˵�����򿪡��ر�Զ���û���������
    *   ����˵����
    *       user_id���û�id
    *       close��true �ر�/ false ��
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int OperateRemoteUserAudio(std::wstring user_id, bool close = false) = 0;
    /*  
    *   �ӿ�˵������ʼ����ͷ\��˷�ɼ�  
    *   ����˵����
    *       VideoProfileIndex��ͨ����ȡ�豸��Ϣʱ�õ�VideoDevProperty::mDevFormatList���豸֧�ֵķֱ��ʣ����ݷֱ�����Ϣѡ����ӦVideoProfileIndex
    *   ������OnOpenCaptureCallback
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int StartLocalCapture(VideoProfileIndex index) = 0;
    /**
    *   �ӿ�˵�����ж���ѡ�豸�Ƿ�֧����ѡ�ֱ���
    *   ����ֵ��ture֧�֡�false��֧��
    **/
    virtual bool IsSupported(const std::string& devGuid,VideoProfileIndex index) = 0;
    /*
    *  �ӿ�˵�����жϸ����豸�Ƿ�֧�ָ����ֱ���
    *  dev �����豸
    *  iProfileIndex �ֱ�������
    */
    virtual bool IsSupported(std::shared_ptr<vhall::VideoDevProperty>& dev, int iProfileIndex) = 0;
    /*
    *   �ӿ�˵����ֹͣ����ͷ\��˷�ɼ�  ����Ѿ���ʼ������ͬʱֹͣ����
    */
    virtual void StopLocalCapture() = 0;
    /*
    *   �ӿ�˵������ʼ����ͷ�������� �������ȴ�����ͷ֮���������
    *   �ص�������OnPushStreamSuc OnPushStreamError
    */
    virtual int StartPushLocalStream() = 0;
    /*
    *   �ӿ�˵����ֹͣ����ͷ�������� 
    *   �ص������� OnStopPushStreamCallback
    */
    virtual int StopPushLocalStream() = 0;
    /*
    *   �ӿ�˵������ǰ�ɼ������Ƿ����ڽ��б�������Դ�ɼ�
    */
    virtual bool IsCapturingStream(VHStreamType streamType) = 0;
    /**
    *   �ӿ�˵������ǰ���������Ƿ�������湲����Ƶ��
    */
    virtual bool IsUserPushingDesktopStream() = 0;
    /**
    *   �ӿ�˵������ǰ���������Ƿ���ڲ岥��Ƶ��
    *   ����ֵ��ture�ɹ���falseʧ��
    */
    virtual bool IsUserPushingMediaFileStream() = 0;
    /**
    *   �ӿ�˵������ǰ�ɼ������Ƿ���������
    *   ����ֵ��ture�ɹ���falseʧ��
    */
    virtual bool IsPushingStream(VHStreamType streamType) = 0;
    /**
    *   �ӿ�˵������ȡ���û�����ID
    *   ����˵����
    *       user_id���û�id
    *       streamType: ������
    *   ����ֵ����id
    */
    virtual std::string GetUserStreamID(const std::wstring user_id, VHStreamType streamType) = 0;
    /*
    *   �ӿ�˵������ʼ��Ⱦý��������
    *   ����˵����
    *       user_id���û�id
    *       wnd�� ��Ⱦ���ھ��
    *       streamType: ������
    *   ����ֵ��ture�ɹ���falseʧ��
    */
    virtual bool StartRenderStream(const std::wstring& user,void*  wnd, vlive::VHStreamType streamType) = 0;
    /**
    *   �ӿ�˵�����������湲����������ߡ� ��������ô˽ӿڣ�Ĭ����������1280 * 720 �ֱ�������
    *   ����˵����
    *       width:������
    *       height:������
    *       fps:֡��
    *       bitrate:���� ��λbps;
    **/
    virtual void SetDesktopPushStreamParam(int width, int height, int fps = 20, int bitRate = 600) = 0;
    /*
    *   �ӿ�˵������ʼ���湲��ɼ�  
    *   ������OnOpenCaptureCallback
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int StartDesktopCapture(int index, int width, int height) = 0;
    /*
    *   �ӿ�˵����ֹͣ���湲��ɼ�  ����Ѿ���ʼ������ͬʱֹͣ����
    */
    virtual void StopDesktopCapture() = 0;
    /*
    *   �ӿ�˵������ʼ���湲��ɼ����� 
    *   �ص���⣺OnPushStreamSuc OnPushStreamError
    */
    virtual int StartPushDesktopStream() = 0;
    /*
    *   �ӿ�˵����ֹͣ���湲��ɼ����� 
    *   ��Ҫ�ص����:OnStopPushStreamCallback
    *   ����ֵ��VhallLiveErrCode
    */
    virtual int StopPushDesktopStream() = 0;
    /**
    *   �ӿ�˵�������ò岥�ļ�������ߡ�
    *   ��������ô˽ӿڣ�Ĭ����������960 * 540 �ֱ�������
    **/
    virtual void SetMediaFilePushStreamParam(int width, int height, int fps = 20, int bitRate = 600) = 0;
    /*
    *   �ӿ�˵������ʼ�岥�ļ�
    *   ����ֵ��VhallLiveErrCode
    *   �ص�������OnOpenCaptureCallback
    */
    virtual int StartPlayMediaFile(std::string filePath) = 0;
    /*
    *   �ӿ�˵�����鿴��ǰ���ŵ��ļ��Ƿ�֧����Ƶ����Ƶ��ʽ�������жϵ�ǰ���ŵ�����Ƶ�ļ�������Ƶ�ļ�
    *   �ӿڲ�����
          type����Ƶ����Ƶ
    *   ����ֵ��true ֧��/ false ��֧��
    */
    virtual bool IsSupportMediaFormat(CaptureStreamAVType type) = 0;
    /*
    *   �ӿ�˵����ֹͣ�岥�ļ�, ����Ѿ���ʼ������ͬʱֹͣ����
    */
    virtual void StopMediaFileCapture() = 0;
    /*
    *   �ӿ�˵������ʼ�岥�ļ����� 
    *   ����ֵ��VhallLiveErrCode
    *   �ص���⣺OnPushStreamError  OnPushStreamSuc
    */
    virtual int StartPushMediaFileStream() = 0;
    /*
    *   �ӿ�˵����ֹͣ�岥�ļ�����
    *   �ص���⣺OnStopPushMediaFileStream
    */
    virtual void StopPushMediaFileStream() = 0;
    /*
    *   �ӿ�˵�����岥�ļ���ͣ����
    *   ����ֵ��VhallLiveErrCode
    */
    virtual void MediaFilePause() = 0;
    /*
    *   �岥�ļ��ָ�����
    *   ����ֵ��VhallLiveErrCode
    */
    virtual void MediaFileResume() = 0;
    /*
    *   �ӿ�˵�����岥�ļ��������
    *   ����ֵ��VhallLiveErrCode
    */
    virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) = 0;
    /*
    *   �ӿ�˵�����岥�ļ���ʱ��
    */
    virtual const long long MediaFileGetMaxDuration() = 0;
    /*  
    *   �ӿ�˵�����岥�ļ���ǰʱ��
    */
    virtual const long long MediaFileGetCurrentDuration() = 0;
    /*
    *   �ӿ�˵�����岥�ļ��ĵ�ǰ״̬ 
    *   ����ֵ������״̬  MEDIA_FILE_PLAY_STATE
    */
    virtual int MediaGetPlayerState() = 0;
};

VHPAASSDK_EXPORT VHPaasInteractionRoom* GetPaasSDKInstance();
VHPAASSDK_EXPORT void DestoryPaasSDKInstance();

}