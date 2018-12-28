#include "VLiveCustomEvent.h"
#include "CustomEventType.h"
#include "../Unility/Unility.h"

VLiveCustomEvent::VLiveCustomEvent(QObject *parent)
    : QObject(parent)
{
}

VLiveCustomEvent::~VLiveCustomEvent()
{
}


void VLiveCustomEvent::customEvent(QEvent *event) {
    if (event == nullptr) {
        return;
    }
    int type = event->type();
    switch (type)
    {
    case CustomEvent_Login: {
        CBaseWnd* wndLogin = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
        if (wndLogin) {
            wndLogin->hide();
        }
        CBaseWnd* wndWebRtc = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_COTROL);
        if (wndWebRtc) {
            CSTDMapParam param;
            param[START_WEB_RTC_LIVE] = PARAM_TRUE;
            param[WEBRTC_ROOM_HOST] = PARAM_TRUE;
            wndWebRtc->Notice(param);
        }
        break;
    }
    case CustomEvent_LoginErr: {
        CBaseWnd* wndLogin = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
        if (wndLogin) {
            CSTDMapParam param;
            QEventRoomEvent* roomEvent = (QEventRoomEvent*)event;
            param["LoginResult"] = QString::fromStdString(roomEvent->mRespMsg);
            wndLogin->Notice(param);
        }
        break;
    }
    case CustomEvent_ShowLogin: {
        CBaseWnd* wndLogin = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
        if (wndLogin) {
            wndLogin->show();
        }
        break;
    }
    case CustomEvent_DestoryWnd: {
        HandleDestoryWnd(event);
        break;
    }
    case CustomEvent_WebRtcRoomConnetSuc:
    case CustomEvent_WebRtcRoomConnetErr:
    case CustomEvent_WebRtcRoomReConneting:
    case CustomEvent_WebRtcRoomReConneted:
    case CustomEvent_WebRtcRoomRecover:
    case CustomEvent_WebRtcRoomNetWorkChanged:
    case CustomEvent_WebRtcRoomNetWorkChangedCover:
    case CustomEvent_WebRtcRoomMixedReady:
    case CustomEvent_DevChanged: 
    case CustomEvent_GetVHMemberList:
    case CustomEvent_GetVHKickOutMemberList:
    case CustomEvent_OnSuccessRoomEvent:
    case CustomEvent_OnErrorRoomEvent:
    case CustomEvent_OnRecvApplyInavPublishMsg:
    case CustomEvent_OnRecvAuditInavPublishMsg:
    case CustomEvent_OnRecvAskforInavPublishMsg:
    case CustomEvent_OnRecvKickInavStreamMsg:
    case CustomEvent_OnRecvKickInavMsg:
    case CustomEvent_OnUserPublishCallback:
    case CustomEvent_User_OnLine:
    case CustomEvent_OnRecvInavCloseMsg:{
        CBaseWnd* wndWebRtc = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
        if (wndWebRtc) {
            wndWebRtc->Notice(event);
        }
        break;
    }
    case CustomEvent_KICKOUT_ROOM: {
        CBaseWnd* loginWnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
        if (loginWnd) {
            loginWnd->show();
        }
        ToolManager::GetInstance()->GetWndManager()->DestoryWnd(WND_ID_WEBRTC_MAIN);
        break;
    }
    default:
        break;
    }
}


void VLiveCustomEvent::HandleDestoryWnd(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventDestoryWnd* destoryEvent = dynamic_cast<QEventDestoryWnd*>(event);
    if (destoryEvent) {
        ToolManager::GetInstance()->GetWndManager()->DestoryWnd(destoryEvent->mWndId);
    }
}