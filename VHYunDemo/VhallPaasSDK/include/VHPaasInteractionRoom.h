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
    *   接口说明：回调事件监听对象注册。此接口为获取SDK对象之后第一个调用的接口
    */
    virtual bool RegisterVHRoomMonitor(VHRoomMonitor* monitor) = 0;
    /*
    *   接口说明：初始化。接口必须在主函数中调用。
    */
    virtual void InitSDK() = 0;
    /*
    *   接口说明：登陆接口。登录微吼互动业务服务器，登录成功之后即可加入“互动媒体房间”
    *   接口参数：
    *      accesstoken：通过平台获取产生的token
    *      appid：应用id
    *      thrid_user_id：用户id
    *      inav_roomid：互动房间id
    *   回调监听：监听RoomEvent::RoomEvent_Login事件
    */
    virtual bool LoginHVRoom(const std::string& accesstoken, const std::string& appid, const std::string& thrid_user_id,const std::string& inav_roomid) = 0;
    /*
    *  接口说明：登出互动服务处理。
    *  回调监听：监听RoomEvent::RoomEvent_Logout事件
    */ 
    virtual void LogoutHVRoom() = 0;
    /**
    *  接口说明：获取当前用户权限。根据权限判断那些功能可用。
    */
    virtual Permission GetPermission() = 0;
    /**
    *  接口说明：进入互动媒体房间
    *  回调监听：监听VHRoomConnectEnum枚举事件
    **/
    virtual void JoinVHMediaRoom() = 0;
    /*
    *  接口说明：退出互动媒体房间
    *  返回值：true 退出成功；false 退出失败
    */
    virtual bool LeaveVHMediaRoom() = 0;
    /*
    *  接口说明：SDK互动房间人员列表
    *  回调监听：
    *       成功：监听回调OnGetVHRoomMembers
    *       失败：OnRoomFailedEvent
    */
    virtual void AsynGetVHRoomMembers() = 0;
    /**
    *  接口说明：申请上麦。
    *  回调监听事件：RoomEvent::RoomEvent_Apply_Push
    *  返回值：VhallLiveErrCode 
    */
    virtual int ApplyInavPublish() = 0;
    /**
    *  接口说明：审核申请上麦
    *  参数说明：
    *    audit_user_id: 受审核人第三放用户ID 
    *    type: 是否同意，AuditPublish
    *  回调监听事件：RoomEvent::RoomEvent_AuditInavPublish
    *  返回值：VhallLiveErrCode
    **/
    virtual int AuditInavPublish(const std::string& audit_user_id, AuditPublish type) = 0; 
    /**
    *  接口说明：邀请上麦
    *  参数说明：
    *    audit_user_id: 受审核人第三放用户ID
    *  回调监听事件：RoomEvent::RoomEvent_AskforInavPublish
    *  返回值：VhallLiveErrCode
    **/
    virtual int AskforInavPublish(const std::string& audit_user_id) = 0;
    /**
    *  接口说明：上/下/拒绝上麦状态回执,用户成功推流后调用服务端API
    *  参数说明：
    *       stream_id: 操作的流ID
    *       type:      PushStreamEvent
    *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void UserPublishCallback(PushStreamEvent type, const std::string& stream_id = std::string()) = 0;
    /**
    *  接口说明：踢出流/取消踢出流
    *  参数说明：
    *     type: 1 踢出流（默认） 2 取消踢出流
    *     kick_user_id	string	是	被踢出的第三方用户ID
    *  回调监听事件：RoomEvent::RoomEvent_KickInavStream
    *  返回值：VhallLiveErrCode
    **/
    virtual int KickInavStream(const std::string& kick_user_id, KickStream type) = 0;
    /**
    *  接口说明：踢出互动房间/取消踢出互动房间
    *  参数说明：
    *    type:1 踢出互动房间（默认） 2 取消踢出互动房间
    *    kick_user_id:被踢出的第三方用户ID
    *  回调监听事件：RoomEvent::RoomEvent_KickInav
    *  返回值：VhallLiveErrCode
    **/
    virtual int KickInav(const std::string& kick_user_id, KickStream type) = 0;
    /*
    *  接口说明：获取被踢出互动房间人列表
    *  回调函数：OnGetVHRoomKickOutMembers、OnRoomFailedEvent
    */
    virtual void GetKickInavList() = 0;
    /*
    *  接口说明：开启旁路直播,此接口是互动房间有旁路推流权限的用户进行调用的,当处理桌面共享或插播时如果想显示在主画面需要调用SetMainView接口，
                  将对应的流ID作为参数进行设置，如果只想显示一路则同时需要设置布局模式。需要监听回调
    *  参数说明：
    *     live_room_id: 直播房间id
    *     layoutMode:混流端布局模式
    *     width： 混流端布局的宽
    *     height： 混流端布局的高
    *     fps：帧率
    *     bitrate： 码率
    *     stream_id： 大画面流id
    *  回调监听：RoomEvent::RoomEvent_Start_PublishInavAnother
    *  返回值：VhallLiveErrCode
    */
    virtual int StartPublishInavAnother(std::string live_room_id, LayoutMode layoutMode, std::string stream_id = std::string(), BROAD_CAST_DPI dip = BROAD_CAST_DPI_UHD, FRAME_FPS fps = FRAME_FPS_20, BITRATE bitrate = BITRATE_1000) = 0;
    /**
    *  接口说明：当开启旁路直播之后，可以将某个用户的流设置在混流画面的主画面当中，此接口是互动房间有旁路推流权限的用户进行调用
    *  回调监听：RoomEvent::RoomEvent_SetMainView
    *  返回值：VhallLiveErrCode
    **/
    virtual int SetMainView(std::string stream_id) = 0;
    /*
    *   接口说明：停止旁路直播。此接口是互动房间有旁路推流权限的用户进行调用的
    *   参数说明：
    *       live_room_id: 直播房间id
    *   回调监听：RoomEvent::RoomEvent_Stop_PublishInavAnother
    *   返回值：VhallLiveErrCode
    */
    virtual int StopPublishInavAnother(std::string live_room_id) = 0;
    /*
    *   接口说明：互动房间是否已经连接成功
    *   返回值：true互动房间已连接，false互动房间链接断开
    */
    virtual bool IsVHMediaConnected() = 0;
    /*
    *   接口说明：获取摄像头列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    */
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras) = 0;
    /*
    *   接口说明：获取麦克风列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    **/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) = 0;
    /**
    *   接口说明：获取扬声器列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    **/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList) = 0;
    /*
    *  接口说明：设置使用的麦克风
    *  参数说明：
    *   index: GetMicDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
    */
    virtual void SetUseMicIndex(int index) = 0;
    /*
    *  接口说明：设置使用的摄像头
    *  参数说明：
    *   index: GetCameraDevices获取的列表中 vhall::VideoDevProperty中的 Guid
    */
    virtual void SetUseCameraGuid(std::string guid) = 0;
    /*
    *  接口说明：设置使用的扬声器
    *  参数说明：
    *   index: GetPlayerDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
    */
    virtual void SetUserPlayIndex(int index) = 0;
    /*
    *   接口说明：设置当前使用的麦克风音量
    *   参数说明：
    *       vol:参数范围 0~100
    **/
    virtual bool SetCurrentMicVol(int vol) = 0;
    /*
    *   接口说明：获取当前使用的麦克风音量
    *   返回值：当前音量
    **/
    virtual int GetCurrentMicVol() = 0;
    /*
    *   接口说明：设置当前使用的扬声器音量
    *   参数说明：
    *       vol:参数范围 0~100
    **/
    virtual bool SetCurrentPlayVol(int vol) = 0;
    /*
    *   接口说明：获取当前使用的扬声器音量
    *   返回值：返回当前的音量值
    **/
    virtual int GetCurrentPlayVol() = 0;
    /*
    *   接口说明：关闭摄像头
    *   返回值：true 关闭成功/ false 关闭失败
    */
    virtual bool CloseCamera() = 0;
    /*
    *   接口说明：打开已经开始采集的摄像头
    *   返回值：true 打开成功/ false 打开失败
    */
    virtual bool OpenCamera() = 0;
    /*
    *   接口说明：摄像头是否关闭
    *   返回值：true 已经关闭/ false 未关闭
    */
    virtual bool IsCameraOpen() = 0;
    /*
    *   接口说明：关闭麦克风
    *   返回值：true 关闭成功/ false 关闭失败
    */
    virtual bool CloseMic() = 0;
    /*
    *   接口说明：打开麦克风
    *   返回值：true 打开成功/ false 打开失败
    */
    virtual bool OpenMic() = 0;
    /*
    *   接口说明：麦克风是否打开
    *   返回值：true 已经关闭/ false 未关闭
    */
    virtual bool IsMicOpen() = 0;
    /*
    *   接口说明：打开、关闭远端用户本地视频
    *   参数说明：
    *       user_id：用户id
    *       close：true 关闭/ false 打开
    *   返回值：VhallLiveErrCode
    */
    virtual int OperateRemoteUserVideo(std::wstring user_id, bool close = false) = 0;
    /*
    *   接口说明：打开、关闭远端用户本地声音
    *   参数说明：
    *       user_id：用户id
    *       close：true 关闭/ false 打开
    *   返回值：VhallLiveErrCode
    */
    virtual int OperateRemoteUserAudio(std::wstring user_id, bool close = false) = 0;
    /*  
    *   接口说明：开始摄像头\麦克风采集  
    *   参数说明：
    *       VideoProfileIndex：通过获取设备信息时得到VideoDevProperty::mDevFormatList中设备支持的分辨率，根据分辨率信息选区对应VideoProfileIndex
    *   监听：OnOpenCaptureCallback
    *   返回值：VhallLiveErrCode
    */
    virtual int StartLocalCapture(VideoProfileIndex index) = 0;
    /**
    *   接口说明：判断所选设备是否支持所选分辨率
    *   返回值：ture支持。false不支持
    **/
    virtual bool IsSupported(const std::string& devGuid,VideoProfileIndex index) = 0;
    /*
    *  接口说明：判断给定设备是否支持给定分辨率
    *  dev 给定设备
    *  iProfileIndex 分辨率索引
    */
    virtual bool IsSupported(std::shared_ptr<vhall::VideoDevProperty>& dev, int iProfileIndex) = 0;
    /*
    *   接口说明：停止摄像头\麦克风采集  如果已经开始推流会同时停止推流
    */
    virtual void StopLocalCapture() = 0;
    /*
    *   接口说明：开始摄像头数据推流 ，必须先打开摄像头之后才能推流
    *   回调监听：OnPushStreamSuc OnPushStreamError
    */
    virtual int StartPushLocalStream() = 0;
    /*
    *   接口说明：停止摄像头数据推流 
    *   回调监听： OnStopPushStreamCallback
    */
    virtual int StopPushLocalStream() = 0;
    /*
    *   接口说明：当前采集类型是否正在进行本地数据源采集
    */
    virtual bool IsCapturingStream(VHStreamType streamType) = 0;
    /**
    *   接口说明：当前互动房间是否存在桌面共享视频流
    */
    virtual bool IsUserPushingDesktopStream() = 0;
    /**
    *   接口说明：当前互动房间是否存在插播视频流
    *   返回值：ture成功。false失败
    */
    virtual bool IsUserPushingMediaFileStream() = 0;
    /**
    *   接口说明：当前采集类型是否正在推流
    *   返回值：ture成功。false失败
    */
    virtual bool IsPushingStream(VHStreamType streamType) = 0;
    /**
    *   接口说明：获取流用户推流ID
    *   参数说明：
    *       user_id：用户id
    *       streamType: 流类型
    *   返回值：流id
    */
    virtual std::string GetUserStreamID(const std::wstring user_id, VHStreamType streamType) = 0;
    /*
    *   接口说明：开始渲染媒体数据流
    *   参数说明：
    *       user_id：用户id
    *       wnd： 渲染窗口句柄
    *       streamType: 流类型
    *   返回值：ture成功。false失败
    */
    virtual bool StartRenderStream(const std::wstring& user,void*  wnd, vlive::VHStreamType streamType) = 0;
    /**
    *   接口说明：设置桌面共享推流最大宽高。 如果不调用此接口，默认推流按照1280 * 720 分辨率推流
    *   参数说明：
    *       width:推流宽
    *       height:推流高
    *       fps:帧率
    *       bitrate:码率 单位bps;
    **/
    virtual void SetDesktopPushStreamParam(int width, int height, int fps = 20, int bitRate = 600) = 0;
    /*
    *   接口说明：开始桌面共享采集  
    *   监听：OnOpenCaptureCallback
    *   返回值：VhallLiveErrCode
    */
    virtual int StartDesktopCapture(int index, int width, int height) = 0;
    /*
    *   接口说明：停止桌面共享采集  如果已经开始推流会同时停止推流
    */
    virtual void StopDesktopCapture() = 0;
    /*
    *   接口说明：开始桌面共享采集推流 
    *   回调检测：OnPushStreamSuc OnPushStreamError
    */
    virtual int StartPushDesktopStream() = 0;
    /*
    *   接口说明：停止桌面共享采集推流 
    *   需要回调检测:OnStopPushStreamCallback
    *   返回值：VhallLiveErrCode
    */
    virtual int StopPushDesktopStream() = 0;
    /**
    *   接口说明：设置插播文件推流宽高。
    *   如果不调用此接口，默认推流按照960 * 540 分辨率推流
    **/
    virtual void SetMediaFilePushStreamParam(int width, int height, int fps = 20, int bitRate = 600) = 0;
    /*
    *   接口说明：开始插播文件
    *   返回值：VhallLiveErrCode
    *   回调监听：OnOpenCaptureCallback
    */
    virtual int StartPlayMediaFile(std::string filePath) = 0;
    /*
    *   接口说明：查看当前播放的文件是否支持音频或视频格式，用于判断当前播放的是音频文件还是视频文件
    *   接口参数：
          type：音频、视频
    *   返回值：true 支持/ false 不支持
    */
    virtual bool IsSupportMediaFormat(CaptureStreamAVType type) = 0;
    /*
    *   接口说明：停止插播文件, 如果已经开始推流会同时停止推流
    */
    virtual void StopMediaFileCapture() = 0;
    /*
    *   接口说明：开始插播文件推流 
    *   返回值：VhallLiveErrCode
    *   回调检测：OnPushStreamError  OnPushStreamSuc
    */
    virtual int StartPushMediaFileStream() = 0;
    /*
    *   接口说明：停止插播文件推流
    *   回调检测：OnStopPushMediaFileStream
    */
    virtual void StopPushMediaFileStream() = 0;
    /*
    *   接口说明：插播文件暂停处理
    *   返回值：VhallLiveErrCode
    */
    virtual void MediaFilePause() = 0;
    /*
    *   插播文件恢复处理
    *   返回值：VhallLiveErrCode
    */
    virtual void MediaFileResume() = 0;
    /*
    *   接口说明：插播文件快进处理
    *   返回值：VhallLiveErrCode
    */
    virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) = 0;
    /*
    *   接口说明：插播文件总时长
    */
    virtual const long long MediaFileGetMaxDuration() = 0;
    /*  
    *   接口说明：插播文件当前时长
    */
    virtual const long long MediaFileGetCurrentDuration() = 0;
    /*
    *   接口说明：插播文件的当前状态 
    *   返回值：播放状态  MEDIA_FILE_PLAY_STATE
    */
    virtual int MediaGetPlayerState() = 0;
};

VHPAASSDK_EXPORT VHPaasInteractionRoom* GetPaasSDKInstance();
VHPAASSDK_EXPORT void DestoryPaasSDKInstance();

}