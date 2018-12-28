#ifndef H_WEBRTRCSDK_EVENT_INTERFACE_H
#define H_WEBRTRCSDK_EVENT_INTERFACE_H

#include "vlive_def.h"

/*
**===================================
**
**   ����SDK�¼������ص��ӿڣ����нӿڻص�����SDKҵ���̡߳�
**   ����ص�����appҵ���¼��������ڻص��н��С�
**
**===================================
*/

class WebRtcSDKEventInterface {
public:
    /*
    *   ���������¼�
    */
    virtual void OnWebRtcRoomConnetEventCallback(const vlive::VHRoomConnectEnum/*,const std::string&*/) = 0;
    /*
    *   ���ش򿪲ɼ��豸,��������ͷ�����湲����ʼ�岥��Ƶ�ص�,
    */
    virtual void OnOpenCaptureCallback(vlive::VHStreamType streamType, vlive::VHCapture code) = 0;
    /*
    *   �����ɹ��ص�
    */
    virtual void OnPushStreamSuc(vlive::VHStreamType streamType, std::string& streamid) = 0;
    /*
    *   ����ʧ�ܻص�
    */
    virtual void OnPushStreamError(vlive::VHStreamType streamType, const int codeErr = 0, const std::string& msg = std::string()) = 0;
    /*
    *   �������������ɹ�֮������������ID�����ı�֪ͨ
    **/
    virtual void OnRePublishStreamIDChanged(vlive::VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid , const std::string& new_streamid) = 0;
    /*
    *   ֹͣ�����ص�
    *   code: 0�ɹ��� ����ʧ��
    */
    virtual void OnStopPushStreamCallback(vlive::VHStreamType streamType,int code, const std::string& msg) = 0;
    /*
    *    ���յ�Զ�˵�ý����  hasVideo ���Ƿ������Ƶ
    */
    virtual void OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const vlive::VHStreamType type, bool hasVideo, bool hasAudio) = 0;
    /*
    *   Զ�˵�ý�����˳���
    */
    virtual void OnRemoveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const vlive::VHStreamType type) = 0;
};

#endif // !H_WEBRTRCSDK_EVENT_INTERFACE_H
