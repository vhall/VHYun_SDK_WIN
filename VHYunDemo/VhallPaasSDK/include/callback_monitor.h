#pragma once
#include "LiveEventInterface.h"
#include "paas_common.h"

//接收到的房间内广播的消息通知
class VHRoomRecvInavMsgCallback {
public:
    /*
    *   收到申请上麦消息 ,消息为广播消息，收到消息后，通过进入互动房间时获取的权限列表，判断用户是否有审核权限
    */
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id) = 0;
    /*
    *   收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作
    */
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish) = 0;
    /*
    *   邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息
    */
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   踢出流消息  消息为广播消息，收到消息后，执行踢出流
    */
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   踢出互动房间 , 消息为广播消息，收到消息后，判断是当前用户后，执行踢出房间操作
    */
    virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   上/下/拒绝上麦消息 消息为广播消息
    */
    virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event) = 0;
    /*
    *   互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间
    */
    virtual void OnRecvInavCloseMsg() = 0;
    /*
    *   用户上下线通知
    *   online： true 用户上线/ false 用户下线
    *   user_id： 用户id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id) = 0;
};

/*
**===================================
**
**   互动SDK事件监听回调接口，所有接口回调处于SDK业务线程。
**   如果回调处理app业务事件不建议在回调中进行。
**
**===================================
*/
class VHRoomMonitor :public VHRoomRecvInavMsgCallback, public WebRtcSDKEventInterface {
public:
    /*
    *   监听互动房间内HTTP业务API调用成功事件
    *   code: RoomEvent 事件类型  
    *   userData: 附带的json数据
    */
    virtual void OnRoomSuccessedEvent(RoomEvent code, std::string userData = std::string()) = 0;
    /*
    *   监听互动房间内HTTP业务API调用失败事件
    *   respCode: 错误码
    *   msg：错误信息
    *   userData: 附带的json数据
    */
    virtual void OnRoomFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string()) = 0;
    /*
    *   获取互动房间成员列表回调
    **/
    virtual void OnGetVHRoomMembers(const std::string& third_party_user_id, std::list<VHRoomMember>&) = 0;
    /*
    *   获取被踢出互动房间人列表回调
    **/
    virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&) = 0;

};

