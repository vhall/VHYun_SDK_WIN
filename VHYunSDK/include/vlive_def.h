#ifndef H_VLIVE_DEF_H
#define H_VLIVE_DEF_H

#include <string>
#include <video_profile.h>
#include <vhall_dev_format.h>

namespace vlive {
    class WebRtcRoomOption {
    public:
        //http://wiki.vhallops.com/pages/viewpage.action?pageId=2491279
        std::wstring strRoomToken;           //����token
        std::wstring strLogUpLogUrl;         //��־�ϱ�URL
        std::wstring strUserId;              //�û�ID
        std::wstring strRoomId;              //����ID
        std::wstring strThirdPushStreamUrl;  //�����˵�ַ���û������˲��ֲ���
        std::wstring strappid = L"0";       //Ӧ��id
        std::wstring strBusinesstype = L"0";    //ҵ�����ͣ�ƽ̨�����б�0: saas 1: pass
        std::wstring strAccountId = L"0";       //������ID
        int nWebRtcSdkReConnetTime = 3000; /* �������ӳ�ʱ����������SDK�ײ�ҵ��û3�봥��һ�Σ�3000��������Ͽ�ʱ������ʱʱ��Ϊ9000�� */
    };

    enum VHStreamType {
        VHStreamType_PureAudio = 0,   //����Ƶ����
        VHStreamType_PureVideo = 1,   //����Ƶ����
        VHStreamType_AVCapture = 2,   //����Ƶ����
        VHStreamType_Desktop = 3,     //���湲������
        VHStreamType_MediaFile = 4,   //�岥��Ƶ����
        VHStreamType_Preview_Video = 5,   //Ԥ����Ƶ����
        VHStreamType_Count
    };

    enum CaptureStreamAVType {
        CaptureStreamAVType_Audio = 0,
        CaptureStreamAVType_Video
    };


    enum VhallLiveErrCode {
        VhallLive_OK = 0,
        VhallLive_ROOM_DISCONNECT,      //�����Ѿ��Ͽ��������쳣
        VhallLive_NO_OBJ,               //���õĶ���Ϊ��
        VhallLive_NO_DEVICE,            //û������Ƶ�豸
        VhallLive_NO_PERMISSION,        //��Ȩ��
        VhallLive_MUTEX_OPERATE,        //������������湲��Ͳ岥һ������ֻ��һ����Ա����
        VhallLive_NOT_SUPPROT,          //��֧�ֵķֱ���
        VhallLive_PARAM_ERR,            //��������
        VhallLive_IS_PROCESSING,        //���ڴ�����
        VhallLive_SERVER_NOT_READY,     //��������δ����
        VhallLive_ERROR,
    };

    enum MEDIA_FILE_PLAY_STATE
    {
        PLAY_STATE_NothingSpecial = 0,
        PLAY_STATE_Opening,               //���ڴ��ļ�
        PLAY_STATE_Buffering,             //���ڼ����ļ�
        PLAY_STATE_Playing,               //���ڲ�����
        PLAY_STATE_Paused,                //����ͣ
        PLAY_STATE_Stopped,               //��ֹͣ
        PLAY_STATE_Ended,                 //�������
        PLAY_STATE_Error                  //����ʧ��
    };


    enum VHRoomConnectEnum
    {
        VHRoomConnect_ROOM_CONNECTED = 0,       //�����������ӳɹ�
        VHRoomConnect_ROOM_ERROR,               //������������ʧ�ܣ��������Գ�ʱ֮���ϱ�����Ϣ
        VHRoomConnect_ROOM_DISCONNECTED,        //�����쳣ʱ��ʾ�����Ѿ��Ͽ�
        VHRoomConnect_ROOM_RECONNECTING,        //���ڽ�����������
        VHRoomConnect_RE_CONNECTED,             //���������Ѿ��ָ�
        VHRoomConnect_ROOM_RECOVER,             //�����Ѿ��ָ�
        VHRoomConnect_ROOM_NETWORKCHANGED,      //���緢���仯
        VHRoomConnect_ROOM_NETWORKRECOVER,      //�����Ѿ��ָ�
        VHRoomConnect_ROOM_MIXED_STREAM_READY, //����������·����ʹ���¼��������յ�����Ϣ֮��SDK�˲��ܽ�����·���������
    };

    enum VHCapture {
        //success
        VHCapture_OK = 0,               //�ɼ��ɹ�
        VHCapture_ACCESS_DENIED,        //�ɼ�ʧ��
        VHCapture_VIDEO_DENIED,         //��Ƶ�ɼ�ʧ��
        VHCapture_AUDIO_DENIED,         //��Ƶ�ɼ�ʧ��
        VHCapture_STREAM_SOURCE_LOST,   //�����豸�ɼ������вɼ�ʧ�� ,SDK�������ɼ�������
        VHCapture_PLAY_FILE_ERR,        //�����ļ�ʧ��
    };
}


#endif // !H_VLIVE_DEF_H