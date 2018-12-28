#include "VhallWebRtcLive.h"
#include "WebRtcLiveWdg.h"
#include "WebRtcLiveControl.h"
//#include "CScreenShareToolDlg.h"
#include "VhallIALiveSettingDlg.h"
#include "InviteJoinWdg\InviteJoinWidget.h"
#include "InviteJoinWdg\ApplyJoinWdg.h"

ToolManager *globalToolManager = nullptr;

VhallWebRtcLive::VhallWebRtcLive()
{
}


bool RegisterGlobalManager(void* reg) {
    if (reg == nullptr) {
        return false;
    }
    globalToolManager = (ToolManager*)reg;
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_MAIN, REGISTER_WND(WebRtcLiveWdg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_COTROL, REGISTER_WND(WebRtcLiveControl));
	//globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_MAIN, REGISTER_WND(CScreenShareToolDlg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_SETTING, REGISTER_WND(VhallIALiveSettingDlg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_INVITE_TO_SPEAK, REGISTER_WND(InviteJoinWidget));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_ASK_FOR_SPEAK, REGISTER_WND(ApplyJoinWdg));

    return true;
}
