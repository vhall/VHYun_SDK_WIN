
#include "WebRtcLiveWdg.h"
#include "WebRtcLiveControl.h"
#include "VhallIALiveSettingDlg.h"
#include "CScreenShareToolWgd.h"
#include "VedioPlayUI/VedioPlayUI.h"
#include "VedioPlayUI/VideoChoiceUI.h"
#include "vhfadeouttipwidget.h"
#include "VhallWebRtcLive.h"
#include "../Unility/CustomEventType.h"
#include "VHRoomMemberWdg.h"
#include "./RenderWdg/VhallRenderWdg.h"
#include "../Unility/Unility.h"
#include "./InviteJoinWdg/ApplyJoinWdg.h"
#include "./InviteJoinWdg/InviteJoinWidget.h"
#include "LiveEventInterface.h"
#include "AudioSliderUI.h"
#include "paas_common.h"
#include "ByPassRoom.h"

#include <QDateTime>
#include <QApplication> 
#include <QDesktopWidget> 
#include <QScreen> 
#include <QFileDialog> 
#include <QScrollBar> 

extern ToolManager *globalToolManager;

#define PLAYUI_PLAYSTATE
#define PLAYUI_PLAYSTATE_STOPPING  -2
#define PLAYUI_PLAYSTATE_FAIL      -1
#define PLAYUI_PLAYSTATE_NOTHING   0
#define PLAYUI_PLAYSTATE_OPENING   1
#define PLAYUI_PLAYSTATE_BUFFERING 2
#define PLAYUI_PLAYSTATE_PALYERING 3
#define PLAYUI_PLAYSTATE_PAUSED    4
#define PLAYUI_PLAYSTATE_STOP      5
#define PLAYUI_PLAYSTATE_END       6
#define PLAYUI_PLAYSTATE_ERROR     7

#define ENABLESPEAKTIME 5000

/*主界面*/
CREATE_WND_FUNCTION(WebRtcLiveWdg);
WebRtcLiveWdg::WebRtcLiveWdg(QWidget *parent) :
	CBaseWnd(parent),
	mIsFullSize(false),
	m_iPublishInavAnother(false)
{
	ui.setupUi(this);

	connect(ui.widget_title, &VhallLiveTitle::sig_ClickedClose, this, &WebRtcLiveWdg::slot_OnClose);
	connect(ui.widget_title, &VhallLiveTitle::sig_ClickedFresh, this, &WebRtcLiveWdg::slot_OnFresh);
	connect(ui.widget_title, &VhallLiveTitle::sig_ClickedFullSize, this, &WebRtcLiveWdg::slot_OnFullSize);
	connect(ui.widget_title, &VhallLiveTitle::sig_ClickedMinSize, this, &WebRtcLiveWdg::slot_OnMinSize);
	connect(ui.widget_title, &VhallLiveTitle::sig_ClickedSetting, this, &WebRtcLiveWdg::slot_OnSetting);
    connect(ui.widget_title, &VhallLiveTitle::sig_ClickedShare, this, &WebRtcLiveWdg::slot_OnShare);
	connect(ui.widget_title, &VhallLiveTitle::signal_move, this, &WebRtcLiveWdg::slot_move);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::sigMemberListClicked, this, &WebRtcLiveWdg::slot_showMemberList);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnClickedCameraCtrlBtn, this, &WebRtcLiveWdg::slot_CameraBtnClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnClickedMicCtrlBtn, this, &WebRtcLiveWdg::slot_MicButtonClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnClickedPlayerCtrlBtn, this, &WebRtcLiveWdg::slot_PlayerButtonClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnClickedScreenShareCtrlBtn, this, &WebRtcLiveWdg::slot_BtnScreenClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnClickedStartLiveCtrlBtn, this, &WebRtcLiveWdg::slot_BtnStreamClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnMicBtnEnter, this, &WebRtcLiveWdg::slot_MicButtonEnter);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnMicBtnLeave, this, &WebRtcLiveWdg::slot_MicButtonLeave);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnPlayerBtnEnter, this, &WebRtcLiveWdg::slot_PlayerButtonEnter);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::Sig_OnPlayerBtnLeave, this, &WebRtcLiveWdg::slot_PlayerButtonLeave);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::sig_OnPlayFileClicked, this, &WebRtcLiveWdg::slot_OnPlayFileClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::sigToSpeakClicked, this, &WebRtcLiveWdg::slot_OnToSpeakClicked);
	connect(ui.widget_ctrl, &CInterActiveCtrlWdg::sigPublishInavAnotherClicked, this, &WebRtcLiveWdg::slot_PublishInavAnotherClicked);
	connect(ui.pushButton_onLine, SIGNAL(clicked()), this, SLOT(slot_OnTurnToOnLinePage()));
	connect(ui.pushButton_kickout, SIGNAL(clicked()), this, SLOT(slot_OnTurnToKickOutPage()));

    connect(&mRePublishLocalStreamTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_RePublishLocalStreamTimeout);
    connect(&mRePublishDesktopStreamTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_RePublishDesktopStreamTimeout);
    connect(&mRePublishMediaFileStreamTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_RePublishMediaFileStreamTimeout);
    connect(&mReSetMediaFileMainViewTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_ReSetMediaFileMainViewTimer);
    connect(&mReSetDesktopMainViewTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_ReSetDesktopMainViewTimer);

	Permission per = globalToolManager->GetPaasSDK()->GetPermission();
	ui.widget_ctrl->InitBtnState(per.publish_inav_stream);

	ui.listWidget_speaking->hide();
	ui.widget_3->hide();
	ui.widget_ctrl->SetHideDevList();


	mpFadeOutTipWidget = new VHFadeOutTipWidget(this);
	if (mpFadeOutTipWidget) {
		mpFadeOutTipWidget->setParent(this);
		mpFadeOutTipWidget->setWindowFlags(Qt::FramelessWindowHint);
		mpFadeOutTipWidget->hide();
		int width = ui.widget_ctrl->width()/*-ui.widget->width()*/;
		mpFadeOutTipWidget->setFixedWidth(width);
	}

	//麦克风音量控制
	mpMicVolumn = new AudioSliderUI((QDialog *)this);
	mpMicVolumn->Create();
	mpMicVolumn->HideMuteBtn();
	mpMicVolumn->HideSettingBtn();
	connect(mpMicVolumn, &AudioSliderUI::sigVolumnChanged, this, &WebRtcLiveWdg::slot_MicVolumnChanged);
	//扬声器音量控制
	mpPlayerVolumn = new AudioSliderUI((QDialog *)this);
	mpPlayerVolumn->Create();
	mpPlayerVolumn->HideMuteBtn();
	mpPlayerVolumn->HideSettingBtn();
	connect(mpPlayerVolumn, &AudioSliderUI::sigVolumnChanged, this, &WebRtcLiveWdg::slot_PlayerVolumnChanged);

	//Permission per = globalToolManager->GetPaasSDK()->GetPermission();
	//ui.widget_ctrl->initToSpeakBtnState(per.apply_inav_publish);//申请上麦
	ui.widget_ctrl->ShowPublishInavAnother(per.publish_inav_another);//旁路直播
	ui.widget_ctrl->intShowStartLive(per.publish_inav_stream);//推流 /上麦
	ui.widget_ctrl->initToSpeakBtnState(per.apply_inav_publish);//申请上麦

    ui.widget_mainView->ShowSetKickOutBtn(false);
    ui.widget_mainView->ShowSetSpeakBtn(false);
    ui.widget_mainView->SetViewState(RenderView_NoJoin);
	ui.widget_mainView->hide();
    ui.widget_topRenderView->hide();
	ui.DesktopWdg->hide();
	ui.MediaFilePlayWdg->hide();
	ui.bgWdg->show();
	
    connect(ui.widget_mainView, &VhallRenderWdg::sig_setToSpeaker, this, &WebRtcLiveWdg::slot_setToSpeaker);
    connect(ui.widget_mainView, &VhallRenderWdg::sig_ClickedCamera, this, &WebRtcLiveWdg::slot_OnClickedCamera);
    connect(ui.widget_mainView, &VhallRenderWdg::sig_ClickedMic, this, &WebRtcLiveWdg::slot_ClickedMic);
    connect(ui.widget_mainView, &VhallRenderWdg::sig_setInMainView, this, &WebRtcLiveWdg::slot_SetMainWdgRender);

	//globalToolManager->GetDataManager()->SetPushStreamPermission(per.publish_inav_stream);

	CreateScreenShareWgd();
	CreateSettingDlg(false);
	if (NULL != mpSettingDlg)
	{
		mpSettingDlg->setPushPermission(per.publish_inav_stream);
		mpSettingDlg->Apply();
	}

}

WebRtcLiveWdg::~WebRtcLiveWdg()
{
    globalToolManager->GetDataManager()->WriteLog("Enter %s", __FUNCTION__);

    if (NULL != mpMicVolumn)
    {
        delete mpMicVolumn;
        mpMicVolumn = NULL;
    }

    if (NULL != mpPlayerVolumn)
    {
        delete mpPlayerVolumn;
        mpPlayerVolumn = NULL;
    }

    if (mpSettingDlg) {
        delete mpSettingDlg;
        mpSettingDlg = NULL;
    }
    globalToolManager->GetDataManager()->WriteLog("Leave %s", __FUNCTION__);
    globalToolManager->RemoveQEventReciver(this);
}


void WebRtcLiveWdg::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find(START_WEB_RTC_LIVE);
    if (iter != mapParam.end()) {
        globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_LOGIN));
        globalToolManager->GetDataManager()->WriteLog("JoinVHMediaRoom %s", __FUNCTION__);
        globalToolManager->GetPaasSDK()->JoinVHMediaRoom();
        ui.widget_title->SetTitleType(eVhallIALiveHost);
        ui.widget_ctrl->InitBtnState(true);
    }
}

void WebRtcLiveWdg::Notice(const CSTDMapParamPtr &mapParam) {

}

void WebRtcLiveWdg::OnDevChanged(const QString &cameraDevId, const int cameraIndex, const QString &micDevId, const int micIndex, const QString &playerDevId, const int playerIndex, const int iLayoutMode) {
    if (miLayoutMod != iLayoutMode) {
        QCoreApplication::postEvent(this, new CustomDevChangedEvent(CustomEvent_CHANGE_LAYOUT, "", 0, "", 0, "", 0));
    }
    miLayoutMod = iLayoutMode;
	QCoreApplication::postEvent(this, new CustomDevChangedEvent(CustomEvent_DevChanged, cameraDevId, cameraIndex, micDevId, micIndex, playerDevId, playerIndex));
}

void WebRtcLiveWdg::CreateSettingDlg(const bool& bShow /*= true*/)
{
    if (NULL == mpSettingDlg) {
        bool isHost = true;//是否是主持or嘉宾
        mpSettingDlg = new VhallIALiveSettingDlg(NULL, isHost);
        if (mpSettingDlg) {
            mpSettingDlg->RegisterListener(this);
        }
    }

    if (NULL != mpSettingDlg)
    {
        mpSettingDlg->init();
        if (bShow)
        {
            mpSettingDlg->exec();
        }
    }

}

void WebRtcLiveWdg::Notice(QEvent* event) {
    customEvent(event);
}

void WebRtcLiveWdg::HandleStopPushLocalStreamSuc(QEvent* event)
{
	QEventStream* eventMsg = (QEventStream*)event;
	if (eventMsg && mbStopLive/*eventMsg->mStreamType< VHStreamType_Desktop*/)
	{
		globalToolManager->GetPaasSDK()->UserPublishCallback(PushStreamEvent::PushStreamEvent_Lower, mStrStreamId[eventMsg->mStreamType].toStdString());
        globalToolManager->GetDataManager()->SetIsLiving(false);
	}
	mbStopLive = false;
    globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
    
	
	VHRoomMemberWdg *widget = GetRoomMemberFromOnLineList( globalToolManager->GetDataManager()->GetThridPartyUserId() );
	if (nullptr != widget && mbStopLive)
	{
		widget->SetUserStatus(MemberStatus_Watching);
	}

	
	//ui.widget_ctrl->OnStartLive(false);
	if (m_pScreenShareToolWgd)
	{
		m_pScreenShareToolWgd->SetStartLive(false);
		m_pScreenShareToolWgd->EnableStartLive(true);
	}
		
	ui.widget_ctrl->EnableStartLive(true);
}

void WebRtcLiveWdg::HandleRecvApplyInavPublishMsg(QEvent* event)
{
    globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
    if (event == nullptr) {
        return;
    }
    QEventVHMember* member = (QEventVHMember*)event;
    if (member->thirdPartyUserId != globalToolManager->GetDataManager()->GetThridPartyUserId()) {
    	Permission per = globalToolManager->GetPaasSDK()->GetPermission();
        if(per.audit_inav_publish)//审核//审核邀请上麦上麦
        {
            QEventVHMember* eventMsg = (QEventVHMember*)event;
            //有可能多个用户同时申请上麦，此时根据用户ID创建对应的显示窗口。
            CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ASK_FOR_SPEAK,eventMsg->thirdPartyUserId);
            if (wnd) {
                CSTDMapParam map;
                map[USER_ID] = eventMsg->thirdPartyUserId;
                wnd->Notice(map);
                wnd->show();
            }
        }
    }

}

void WebRtcLiveWdg::HandleRecvAuditInavPublishMsg(QEvent* event)
{
    QEventVHMember* eventMsg = (QEventVHMember*)event;
    if (globalToolManager->GetDataManager()->GetThridPartyUserId().compare(eventMsg->thirdPartyUserId) == 0) {
        if (AuditPublish::AuditPublish_Accept == eventMsg->memberStatusType)//开始推流
        {
            globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Accept", __FUNCTION__);
            //globalToolManager->GetPaasSDK()->StartPushLocalStream();
			int iStat = 0;
			StartPushStream(iStat);

            QString tips = QStringLiteral("上麦申请已同意");
            FadeOutTip(tips);
			ui.widget_ctrl->OnStartLive(true);
			ui.widget_ctrl->SetToSpeakBtnState(true);
            globalToolManager->GetDataManager()->SetIsLiving(true);
        }
        else//拒绝上麦
        { 
            globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Refused", __FUNCTION__);
            QString tips = QStringLiteral("您的上麦申请被拒绝");
            FadeOutTip(tips);
			ui.widget_ctrl->OnStartLive(false);
			ui.widget_ctrl->SetToSpeakBtnState(false);
        }
    }
}

void WebRtcLiveWdg::RemoveMediaFileStreamMsg(const QString& uid)
{
    globalToolManager->GetDataManager()->WriteLog("%s uid:%s \n", __FUNCTION__, uid.toStdString().c_str());
    QMutexLocker l(&mStreamMutexCS);
    StopShowMediaVideo();
}

void WebRtcLiveWdg::RemoveDesktopStreamMsg(const QString& uid)
{
    globalToolManager->GetDataManager()->WriteLog("%s uid:%s \n", __FUNCTION__, uid.toStdString().c_str());
    QMutexLocker l(&mStreamMutexCS);
	ui.DesktopWdg->hide();
	SetPlayDeskTopAndMediaFileState(false);
}

void WebRtcLiveWdg::RemoveLocalStreamMsg(const QString& uid) {
    globalToolManager->GetDataManager()->WriteLog("%s uid:%s \n", __FUNCTION__, uid.toStdString().c_str());
    QMutexLocker l(&mStreamMutexCS);
	bool bValue = false;
    if (uid.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0)
    {
		mRenderMap.remove(uid);

        int iCount = ui.listWidget_topView->count();
        globalToolManager->GetDataManager()->WriteLog("%s  iCount：%d reMoveInfoId:%s\n", __FUNCTION__, iCount, uid.toLatin1().data());
        for (int i = iCount - 1; i >= 0; i--) {
            QListWidgetItem *item = ui.listWidget_topView->item(i);
            if (NULL != item) {
                QWidget *w = ui.listWidget_topView->itemWidget(item);
                if (NULL != w) {
                    VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);

                    if (itemWidget->isCurrentUser(uid)) {
                        ui.listWidget_topView->takeItem(i);
                        itemWidget->deleteLater();
                        itemWidget = NULL;
                        delete item;
                        item = NULL;
						bValue = true;
                        globalToolManager->GetDataManager()->WriteLog("%s  RemoveInfo SUC  id：%s  \n", __FUNCTION__, uid.toLocal8Bit().data());
						break;
                    }
                }
                else {
                    globalToolManager->GetDataManager()->WriteLog("%s  Get w FAIL \n", __FUNCTION__);
                }
            }
            else {
                globalToolManager->GetDataManager()->WriteLog("%s  Get Item FAIL \n", __FUNCTION__);
            }
        }

        if (uid.compare(mCurMainShowUid) == 0 && !mRenderMap.isEmpty())
		{

			QStringList strIdList = mRenderMap.keys();
			QString curPushingStreamUser = strIdList.at(0);

            if (!curPushingStreamUser.isEmpty()) {
                if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop)
					|| globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile)
					|| globalToolManager->GetPaasSDK()->IsUserPushingDesktopStream()
					|| globalToolManager->GetPaasSDK()->IsUserPushingMediaFileStream()
					) {

                    mCurMainShowUid = curPushingStreamUser;
                }
                else{
                    ui.widget_topRenderView->hide();
                    SetMainWdgRenderCameraUser(curPushingStreamUser);
                }
            }
        }
		else if(bValue && 2==iCount && mCurMainShowUid.compare(globalToolManager->GetDataManager()->GetThridPartyUserId())==0 &&
			!globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile))
		{
			ui.widget_topRenderView->hide();
		}
    }

}

void WebRtcLiveWdg::HandleUserOnLine(QEvent* event) {
    QEventVHMember* member = (QEventVHMember*)event;
    if (member) {
        if (member->memberStatusType == 1) {
            //上线
            AddMemeberIntoOnLineList(member->thirdPartyUserId, MemberStatus_Watching);
        }
        else {
            //下线
            RemoveMemberFromOnLineUser(member->thirdPartyUserId);
        }
    }
}

void WebRtcLiveWdg::HandleStreamRemoved(QEvent* event)
{
    globalToolManager->GetDataManager()->WriteLog("%s\n", __FUNCTION__);

    if (event == NULL) {
        return;
    }
    QEventStream* eventMsg = (QEventStream*)event;
    if (eventMsg)
    {
        globalToolManager->GetDataManager()->WriteLog("%s user stream remove uid:%s\n", __FUNCTION__, eventMsg->strUser.toStdString().c_str());

        if (eventMsg->mStreamType <= VHStreamType_AVCapture)
        {
            RemoveLocalStreamMsg(eventMsg->strUser);
        }
        else if (eventMsg->mStreamType == VHStreamType_MediaFile 
			&& globalToolManager->GetDataManager()->GetThridPartyUserId().compare(eventMsg->strUser)!=0  )//且不是本人的
        {
            RemoveMediaFileStreamMsg(eventMsg->strUser);
        }
        else if (eventMsg->mStreamType == VHStreamType_Desktop)
        {
            RemoveDesktopStreamMsg(eventMsg->strUser);
        }

    }
    globalToolManager->GetDataManager()->WriteLog("%s end\n", __FUNCTION__);
}

void WebRtcLiveWdg::HandlePublishInavAnother(QEvent* event)
{
    m_iPublishInavAnother = true;
    ui.widget_ctrl->setPubLishInavAnotherState(m_iPublishInavAnother);
}

void WebRtcLiveWdg::HandlePublishInavAnotherError(QEvent*event)
{
    if (event == nullptr) {
        return;
    }
    QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
    if (roomEvent) {
        QString msg = QString::fromStdString(roomEvent->mRespMsg);
        FadeOutTip(msg);
    }
	ui.widget_ctrl->setPubLishInavAnotherState(m_iPublishInavAnother);
}

//关闭旁路直播成功
void WebRtcLiveWdg::HandleStopPublishInavAnother(QEvent* event)
{
    m_iPublishInavAnother = false;
    ui.widget_ctrl->setPubLishInavAnotherState(m_iPublishInavAnother);
}

//关闭旁路直播失败
void WebRtcLiveWdg::HandleStopPublishInavAnotherError(QEvent*event)
{
	if (event == nullptr) {
		return;
	}
	QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
	if (roomEvent) {
		QString msg = QString::fromStdString(roomEvent->mRespMsg);
		FadeOutTip(msg);
	}
	ui.widget_ctrl->setPubLishInavAnotherState(m_iPublishInavAnother);
}

void WebRtcLiveWdg::HandleLogout(QEvent* roomEvent)
{
	if (m_bClose)
	{
		m_bClose = false;
		close();
		if (globalToolManager)
			globalToolManager->AppQuit();
	}
}

void WebRtcLiveWdg::HandleRecvAskForInav(QEvent* event) {
    globalToolManager->GetDataManager()->WriteLog("%s HandleRecvAskForInav", __FUNCTION__);
    QEventVHMember* eventMsg = (QEventVHMember*)event;
    //如果自己收到了邀请上麦消息则弹窗提示框。
    if (globalToolManager->GetDataManager()->GetThridPartyUserId().compare(eventMsg->thirdPartyUserId) == 0) {
        QEventVHMember* eventMsg = (QEventVHMember*)event;
        //有可能主持人同时邀请，此时根据用户ID创建对应的显示窗口。
        CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_INVITE_TO_SPEAK, eventMsg->thirdPartyUserId);
        if (wnd) {
            CSTDMapParam map;
            map[USER_ID] = eventMsg->thirdPartyUserId;
            wnd->Notice(map);
            wnd->show();
        }
    }
    VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(eventMsg->thirdPartyUserId);
    if (member) {
        member->SetUserStatus(MemberStatus_BeInvited);
    }
}

void WebRtcLiveWdg::HandlePushLocalStreamSuc(QEvent* event) {
    globalToolManager->GetDataManager()->SetIsLiving(true);
    ui.widget_ctrl->OnStartLive(true);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetStartLive(true);
    globalToolManager->GetDataManager()->WriteLog("%s\n", __FUNCTION__);
    if (event == NULL) {
        return;
    }

    QEventStream* eventMsg = (QEventStream*)event;
    if (eventMsg)
    {
        mLocalStreamId = QString::fromStdString(eventMsg->strStream);
        globalToolManager->GetDataManager()->WriteLog("%s  eventMsg->mStreamType:%d eventMsg->strStream:%s\n", __FUNCTION__, eventMsg->mStreamType, eventMsg->strStream.c_str());
        mStrStreamId[eventMsg->mStreamType] = QString::fromStdString(eventMsg->strStream);

        VhallRenderWdg *render = GetRenderWnd(QString::fromStdString(eventMsg->strStream));
        if (render) {
            if (VHStreamType_AVCapture == eventMsg->mStreamType) {
                render->SetViewState(RenderView_None);
            }
        }
        if ((eventMsg->mStreamType == VHStreamType_MediaFile || eventMsg->mStreamType == VHStreamType_Desktop) 
			&& globalToolManager->GetDataManager()->GetIsLiving()) {
            mPublishMainViewStreamId = QString::fromStdString(eventMsg->strStream);
        }
		else if (!globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop)
			&& !globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile))
		{
			mPublishMainViewStreamId = QString::fromStdString(eventMsg->strStream);
		}

		if (eventMsg->mStreamType == VHStreamType_MediaFile)
		{
            if (globalToolManager->GetPaasSDK()->IsSupportMediaFormat(CaptureStreamAVType_Video)) {
                ui.MediaFilePlayWdg->show();
            }
            else {
                ui.MediaFilePlayWdg->hide();
            }
		}
        globalToolManager->GetPaasSDK()->UserPublishCallback(PushStreamEvent::PushStreamEvent_Upper, mStrStreamId[VHStreamType_AVCapture].toStdString());

    }
    //slot_showMemberList();

	VHRoomMemberWdg *widget = GetRoomMemberFromOnLineList(globalToolManager->GetDataManager()->GetThridPartyUserId());
	if (nullptr != widget)
	{
		widget->SetUserStatus(MemberStatus_Pushing);
	}
}

void WebRtcLiveWdg::HandlePushLocalStreamErro(QEvent* event)
{
	QEventStreamError* eventMsg = (QEventStreamError*)event;
	if (eventMsg)
	{
		if (0 == eventMsg->iCodeErr)
		{
			FadeOutTip(QStringLiteral("您当前没有上麦权限"));
			ui.widget_ctrl->SetToSpeakBtnState(false);
		}
		else
			FadeOutTip(QString::fromStdString(eventMsg->strMsg) );
	}

}

void WebRtcLiveWdg::customEvent(QEvent *event) {
    if (event == nullptr) {
        return;
    }
    int type = event->type();
    globalToolManager->GetDataManager()->WriteLog("Enter %s type:%d", __FUNCTION__, type);
    switch (type) {
    case CustomEvent_GetLocalStreamSuc:
        HandleGetStream(event);
        break;
    case CustomEvent_PushLocalStreamSuc:
        HandlePushLocalStreamSuc(event);
        break;
	case CustomEvent_PushLocalStreamError:
		HandlePushLocalStreamErro(event);
		break;
    case CustomEvent_StopPushLocalStreamSuc:
        HandleStopPushLocalStreamSuc(event);
        break;
	case CustomEvent_StopPushLocalStreamErr:
		ui.widget_ctrl->EnableStartLive(true);
		if (m_pScreenShareToolWgd)
		{
			m_pScreenShareToolWgd->EnableStartLive(true);
		}
		break;
    case CustomEvent_WebRtcRoomConnetSuc:
        HandleWebRtcRoomConnetSuc();
        break;
    case CustomEvent_WebRtcRoomConnetErr:
        HandleWebRtcRoomConnetErr();
        break;
    case CustomEvent_WebRtcRoomReConneting:
        HandleWebRtcRoomReConneting();
        break;
    case CustomEvent_WebRtcRoomReConneted:
        HandleWebRtcRoomReConneted();
        break;
    case CustomEvent_WebRtcRoomRecover:
        HandleWebRtcRoomRecover();
        break;
    case CustomEvent_WebRtcRoomNetWorkChanged:
        HandleWebRtcRoomNetWorkChanged();
        break;
    case CustomEvent_WebRtcRoomNetWorkChangedCover:
        HandleWebRtcRoomNetWorkChangedCover();
        break;
    case CustomEvent_DevChanged: {
        HandleDeviceEvent(event);
        break;
    }
    case CustomEvent_WebRtcRoomMixedReady: {
        //接收到此消息后方可设置布局与大画面
		Permission per = globalToolManager->GetPaasSDK()->GetPermission();

		if (per.publish_inav_another)//旁路直播
		{
			int nRet = globalToolManager->GetPaasSDK()->SetMainView(mPublishMainViewStreamId.toStdString());
			if (nRet == VhallLive_SERVER_NOT_READY) {
				FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
			}
		}
        break;
    }
    case CustomEvent_CHANGE_LAYOUT: {
        QString live_room_id = globalToolManager->GetDataManager()->GetLiveRoomId();

		Permission per = globalToolManager->GetPaasSDK()->GetPermission();
		if (per.publish_inav_another)//旁路直播
		{
			int nRet = globalToolManager->GetPaasSDK()->StartPublishInavAnother(live_room_id.toStdString().c_str(), (LayoutMode)miLayoutMod);
			if (nRet == VhallLive_SERVER_NOT_READY) {
				FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
                ui.widget_ctrl->setPubLishInavAnotherState(false);
			}
            else if (nRet != VhallLive_OK) {
                FadeOutTip(QStringLiteral("设置混流失败，请稍后再试"));
                ui.widget_ctrl->setPubLishInavAnotherState(false);
            }
		}

        break;
    }
    case CustomEvent_OnRecvApplyInavPublishMsg: {
        //收到申请上麦消息
        HandleRecvApplyInavPublishMsg(event);
        break;
    }
    case CustomEvent_OnRecvAuditInavPublishMsg: {
        //收到审核上麦消息
        HandleRecvAuditInavPublishMsg(event);
        break;
    }
    case CustomEvent_OnRecvAskforInavPublishMsg:
        //收到邀请上麦消息
        HandleRecvAskForInav(event);
        break;
    case CustomEvent_OnRecvKickInavStreamMsg: {
		ui.widget_ctrl->EnableStartLive(true);
		if (m_pScreenShareToolWgd)
		{
			m_pScreenShareToolWgd->EnableStartLive(true);
		}
		//m_SelfStopPush = false;
		QEventPublishStream* msgEvent = (QEventPublishStream*)event;
		if (0 == msgEvent->strStream.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) 
			/*&& !m_SelfStopPush*/)//被动下麦
		{
			ui.widget_ctrl->OnStartLive(false);

			Permission per = globalToolManager->GetPaasSDK()->GetPermission();
			ui.widget_ctrl->intShowStartLive(per.publish_inav_stream);//推流 /上麦
			ui.widget_ctrl->SetToSpeakBtnState(!per.apply_inav_publish);//申请上麦

			HandleStopLiving();

			VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(globalToolManager->GetDataManager()->GetThridPartyUserId());
			if (member) {
					member->SetUserStatus(MemberStatus_Watching);
			}

		}
	}
        break;
    case CustomEvent_OnRecvKickInavMsg: {
        HandleRecvKickInvaMsg(event);
        break;
    }
    case CustomEvent_OnUserPublishCallback: {
        HandleUserPublishCallback(event);
        break;
    }
    case CustomEvent_GetVHMemberList: {
        HandleGetVHMemberList(event);
        break;
    }
    case CustomEvent_GetVHKickOutMemberList: {
        HandleGetKickOutMemberList(event);
        break;
    }
    case CustomEvent_OnErrorRoomEvent: {
        HandleErrorEventRoomEvent(event);
        break;
    }
    case CustomEvent_OnSuccessRoomEvent: {
        HandleSuccessEventRoomEvent(event);
        break;
    }
    case CustomEvent_SubScribSuc:
        HandleSubScribSucEvent(event);
        break;
    case CustomEvent_STREAM_REMOVED:
        HandleStreamRemoved(event);
        break;
    case CustomEvent_User_OnLine: {
        HandleUserOnLine(event);
        break;
    }
    default:
        break;
    }
    globalToolManager->GetDataManager()->WriteLog("Leave %s type:%d", __FUNCTION__, type);
}

void WebRtcLiveWdg::HandleInitVhallSDK() {
    ui.widget_ctrl->OnStartLive(true);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetStartLive(true);
}

void WebRtcLiveWdg::RecvRemoteMediaStream() {
    if (mbIsDeskTop)//桌面共享中
    {
        mbEnableStopDesktopStream = false;
        mbIsDeskTop = false;
        if (m_pScreenShareToolWgd) {
            m_pScreenShareToolWgd->hide();
        }
        show();
    }

    m_qCycleTimer.stop();
    //隐藏UI
    ShowVedioPlayWidget(FALSE);
    ForceHide(true);
    m_bPlayCurrentFile = true;
    StopShowMediaVideo();
}

void WebRtcLiveWdg::HandleWebRtcRoomConnetSuc() {
    slot_showMemberList();
}

void WebRtcLiveWdg::HandleWebRtcRoomConnetErr() {
    FadeOutTip(QStringLiteral("网络异常互动房间已断开"));
}

void WebRtcLiveWdg::HandleWebRtcRoomReConneting() {
    FadeOutTip(QStringLiteral("网络异常，正在进行重连..."));
}

void WebRtcLiveWdg::HandleWebRtcRoomReConneted() {
    FadeOutTip(QStringLiteral("网络异常已恢复"));
}

void WebRtcLiveWdg::HandleWebRtcRoomRecover() {
    FadeOutTip(QStringLiteral("网络异常已恢复"));
}

void WebRtcLiveWdg::HandleWebRtcRoomNetWorkChangedCover() {
    FadeOutTip(QStringLiteral("网络异常已恢复"));
}

void WebRtcLiveWdg::HandleWebRtcRoomNetWorkChanged() {
    FadeOutTip(QStringLiteral("网络异常，正在进行重连..."));
}

//信号与槽函数
void WebRtcLiveWdg::slot_joinroom() {
    if (globalToolManager->GetPaasSDK() == nullptr) {
        return;
    }
    globalToolManager->GetPaasSDK()->JoinVHMediaRoom();
}

void WebRtcLiveWdg::slot_exitroom() {
    if (globalToolManager->GetPaasSDK() == nullptr) {
        return;
    }

    if (globalToolManager->GetPaasSDK()) {
        globalToolManager->GetPaasSDK()->LeaveVHMediaRoom();
    }
}

void WebRtcLiveWdg::slot_showMemberList() {
	ClearAllOnLineUser();
	ClearAllKickOutUSer();
    globalToolManager->GetPaasSDK()->AsynGetVHRoomMembers();
    globalToolManager->GetPaasSDK()->GetKickInavList();
}

void WebRtcLiveWdg::slot_RePublishLocalStreamTimeout() {
	
    int nRet = VhallLive_OK/* = globalToolManager->GetPaasSDK()->StartPushLocalStream()*/;
	StartPushStream(nRet);
    if (nRet == VhallLive_ROOM_DISCONNECT) {
        //网络连接异常，启动定时器尝试重新推流。
        mRePublishLocalStreamTimer.start(3000);
        mRePublishLocalStreamTimer.setSingleShot(true);
    }
}

void WebRtcLiveWdg::slot_RePublishDesktopStreamTimeout() {
    int nRet = globalToolManager->GetPaasSDK()->StartPushDesktopStream();
    if (nRet == VhallLive_ROOM_DISCONNECT) {
        //网络连接异常，启动定时器尝试重新推流。
        mRePublishDesktopStreamTimer.start(3000);
        mRePublishDesktopStreamTimer.setSingleShot(true);
    }
}

void WebRtcLiveWdg::slot_RePublishMediaFileStreamTimeout() {
    int nRet = globalToolManager->GetPaasSDK()->StartPushMediaFileStream();
    if (nRet == VhallLive_ROOM_DISCONNECT) {
        //网络连接异常，启动定时器尝试重新推流。
        mRePublishMediaFileStreamTimer.start(3000);
        mRePublishMediaFileStreamTimer.setSingleShot(true);
    }
}

void WebRtcLiveWdg::slot_ReSetMediaFileMainViewTimer() {
    if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile) && globalToolManager->GetDataManager()->GetIsLiving()) {
        int nRet = globalToolManager->GetPaasSDK()->SetMainView(mPublishMainViewStreamId.toStdString());
        if (nRet == VhallLive_SERVER_NOT_READY) {
            FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
        }
    }

}

void WebRtcLiveWdg::slot_ReSetDesktopMainViewTimer() {
    if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop) && globalToolManager->GetDataManager()->GetIsLiving()) {
        int nRet = globalToolManager->GetPaasSDK()->SetMainView(mPublishMainViewStreamId.toStdString());
        if (nRet == VhallLive_SERVER_NOT_READY) {
            FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
        }
    }
}

void WebRtcLiveWdg::slot_CameraBtnClicked() {
    bool curState = globalToolManager->GetPaasSDK()->IsCameraOpen();
    VhallRenderWdg* render = GetRenderWnd(globalToolManager->GetDataManager()->GetThridPartyUserId());
    ui.widget_ctrl->OnOpenCamera(!curState);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetCameraState(!curState);
    if (curState)
        globalToolManager->GetPaasSDK()->CloseCamera();
    else
        globalToolManager->GetPaasSDK()->OpenCamera();

    if (render) {
        render->SetUserCameraState(!curState);
        SetMainViewCameraState(globalToolManager->GetDataManager()->GetThridPartyUserId(), !curState);
        if (!curState) {
            render->SetEnableUpdateRendWnd(false);
        }
        else {
            render->SetEnableUpdateRendWnd(true);
        }
    }
}

void WebRtcLiveWdg::openMic(bool bOpen)
{
	VhallRenderWdg* render = GetRenderWnd(globalToolManager->GetDataManager()->GetThridPartyUserId());
	if (render) {
		render->SetUserMicState(bOpen);
		ui.widget_mainView->SetUserMicState(bOpen);
		if (bOpen) {
			render->SetEnableUpdateRendWnd(false);
		}
		else {
			render->SetEnableUpdateRendWnd(true);
		}
	}

	ui.widget_ctrl->OnOpenMic(bOpen);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetMicState(bOpen);

	if (!bOpen) {
		globalToolManager->GetPaasSDK()->CloseMic();
	}
	else {
		globalToolManager->GetPaasSDK()->OpenMic();
		globalToolManager->GetPaasSDK()->SetCurrentMicVol(100);

		if (mpMicVolumn) {
			mpMicVolumn->StopClose();
			mpMicVolumn->SetVolumn(100);
		}
	}
}


void WebRtcLiveWdg::slot_MicButtonClicked() {
    bool curState = globalToolManager->GetPaasSDK()->IsMicOpen();
	openMic(!curState);
}

void WebRtcLiveWdg::slot_PlayerButtonClicked() {
    int  iPlayVol = globalToolManager->GetPaasSDK()->GetCurrentPlayVol();
    ui.widget_ctrl->OnOpenPlayer(iPlayVol <= 0);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetPlayerState(iPlayVol <= 0);

    if (iPlayVol <= 0) {
        globalToolManager->GetPaasSDK()->SetCurrentPlayVol(100);

        if (mpMicVolumn) {
            mpMicVolumn->StopClose();
            mpMicVolumn->SetVolumn(100);
        }
    }
    else {
        //globalToolManager->GetPaasSDK()->OpenMic();
        globalToolManager->GetPaasSDK()->SetCurrentPlayVol(0);
        if (mpMicVolumn) {
            mpMicVolumn->StopClose();
            mpMicVolumn->SetVolumn(0);
        }
    }
}

void WebRtcLiveWdg::slot_MicButtonEnter() {
    if (mCurMicDevID.isEmpty() || mpMicVolumn == NULL) {
        return;
    }

    mpMicVolumn->StopClose();
    mpMicVolumn->Show(globalToolManager->GetPaasSDK()->GetCurrentMicVol(), ui.widget_ctrl->GetMicBtnGlobalPos());
}

void WebRtcLiveWdg::slot_MicButtonLeave() {
    if (mpMicVolumn) {
        mpMicVolumn->WillClose(200);
    }
}

void WebRtcLiveWdg::slot_PlayerButtonEnter() {
    if (mCurPlayerID.isEmpty() || mpPlayerVolumn == NULL) {
        return;
    }

    mpPlayerVolumn->StopClose();
    mpPlayerVolumn->Show(globalToolManager->GetPaasSDK()->GetCurrentPlayVol(), ui.widget_ctrl->GetPlayBtnGlobalPos());
}

void WebRtcLiveWdg::slot_PlayerButtonLeave() {
    if (mpPlayerVolumn) {
        mpPlayerVolumn->WillClose(200);
    }
}

void WebRtcLiveWdg::slot_BtnStopScreenClicked()
{
	if (mbIsDeskTop)//桌面共享中
	{
		mbEnableStopDesktopStream = false;
        globalToolManager->GetPaasSDK()->StopDesktopCapture();
        mbIsDeskTop = false;
        if (m_pScreenShareToolWgd) {
            m_pScreenShareToolWgd->hide();
        }
        show();
	}
}

void WebRtcLiveWdg::slot_BtnScreenClicked() {
	if (globalToolManager->GetPaasSDK()->IsUserPushingDesktopStream())
	{
		FadeOutTip(QString::fromWCharArray(L"桌面共享进行中……"));
		return;
	}

	if (globalToolManager->GetPaasSDK()->IsUserPushingMediaFileStream())
	{
		FadeOutTip(QString::fromWCharArray(L"视频插播进行中……"));
		return;
	}

	Permission per = globalToolManager->GetPaasSDK()->GetPermission();
    if (!per.publish_inav_stream) {
        FadeOutTip(QString::fromWCharArray(L"无推流权限不能进行桌面共享"));
        return;
    }

    if (IsPlayMediaFile()) {
        CBaseWnd* wndLoading = globalToolManager->GetWndManager()->FindWnd(WND_ID_LOADING);
        if (wndLoading) {
            CSTDMapParam mapParam;
            mapParam["msg"] = ALREADY_PLAY_MEDIA_FILE;
            wndLoading->Notice(mapParam);
            wndLoading->exec();
        }
        return;
    }

    if (!IsEnalbePlayMedia()) {
        FadeOutTip(QString::fromWCharArray(L"您没有该权限"));
        return;
    }
    else
    {
		bool IsDesktopSharCapturing = globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_Desktop);
        globalToolManager->GetDataManager()->WriteLog("%s m_bIsUserSharing:%d\n", __FUNCTION__, IsDesktopSharCapturing);
        if (IsDesktopSharCapturing) {
            globalToolManager->GetPaasSDK()->StopDesktopCapture();
			mbIsDeskTop = false;
            RemoveDesktopStreamMsg("");
        }
        else {
            if (mbIsDeskTop)//桌面共享中
            {
                mbEnableStopDesktopStream = false;
                globalToolManager->GetPaasSDK()->StopDesktopCapture();
                if (m_pScreenShareToolWgd) {
                    m_pScreenShareToolWgd->hide();
                }
                show();
            }
            else
            {
                QDesktopWidget pwd;
                int iIndex = pwd.primaryScreen();
                QScreen *screen = QGuiApplication::primaryScreen();
                QRect mm = screen->availableGeometry();

                int iRef = globalToolManager->GetPaasSDK()->StartDesktopCapture(iIndex, mm.width(), mm.height());
                if (VhallLive_OK != iRef)
                {
                    FadeOutTip(QString::fromWCharArray(L"桌面共享错误，错误码：%1").arg(iRef));
                    return;
                }
                if (globalToolManager->GetDataManager()->GetIsLiving())
                {
                    globalToolManager->GetPaasSDK()->StartPushDesktopStream();
                }
            }
        }
    }
}

void WebRtcLiveWdg::OpenScreenShare()
{
    mbIsDeskTop = true;
    this->hide();
    QRect rect = QApplication::desktop()->availableGeometry();
}

void WebRtcLiveWdg::stopPushStream()
{
	if (!globalToolManager->GetDataManager()->GetIsLiving()) {
		globalToolManager->GetDataManager()->WriteLog("m_streamStatus = false\n");
		ui.widget_ctrl->EnableStartLive(true);
		if (m_pScreenShareToolWgd)
		{
			m_pScreenShareToolWgd->EnableStartLive(true);
		}
		return;
	}
	globalToolManager->GetDataManager()->WriteLog("%s mbEnableStopStream：%d\n", __FUNCTION__, mbEnableStopStream);
	if (!mbEnableStopStream) {
		QString tips = QString::fromWCharArray(L"设备正在初始化，请稍后再结束直播");
		FadeOutTip(tips);
		return;
	}
	HandleStopLiving();
}

void WebRtcLiveWdg::slot_ScreenShareStartLive()
{
	if (!globalToolManager->GetDataManager()->GetIsLiving()) { //开播
		Permission per = globalToolManager->GetPaasSDK()->GetPermission();
		if(per.apply_inav_publish)
			slot_OnToSpeakClicked();
	}
	slot_BtnStreamClicked();
}

void WebRtcLiveWdg::slot_BtnStreamClicked() {

    globalToolManager->GetDataManager()->WriteLog("Enter Function:%s \n", __FUNCTION__);
    globalToolManager->GetDataManager()->WriteLog("Enter Function:%s !pVhallRightExtraWidget\n", __FUNCTION__);
    ui.widget_ctrl->EnableStartLive(true);
	if (m_pScreenShareToolWgd)
	{
		m_pScreenShareToolWgd->EnableStartLive(true);
	}
    if (!globalToolManager->GetDataManager()->GetIsLiving()) { //主播判断未开播，嘉宾判断未上麦
		mbStopLive = false;
        ui.widget_ctrl->EnableStartLive(false);
		if (m_pScreenShareToolWgd)
		{
			m_pScreenShareToolWgd->EnableStartLive(false);
		}
		ui.widget_ctrl->EnableStartLive(true);
		if (m_pScreenShareToolWgd)
		{
			m_pScreenShareToolWgd->EnableStartLive(true);
		}

        if (!globalToolManager->GetPaasSDK()->IsVHMediaConnected()) {
            QString msg;
            msg = QString::fromWCharArray(L"互动房间连接失败，暂不能开始直播，请稍后再试");
            CBaseWnd* wndLoading = globalToolManager->GetWndManager()->FindWnd(WND_ID_LOADING);
            if (wndLoading) {
                CSTDMapParam mapParam;
                mapParam["msg"] = msg;
                wndLoading->Notice(mapParam);
                wndLoading->exec();
            }
            return;
        }
        globalToolManager->GetDataManager()->WriteLog("VhallInteractive::Slot_BtnStreamClicked() StartPushStream\n");

        HandleStartLiving();
    }
    else {
		mbStopLive = true;
        if (!globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_AVCapture) &&
            !globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_Desktop) &&
            !globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_MediaFile)) {
            globalToolManager->GetPaasSDK()->UserPublishCallback(PushStreamEvent::PushStreamEvent_Lower);
        }
        else {
            stopPushStream();
        }
    }
    globalToolManager->GetDataManager()->WriteLog("Leave Function:%s \n", __FUNCTION__);
}

void WebRtcLiveWdg::HandleStopLiving()
{
    globalToolManager->GetDataManager()->WriteLog("VhallInteractive::HandleStopLiving() start\n");
	StopAllRenderStream();
    //ResetPlayMediaFileAndDesktopShare();
    globalToolManager->GetDataManager()->WriteLog("VhallInteractive::HandleStopLiving() end\n");
}

//void WebRtcLiveWdg::ResetPlayMediaFileAndDesktopShare() {
//	globalToolManager->GetPaasSDK()->StopPushMediaFileStream();
//	globalToolManager->GetPaasSDK()->StopPushDesktopStream();
//}

void WebRtcLiveWdg::StopAllRenderStream() {
    QMutexLocker l(&mStreamMutexCS);
    globalToolManager->GetPaasSDK()->StopPushLocalStream();
	//globalToolManager->GetPaasSDK()->StopDesktopCapture();
	//globalToolManager->GetPaasSDK()->StopMediaFileCapture();
	globalToolManager->GetPaasSDK()->StopPushMediaFileStream();
	globalToolManager->GetPaasSDK()->StopPushDesktopStream();
}

//void WebRtcLiveWdg::RemoveAllRender() {
//    globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
//    StopAllRenderStream();
//    globalToolManager->GetDataManager()->WriteLog("%s  end\n", __FUNCTION__);
//}


void WebRtcLiveWdg::StopPlayFile()
{
    //判断是否推流中 如是 则停止推流
    if (globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_MediaFile))
    {
		globalToolManager->GetDataManager()->WriteLog("%s ：StopPushMediaFileStream", __FUNCTION__);
        globalToolManager->GetPaasSDK()->StopPushMediaFileStream();
    }

    //判断是否播放中 如是 则释放播放窗体
    if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile))
    {
		globalToolManager->GetDataManager()->WriteLog("%s ：StopMediaFileCapture", __FUNCTION__);
        globalToolManager->GetPaasSDK()->StopMediaFileCapture();
        ui.MediaFilePlayWdg->hide();
    }

    m_qCycleTimer.stop();
    if (NULL != m_pVedioPlayUI) {
        ReposVedioPlay(!true);
        m_qCycleTimer.stop();
        if (m_pVedioPlayUI != NULL) {
            m_pVedioPlayUI->StopAdminIMMediately(true);
            m_pVedioPlayUI->Destroy();
            delete m_pVedioPlayUI;
            m_pVedioPlayUI = NULL;
			globalToolManager->GetDataManager()->WriteLog("%s ：delete m_pVedioPlayUI", __FUNCTION__);
        }
    }
}

void WebRtcLiveWdg::slot_OnPlayFileClicked() {
	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
    bool bPlaying = globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile);
    globalToolManager->GetDataManager()->WriteLog("%s ：m_bIsMediaPlaying=%d", __FUNCTION__, bPlaying);
    if (bPlaying)
    {
        openMic(true);
        StopPlayFile();
        RemoveMediaFileStreamMsg("");
        //m_bIsMediaPlaying = false;
    }
    else
    {
		if (IsPlayDesktopShare() || globalToolManager->GetPaasSDK()->IsUserPushingDesktopStream()) {
			FadeOutTip(QString::fromWCharArray(L"桌面共享进行中……"));
			return;
		}

		if (globalToolManager->GetPaasSDK()->IsUserPushingMediaFileStream())
		{
			FadeOutTip(QString::fromWCharArray(L"视频插播进行中……"));
			return;
		}

		Permission per = globalToolManager->GetPaasSDK()->GetPermission();
		if (!per.publish_inav_stream) {
			FadeOutTip(QString::fromWCharArray(L"无推流权限不能进行文件插播"));
			return;
		}

        openMic(false);
        StopPlayFile();
        RemoveMediaFileStreamMsg("");

        CreatePlayMediaFileUI();
        ForceHide(false);
        ShowVedioPlayWidget(TRUE);
        //m_bIsMediaPlaying = true;
    }
}

void WebRtcLiveWdg::slot_OnToSpeakClicked() {
    if (globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_AVCapture) ||
        globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_Desktop) ||
        globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_MediaFile)) {
        QString tips = QStringLiteral("已上麦");
        FadeOutTip(tips);
        globalToolManager->GetDataManager()->WriteLog("%s already pushing stream\n", __FUNCTION__);

        return;
    }
    globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
    ui.widget_ctrl->SetToSpeakEnble(false);
    if (NULL == mpEnableSpeakTimer)
    {
        mpEnableSpeakTimer = new QTimer();
        connect(mpEnableSpeakTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_EnableSpeak);
    }

    if (NULL != mpEnableSpeakTimer)
    {
        if (mpEnableSpeakTimer->isActive())
        {
            mpEnableSpeakTimer->stop();
        }
        mpEnableSpeakTimer->start(ENABLESPEAKTIME);
    }

	QString tips;
	
	switch (globalToolManager->GetPaasSDK()->ApplyInavPublish())
	{
	case  VhallLive_OK: break;
	case VhallLive_ROOM_DISCONNECT: //房间已经断开或网络异常
		tips = QStringLiteral("房间已经断开或网络异常");
		break;
	case VhallLive_NO_OBJ://调用的对象为空
		tips = QStringLiteral("调用的对象为空");
		break;
	case VhallLive_NO_DEVICE: //没有音视频设备
		tips = QStringLiteral("没有音视频设备");
		break;
	case VhallLive_NO_PERMISSION: //无权限
		tips = QStringLiteral("无权限");
		break;
	case VhallLive_MUTEX_OPERATE: //互斥操作，桌面共享和插播一个房间只能一个成员触发
		tips = QStringLiteral("互斥操作，桌面共享和插播一个房间只能一个成员触发");
		break;
	case VhallLive_NOT_SUPPROT: //不支持的分辨率
		tips = QStringLiteral("不支持的分辨率");
		break;
	case VhallLive_PARAM_ERR: //参数错误
		tips = QStringLiteral("参数错误");
		break;
	case VhallLive_IS_PROCESSING:  //正在处理中
		tips = QStringLiteral("正在处理中");
		break;
	case VhallLive_SERVER_NOT_READY: //服务器端未就绪
		tips = QStringLiteral("服务器端未就绪");
		break;
	default: tips = QStringLiteral("其它错误，请联系管理员"); break;
	}

	FadeOutTip(tips);
    globalToolManager->GetDataManager()->WriteLog("%s leave\n", __FUNCTION__);
}

void WebRtcLiveWdg::slot_OnTurnToOnLinePage() {
    ui.stackedWidget->setCurrentIndex(0);
}

void WebRtcLiveWdg::slot_OnTurnToKickOutPage() {
    ui.stackedWidget->setCurrentIndex(1);
}

void WebRtcLiveWdg::CreatePlayMediaFileUI()
{
    if (m_pVedioPlayUI == NULL) {

        QWidget* pContentUI = ui.widget_mainView;
        int width = ui.widget_ctrl->width();

        if (width == 0) {
            return;
        }
        //插播窗口创建
        if (NULL == m_pVedioPlayUI)
            m_pVedioPlayUI = new VedioPlayUI(this);
        if (m_pVedioPlayUI == NULL) {
            return;
        }

        if (NULL == m_pVideoChoiceUI)
        {
            m_pVideoChoiceUI = new VideoChoiceUI(this);
            if (NULL == m_pVideoChoiceUI || !m_pVideoChoiceUI->Create()) {
                //ASSERT(FALSE);
                //break;
            }
        }

        m_qCycleTimer.stop();
        m_pVedioPlayUI->Create();
        m_pVedioPlayUI->setFixedWidth(width);
        connect(m_pVedioPlayUI, &VedioPlayUI::SigRepos, this, &WebRtcLiveWdg::SlotRepos);
        connect(m_pVedioPlayUI, &VedioPlayUI::SigForceHide, this, &WebRtcLiveWdg::SlotForceHide);
        connect(m_pVedioPlayUI, &VedioPlayUI::sig_PlayClicked, this, &WebRtcLiveWdg::Slot_VedioPlayClick);
        connect(&m_qCycleTimer, &QTimer::timeout, this, &WebRtcLiveWdg::OnCyclePlay);
        connect(m_pVideoChoiceUI, &VideoChoiceUI::SigSyncFileList, m_pVedioPlayUI, &VedioPlayUI::SlotFlushPlayList);
        connect(m_pVedioPlayUI, &VedioPlayUI::sig_PlayListChecked, this, &WebRtcLiveWdg::PlaySelectedFile);
		connect(m_pVedioPlayUI, &VedioPlayUI::sig_AddFiles, this, &WebRtcLiveWdg::slo_ShowVedioPlayWidget);
		connect(m_pVedioPlayUI, &VedioPlayUI::sig_stoplay, this, &WebRtcLiveWdg::slotDealStopPlay);
    }
}

bool WebRtcLiveWdg::IsEnalbePlayMedia()
{
	Permission per = globalToolManager->GetPaasSDK()->GetPermission();
    bool bref = per.publish_inav_stream;
    return bref;
}

void WebRtcLiveWdg::ForceHide(bool bHide) {
	globalToolManager->GetDataManager()->WriteLog("%s ：bHide=%d ", __FUNCTION__, bHide);

    if (m_pVedioPlayUI == NULL) {
        return;
    }
    ReposVedioPlay(!bHide);
    if (bHide) {
        m_pVedioPlayUI->StopAdminIMMediately(false);
    }
}

void WebRtcLiveWdg::ReposVedioPlay(bool bShow) {
	globalToolManager->GetDataManager()->WriteLog("%s ：bShow=%d ", __FUNCTION__, bShow);
    int left = 0;
    int right = 0;
    if (!m_pVedioPlayUI) {
        return;
    }
	calVedioPlayUISize();

}

void  WebRtcLiveWdg::FadeOutTip(QString str) {
    if (mpFadeOutTipWidget) {
        mpFadeOutTipWidget->Tip(NULL, QPoint(ui.bgWdg->x(), ui.widget_title->height()), str);
        mpFadeOutTipWidget->rePos(NULL);
    }
}

void WebRtcLiveWdg::ShowVedioPlayWidget(BOOL bShow) {
	globalToolManager->GetDataManager()->WriteLog("%s ：bShow=%d ", __FUNCTION__, bShow);
    if (NULL != m_pVedioPlayUI) {

        if (bShow) {
			globalToolManager->GetDataManager()->WriteLog("%s ：m_pVideoChoiceUI init ", __FUNCTION__);
            int currentIndex = 0;
            QStringList fileList = m_pVedioPlayUI->GetPlayList(currentIndex);
            m_pVideoChoiceUI->Clear();
            m_pVideoChoiceUI->AppendFileList(fileList, currentIndex);

            bool isVedioPlayUIShow = false;
            isVedioPlayUIShow = m_pVedioPlayUI->IsShow();
            CycleType currentMode = m_pVedioPlayUI->GetCurPlayMode();
            m_pVideoChoiceUI->SetCircleMode((int)currentMode);
            m_pVideoChoiceUI->FlashTips();
            m_pVideoChoiceUI->CenterWindow(NULL);

            m_pVideoChoiceUI->SetCurrentPlayOutState(false);
            m_pVideoChoiceUI->HidePlayOutCheck(true);

            VHDialog::WillShow(m_pVideoChoiceUI);
            if (m_pVideoChoiceUI->exec() == QDialog::Accepted) {
                int currentIndex = 0;
                QStringList playList = m_pVideoChoiceUI->GetPlayList(currentIndex);
                int circleMode = m_pVideoChoiceUI->getCircleMode();
                if (playList.count() > 0) {
                    m_pVedioPlayUI->SetCurPlayMode((CycleType)circleMode);
                    m_pVedioPlayUI->AppendPlayList(playList, currentIndex);
                    ReposVedioPlay(bShow);
                    ReposVedioPlay(false);
                    m_pVedioPlayUI->show();
                    //m_bShowState = bShow;
                    isVedioPlayUIShow = true;
                    ReposVedioPlay(true);
                }
                if (!m_qCycleTimer.isActive()) {
                    m_qCycleTimer.start(400);
                }
                m_pVideoChoiceUI->FLushFileList(true);
                //mbIsEnablePlayOutAudio = m_pVideoChoiceUI->IsEnablePlayOutAudio();
            }
            else {
                int currentIndex = 0;
                QStringList playList = m_pVideoChoiceUI->GetPlayList(currentIndex);
                if (playList.count() == 0) {
                    DealStopPlay(NULL, 0);
                }
                m_pVideoChoiceUI->FLushFileList(false);
            }
            if (isVedioPlayUIShow) {
                m_pVedioPlayUI->SlotSetForceActice(true);
                m_pVedioPlayUI->TryHideToolBar();
            }
        }
        else {
			globalToolManager->GetDataManager()->WriteLog("%s ：m_pVideoChoiceUI hide ", __FUNCTION__);
            m_pVedioPlayUI->hide();
            m_pVedioPlayUI->StopAdmin();
        }
    }
}

void WebRtcLiveWdg::DealStopPlay(void* apData, DWORD adwLen) {
    //关闭循环播放定时器
    m_qCycleTimer.stop();
    //隐藏UI
    ShowVedioPlayWidget(FALSE);
    ForceHide(true);

	StopPlayMediaFile();
}

void WebRtcLiveWdg::SlotRepos(bool bRePos)
{
    ReposVedioPlay(bRePos);
}

void WebRtcLiveWdg::SlotForceHide(bool bHide) {
    ForceHide(bHide);
}

int WebRtcLiveWdg::GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos) {
    int nRet = -1;
    nRet = globalToolManager->GetPaasSDK()->MediaGetPlayerState();
	n64Pos = globalToolManager->GetPaasSDK()->MediaFileGetCurrentDuration();
	n64MaxPos = globalToolManager->GetPaasSDK()->MediaFileGetMaxDuration();
    if (nRet == 7) {
        MediaFilePlayFaild();
    }
    return nRet;
}

void WebRtcLiveWdg::StopPlayMediaFile() {
	if (globalToolManager->GetPaasSDK()->IsPushingStream(VHStreamType_MediaFile))
	{
		globalToolManager->GetDataManager()->WriteLog("%s ：StopPushMediaFileStream", __FUNCTION__);
		globalToolManager->GetPaasSDK()->StopPushMediaFileStream();
	}

	//判断是否播放中 如是 则释放播放采集中
	if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile))
	{
		globalToolManager->GetDataManager()->WriteLog("%s ：StopMediaFileCapture", __FUNCTION__);
		globalToolManager->GetPaasSDK()->StopMediaFileCapture();
	}

    StopShowMediaVideo();
}

void WebRtcLiveWdg::StopShowMediaVideo() {
    globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
	ui.MediaFilePlayWdg->hide();
	SetPlayDeskTopAndMediaFileState(false);
    //if (mMediaFilePlayWdg) {
    //    ui.verticalLayout_desktopAndMedia->removeWidget(mMediaFilePlayWdg);
    //    delete mMediaFilePlayWdg;
    //    mMediaFilePlayWdg = NULL;
    //}
    globalToolManager->GetDataManager()->WriteLog("%s leaveHandleConfigMediaFileMainView\n", __FUNCTION__);

}

void WebRtcLiveWdg::Slot_VedioPlayClick()
{
	signed long long curPos, maxPos;
    int nState = GetPlayFileState(curPos, maxPos);
    if (nState == PLAYUI_PLAYSTATE_PAUSED) {
        globalToolManager->GetPaasSDK()->MediaFileResume();
    }
    else if (nState == PLAYUI_PLAYSTATE_PALYERING) {
        globalToolManager->GetPaasSDK()->MediaFilePause();
    }
    else {
        QString fileName;
        if (m_pVedioPlayUI == NULL) {
            return;
        }
        bool bCanPlay = m_pVedioPlayUI->GetCurFile(fileName);
        unsigned int iVolume = m_pVedioPlayUI->GetCurVolume();
        if (bCanPlay) {
            globalToolManager->GetDataManager()->WriteLog("%s playfile:%s\n", __FUNCTION__, fileName.toStdString().c_str());

            int iRef = globalToolManager->GetPaasSDK()->StartPlayMediaFile(fileName.toStdString());
            if (VhallLive_OK == iRef)
            {
                globalToolManager->GetPaasSDK()->SetCurrentMicVol(iVolume);
            }
            else
            {
                FadeOutTip(QString::fromWCharArray(L"插播视频错误，错误码：%1").arg(iRef));
                return;
            }
        }
        else {
            AddPlayFiles();
            PlaySelectedFile();
        }
    }
}

void WebRtcLiveWdg::AddPlayFiles() {
    if (NULL == m_pVedioPlayUI) {
        return;
    }
    QStringList selectFileName;
    selectFileName = QFileDialog::getOpenFileNames(NULL, QObject::tr("打开文件"), "", QObject::tr("文件(*.mp3 *.wav *.mp4 *.flv *.mov *.rmvb );"), 0, 0);
    if (selectFileName.isEmpty()) {
        return;
    }


    foreach(QString filename, selectFileName) {
        m_pVedioPlayUI->AddPlayList((char*)filename.toStdString().c_str());
    }
}

void WebRtcLiveWdg::OnCyclePlay()
{
    HandleVhallActiveCyclePlay();
}

void WebRtcLiveWdg::HandleVhallActiveCyclePlay() {
    if (!m_pVedioPlayUI) {
        return;
    }

	signed long long curPos, maxPos;
	int nState = GetPlayFileState(curPos, maxPos);
    long dulation = curPos;
    long maxDulation = maxPos;

    QString dulationStr = dulation2Str(dulation, maxDulation);
    if (nState == PLAYUI_PLAYSTATE_STOP ||
        nState == PLAYUI_PLAYSTATE_END ||
        nState == PLAYUI_PLAYSTATE_FAIL) {
        QString dulationStr = dulation2Str(0, 0);
        m_pVedioPlayUI->UpdatePlayTime(dulationStr);
        m_pVedioPlayUI->UpdatePlayStates(FALSE);
        m_pVedioPlayUI->SetProgressValue(0, 0);
    }
    else if (nState == PLAYUI_PLAYSTATE_PAUSED) {
        m_pVedioPlayUI->UpdatePlayTime(dulationStr);
        m_pVedioPlayUI->UpdatePlayStates(FALSE);
        m_pVedioPlayUI->SetProgressValue(dulation, maxDulation);
    }
    else if (PLAYUI_PLAYSTATE_STOPPING == nState) {
        m_pVedioPlayUI->UpdatePlayTime(dulationStr);
        m_pVedioPlayUI->UpdatePlayStates(FALSE);
        m_pVedioPlayUI->SetProgressValue(dulation, maxDulation);
    }
    else {
        m_pVedioPlayUI->UpdatePlayTime(dulationStr);
        m_pVedioPlayUI->UpdatePlayStates(TRUE);
        m_pVedioPlayUI->SetProgressValue(dulation, maxDulation);
    }

    switch (nState) {
    case PLAYUI_PLAYSTATE_STOPPING:
    case PLAYUI_PLAYSTATE_OPENING:
    case PLAYUI_PLAYSTATE_BUFFERING:
        m_pVedioPlayUI->Enabled(false);
        break;
    default:
        m_pVedioPlayUI->Enabled(true);
        break;
    }

    if (nState == PLAYUI_PLAYSTATE_END) {
        switch (m_pVedioPlayUI->GetCurPlayMode()) {
        case CycleTyle_None: {				//顺序播放
            if (!m_pVedioPlayUI->IsArriveTheEnd()) {
                m_pVedioPlayUI->SwitchToNext();
                PlaySelectedFile();
            }
            else {
                DealStopPlay(NULL, 0);
            }
        }
                             break;
        case CycleTyle_List: {	//列表循环
            m_pVedioPlayUI->SwitchToNext();
            PlaySelectedFile();
        }
                             break;
        case CycleTyle_Single: { //单视频循环
            PlaySelectedFile();
        }
                               break;
        case CycleTyle_END:
            break;
        default:
            break;
        }
    }

    if (m_bPlayCurrentFile) {
        if (NULL != m_pVedioPlayUI) {
            QString szFileName;
            bool bCanPlay = m_pVedioPlayUI->GetCurFile(szFileName);
            unsigned int iVolume = m_pVedioPlayUI->GetCurVolume();
            if (bCanPlay) {
                if (OpenPlayMediaFile((char*)szFileName.toStdString().c_str()) == 0) {
                    SetPlayMeidaFileVolume(iVolume);
                    m_bPlayCurrentFile = false;
                }
            }
        }
    }
}


void WebRtcLiveWdg::HandleDeviceEvent(QEvent* event) {
    if (event == NULL) {
        return;
    }
    CustomDevChangedEvent* devEvent = (CustomDevChangedEvent*)event;
    if (devEvent && globalToolManager->GetPaasSDK()) {

        m_IsPushingLocalStream = isPushing();

        globalToolManager->GetPaasSDK()->StopLocalCapture();
        mCurCameraDevID = devEvent->cameraDevId;
        mCurMicDevID = devEvent->micDevId;
        mCurPlayerID = devEvent->playerDevId;

        globalToolManager->GetPaasSDK()->SetUseMicIndex(devEvent->micIndex);
        globalToolManager->GetPaasSDK()->SetUseCameraGuid(devEvent->cameraDevId.toStdString());

        globalToolManager->GetPaasSDK()->SetUserPlayIndex(devEvent->playerIndex);
		int nRet = globalToolManager->GetPaasSDK()->StartLocalCapture((VideoProfileIndex)devEvent->cameraIndex);
        if (nRet != 0) {
            FadeOutTip(QStringLiteral("音视频设备无法正常打开，请检测设备是否已连接"));
        }

    }
}

void WebRtcLiveWdg::HandleGetVHMemberList(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventVHMember* member = dynamic_cast<QEventVHMember*>(event);
    if (member) {
        AddMemeberIntoOnLineList(member->thirdPartyUserId, member->memberStatusType);
    }
	else
	{
		//int a = 0;
	}
}

void  WebRtcLiveWdg::HandleUserPublishCallback(QEvent* event) {
    if (event == nullptr) {
        return;
    }
	QEventUserPublish* member = dynamic_cast<QEventUserPublish*>(event);
    if (member) {
        VHRoomMemberWdg* memberOnLine = GetRoomMemberFromOnLineList(member->thirdPartyUserId);
        if (memberOnLine) {
            memberOnLine->SetUserStatus(member->memberStatusType);
        }

		QString objId = member->thirdPartyUserId;
		if (PushStreamEvent_Refused == member->memberStatusType)
		{
			FadeOutTip(QString::fromWCharArray(L"%1还没有准备好，暂时不能上麦").arg(member->thirdPartyUserId.toStdString().c_str()));
		}
		else if (objId.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) == 0)
		{
			 if (PushStreamEvent_Lower == member->memberStatusType /*&& !m_SelfStopPush*/)//被动下麦
			{
				Permission per = globalToolManager->GetPaasSDK()->GetPermission();
				ui.widget_ctrl->SetToSpeakBtnState(!per.apply_inav_publish);//申请上麦
				ui.widget_ctrl->OnStartLive(false);
			}
			else if (PushStreamEvent_Upper == member->memberStatusType && m_AgreePush)//同意被邀请上麦
			{
				SetAgreePush(false);
				ui.widget_ctrl->OnStartLive(true);
				ui.widget_ctrl->SetToSpeakBtnState(true);
			}
			 //m_SelfStopPush = false;
		}
		
		if (member->thirdPartyUserId.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) == 0)
		{
			VHRoomMemberWdg *widget = GetRoomMemberFromOnLineList(globalToolManager->GetDataManager()->GetThridPartyUserId());
			if (nullptr != widget)
			{
				
				if (PushStreamEvent_Upper == member->memberStatusType)
				{
					widget->SetUserStatus(MemberStatus_Pushing);
				}
				else
				{
					slot_showMemberList();
				}
			}
		}
    }
}

void  WebRtcLiveWdg::HandleRecvKickInvaMsg(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventPublishStream* member = dynamic_cast<QEventPublishStream*>(event);
    if (member) {
        RemoveMemberFromOnLineUser(member->strStream);
        AddMemberIntoKickOutLineList(member->strStream, MemberStatus_KickOut);
        //当被用户踢出时，退出互动房间。
        if (member->strStream == globalToolManager->GetDataManager()->GetThridPartyUserId()) {
			//结束推流
            globalToolManager->GetPaasSDK()->LeaveVHMediaRoom();
            globalToolManager->GetPaasSDK()->LogoutHVRoom();
            globalToolManager->PostEventToMainThread(new QEvent(CustomEvent_KICKOUT_ROOM));
        }
    }

}

void WebRtcLiveWdg::HandleGetKickOutMemberList(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventVHMember* member = dynamic_cast<QEventVHMember*>(event);
    if (member) {
        AddMemberIntoKickOutLineList(member->thirdPartyUserId, MemberStatus_KickOut);
    }
}

void WebRtcLiveWdg::ClearAllOnLineUser() {
	while (ui.listWidget_on_line->count() > 0) {
		QListWidgetItem *item = ui.listWidget_on_line->item(0);
		if (item) {
			VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_on_line->itemWidget(item));
			if (widget) {
				delete widget;
				widget = NULL;
				QListWidgetItem *item = ui.listWidget_on_line->takeItem(0);
				delete item;
			}
			else
			{
				break;

			}
		}
	}
	int count = ui.listWidget_on_line->count();
	ui.pushButton_onLine->setText(QStringLiteral("在线(%1)").arg(count));
}

void WebRtcLiveWdg::ClearAllKickOutUSer() {
	while (ui.listWidget_kickOut->count() > 0) {
		QListWidgetItem *item = ui.listWidget_kickOut->item(0);
		if (item) {
			VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_kickOut->itemWidget(item));
			if (widget) {
				delete widget;
				widget = NULL;
				QListWidgetItem *item = ui.listWidget_kickOut->takeItem(0);
				delete item;
			}
		}
	}
}

void WebRtcLiveWdg::HandleErrorEventRoomEvent(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
    if (roomEvent) {
        switch (roomEvent->mCallBackCodeType)
        {
        case RoomEvent_InavUserList: {
            break;
        }
        case RoomEvent_Apply_Push: {
            break;
        }
        case RoomEvent_AuditInavPublish: {
            HandleAuditInavPublish(roomEvent, false);
            break;
        }
        case RoomEvent_AskforInavPublish: {
            HandleAskforInavPublish(roomEvent, false);
            break;
        }
        case RoomEvent_UserPublishCallback: {
            break;
        }
        case RoomEvent_KickInavStream: {
            HandleKickInavStream(roomEvent, false);
            break;
        }
        case RoomEvent_KickInav: {
            HandleKickInva(roomEvent, false);
            break;
        }
        case RoomEvent_GetKickInavList: {
            break;
        }
        case RoomEvent_NoPushStreamPermission: {
            QString msg = QStringLiteral("您没有推流权限");
            FadeOutTip(msg);
            break;
        }
        case RoomEvent_Start_PublishInavAnother:
            HandlePublishInavAnotherError(event);
            break;
        case RoomEvent_Stop_PublishInavAnother:
            HandleStopPublishInavAnotherError(event);
            break;
        case RoomEvent_SetMainView: {
            if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile)) {
                mReSetMediaFileMainViewTimer.start(3000);
                mReSetMediaFileMainViewTimer.setSingleShot(true);
            }
            else if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop)) {
                mReSetDesktopMainViewTimer.start(3000);
                mReSetDesktopMainViewTimer.setSingleShot(true);
            }
            break;
        }
        default:
            break;
        }
    }
}

void WebRtcLiveWdg::HandleSuccessEventRoomEvent(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
    if (roomEvent) {
        switch (roomEvent->mCallBackCodeType)
        {
        case RoomEvent_InavUserList: {
            break;
        }
        case RoomEvent_Apply_Push: {
            break;
        }
        case RoomEvent_AuditInavPublish: {
            HandleAuditInavPublish(roomEvent, true);
            break;
        }
        case RoomEvent_AskforInavPublish: {
            HandleAskforInavPublish(roomEvent, true);
            break;
        }
        case RoomEvent_UserPublishCallback: {
            break;
        }
        case RoomEvent_KickInavStream: {
            //下麦处理
            HandleKickInavStream(roomEvent, true);
            break;
        }
        case RoomEvent_KickInav: {
            //被踢出互动房间
            HandleKickInva(roomEvent, true);
            break;
        }
        case RoomEvent_GetKickInavList: {
            break;
        }
        case RoomEvent_Start_PublishInavAnother:
            HandlePublishInavAnother(roomEvent);
            break;
        case RoomEvent_Stop_PublishInavAnother:
            HandleStopPublishInavAnother(roomEvent);
            break;
		case RoomEvent_Logout:
			HandleLogout(roomEvent);
			break;
        default:
            break;
        }
    }
}

bool WebRtcLiveWdg::StartPushStream(int & iState)
{
	bool bRef = false;
	if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_AVCapture)) {
		iState = globalToolManager->GetPaasSDK()->StartPushLocalStream();
		bRef = true;
	}

	if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop)) {
		globalToolManager->GetPaasSDK()->StartPushDesktopStream();
		bRef = true;
	}
	else if (globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile)) {
		globalToolManager->GetPaasSDK()->StartPushMediaFileStream();
		bRef = true;
	}
	return bRef;
}

void WebRtcLiveWdg::HandleStartLiving() {
	bool bRef = false;
    //开始推流
	int iStat = 0;
	bRef = StartPushStream(iStat);

	if (!bRef)
	{
		QString tips = QStringLiteral("请检查视频采集设备后重试！");
		FadeOutTip(tips);
	}
}

void WebRtcLiveWdg::HandleAskforInavPublish(QEventRoomEvent* roomEvent, bool sucessed) {
    if (sucessed) {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            member->SetUserStatus(MemberStatus_BeInvited);
        }
    }
    else {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            member->EnableAllButton();
        }
        QString tips = QStringLiteral("邀请") + roomEvent->mUserId + QStringLiteral("上麦失败：") + QString::fromStdString(roomEvent->mRespMsg);
        FadeOutTip(tips);
    }
}

void WebRtcLiveWdg::HandleKickInavStream(QEventRoomEvent* roomEvent, bool sucessed) {
    if (sucessed) {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            member->SetUserStatus(MemberStatus_Watching);
        }
    }
    else {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            member->EnableAllButton();
        }
        QString tips = QStringLiteral("下麦") + roomEvent->mUserId + QStringLiteral("失败：") + QString::fromStdString(roomEvent->mRespMsg);
        FadeOutTip(tips);
    }
}

void WebRtcLiveWdg::HandleKickInva(QEventRoomEvent* roomEvent, bool sucessed) {
    if (sucessed) {
        if (roomEvent->mProcessType == KickStream_KickOutUser) {
            RemoveMemberFromOnLineUser(roomEvent->mUserId);
            AddMemberIntoKickOutLineList(roomEvent->mUserId, MemberStatus_KickOut);
        }
        else if (roomEvent->mProcessType == KickStream_CancelKickOutUser) {
            RemoveMemberFromKickOutUser(roomEvent->mUserId);
            // AddMemeberIntoOnLineList(QString::fromStdString(roomEvent->mUserId), MemberStatus_Watching);
        }
    }
    else {
        if (roomEvent->mProcessType == KickStream_KickOutUser) {
            QString tips = roomEvent->mUserId + QStringLiteral(" 踢出互动房间失败：") + QString::fromStdString(roomEvent->mRespMsg);
            FadeOutTip(tips);
        }
        else if (roomEvent->mProcessType == KickStream_CancelKickOutUser) {
            QString tips = roomEvent->mUserId + QStringLiteral(" 取消踢出互动房间失败：") + QString::fromStdString(roomEvent->mRespMsg);
            FadeOutTip(tips);
        }
    }
}

void WebRtcLiveWdg::HandleAuditInavPublish(QEventRoomEvent* roomEvent, bool sucessed) {
    if (sucessed) {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            //发送同于上麦成功之后，设置当前用户状态为推流中。
            if (roomEvent->mProcessType == AuditPublish_Accept) {
                member->SetUserStatus(MemberStatus_Pushing);
            }
            else {
                member->SetUserStatus(MemberStatus_Watching);
            }
        }
    }
    else {
        VHRoomMemberWdg* member = GetRoomMemberFromOnLineList(roomEvent->mUserId);
        if (member) {
            member->EnableAllButton();
        }
        if (member) {
            //发送同于上麦成功之后，设置当前用户状态为推流中。
            if (roomEvent->mProcessType == AuditPublish_Accept) {
                QString tips = QStringLiteral("同意") + roomEvent->mUserId + QStringLiteral("失败：") + QString::fromStdString(roomEvent->mRespMsg);
                FadeOutTip(tips);
            }
            else {
                member->SetUserStatus(MemberStatus_Watching);
            }
        }
    }
}

VHRoomMemberWdg* WebRtcLiveWdg::GetRoomMemberFromOnLineList(QString userId) {
    for(int i = 0; i < ui.listWidget_on_line->count(); i++)
    {
        QListWidgetItem *item = ui.listWidget_on_line->item(i);
        if (item) {
            VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_on_line->itemWidget(item));
            if (widget) {
                if (widget->GetThirdPartyUserId() == userId) {
                    return widget;
                }
            }
        }
    }
    return nullptr;
}

VHRoomMemberWdg* WebRtcLiveWdg::GetRoomMemberFromKickOutLineList(QString userId) {
    int counter = ui.listWidget_kickOut->count();
    for (int index = 0; index < counter; index++)
    {
        QListWidgetItem *item = ui.listWidget_kickOut->item(index);
        if (item) {
            VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_kickOut->itemWidget(item));
            if (widget) {
                if (widget->GetThirdPartyUserId() == userId) {
                    return widget;
                }
            }
        }
    }
    return nullptr;
}

void WebRtcLiveWdg::RemoveMemberFromOnLineUser(QString userId) {
    int counter = ui.listWidget_on_line->count();
    for (int index = 0; index < counter; index++) {
        QListWidgetItem *item = ui.listWidget_on_line->item(index);
        if (item) {
            VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_on_line->itemWidget(item));
            if (widget) {
                if (widget->GetThirdPartyUserId() == userId) {
					delete widget;
					widget = NULL;
                    QListWidgetItem *item = ui.listWidget_on_line->takeItem(index);
                    delete item;
                }
            }
        }
    }
    int count = ui.listWidget_on_line->count();
    ui.pushButton_onLine->setText(QStringLiteral("在线(%1)").arg(count));
}

void WebRtcLiveWdg::RemoveMemberFromKickOutUser(QString userId) {

	int counter = ui.listWidget_kickOut->count();
	for (int index = 0; index < counter; index++) {

        QListWidgetItem *item = ui.listWidget_kickOut->item(index);
        if (item) {
            VHRoomMemberWdg *widget = dynamic_cast<VHRoomMemberWdg*>(ui.listWidget_kickOut->itemWidget(item));
            if (widget) {
                if (widget->GetThirdPartyUserId() == userId) {
					delete widget;
					widget = NULL;
                    QListWidgetItem *item = ui.listWidget_kickOut->takeItem(index);
                    delete item;
                }
            }
        }
    }
    int count = ui.listWidget_kickOut->count();
    ui.pushButton_kickout->setText(QStringLiteral("踢出(%1)").arg(count));
}

bool WebRtcLiveWdg::isPushing() 
{
	bool bRef = false;
	for (int i= VHStreamType_PureAudio; i< VHStreamType_Count; i++)
	{
		if (globalToolManager->GetPaasSDK()->IsPushingStream(vlive::VHStreamType(i)))
		{
			bRef = true;
			break;
		}
	}
	return bRef;
}

void  WebRtcLiveWdg::AddMemeberIntoOnLineList(QString userId, int state) {
    if (GetRoomMemberFromOnLineList(userId) == nullptr) {
		//ClearAllOnLineUser();
		VHRoomMemberWdg *w = new VHRoomMemberWdg(MEMBER_LIST_ONLINE_LIST_MEMBER, userId.toStdString(), state, this);
        
		bool bSelf = userId.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) == 0;
		Permission per = globalToolManager->GetPaasSDK()->GetPermission();
		//w->ShowInvite(per.askfor_inav_publish&&bNotSelf);//邀请上麦
		//w->ShowKickout(per.kick_inav&&bNotSelf);//踢出互动房间 / 取消踢出互动房间
		//w->ShowStopActive(per.kick_inav_stream&&bNotSelf);//踢出路流 / 取消踢出流   将别人下麦

		QListWidgetItem *renderViewItem = new QListWidgetItem(ui.listWidget_on_line);
        ui.listWidget_on_line->setItemWidget(renderViewItem, w);
        ui.listWidget_on_line->addItem(renderViewItem);
        int count = ui.listWidget_on_line->count();
        ui.pushButton_onLine->setText(QStringLiteral("在线(%1)").arg(count));
		w->SetUserStatus(state);
		if (bSelf && (state!= MemberStatus_Watching)	&& (!isPushing()))//得到自己信息 非观看端状态 
		{
			//m_SelfStopPush = true;
			globalToolManager->GetPaasSDK()->UserPublishCallback(PushStreamEvent::PushStreamEvent_Lower);
		}
		
    }
	else
	{
		//int a = 0;
	}
}

void  WebRtcLiveWdg::AddMemberIntoKickOutLineList(QString userId, int state) {
    if (GetRoomMemberFromKickOutLineList(userId) == nullptr) {
		//ClearAllKickOutUSer();
		VHRoomMemberWdg *w = new VHRoomMemberWdg(MEMBER_LIST_KICK_OUT, userId.toStdString(), state, ui.listWidget_kickOut);
		bool bNotSelf = userId.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0;
		Permission per = globalToolManager->GetPaasSDK()->GetPermission();
		//w->ShowInvite(per.askfor_inav_publish&&bNotSelf);//邀请上麦
		w->CancelShowKickout(per.kick_inav&&bNotSelf);//踢出互动房间 / 取消踢出互动房间
		QListWidgetItem *renderViewItem = new QListWidgetItem(ui.listWidget_kickOut);
        ui.listWidget_kickOut->setItemWidget(renderViewItem, w);
        ui.listWidget_kickOut->addItem(renderViewItem);
        int count = ui.listWidget_kickOut->count();
        ui.pushButton_kickout->setText(QStringLiteral("踢出(%1)").arg(count));
    }
}

void WebRtcLiveWdg::HandleSubScribSucEvent(QEvent* eventMsg)
{
    globalToolManager->GetDataManager()->WriteLog("%s start\n", __FUNCTION__);
    if (eventMsg == NULL) {
        return;
    }
    QEventStream* event = (QEventStream*)eventMsg;
    //如果有流，先判断是否有窗口，如果没有窗口不显示。
    if (event) {
        //mStreamMutexCS.Lock();
        QMutexLocker l(&mStreamMutexCS);
        globalToolManager->GetDataManager()->WriteLog("%s to find uid:%s\n", __FUNCTION__, event->strUser.toStdString().c_str());
        if (event->mStreamType < VHStreamType_Desktop)//远端非桌面共享、插播视频的普通流
        {
            VhallRenderWdg *render = GetRenderWnd(event->strUser);
            if (NULL == render)
            {
                bool bIsCameraOpen = true;
                bool bIsMicOpen = true;
                bool bShowMain = false;
                AppendRenderUser(event->strUser, bIsMicOpen, bIsCameraOpen, bShowMain);
            }

            render = GetRenderWnd(event->strUser);

            if (render) {
                HWND wnd = render->GetRenderWndID();
                render->SetViewState(RenderView_None);
                render->show();
                if (IsWindow(wnd)) {
                    globalToolManager->GetDataManager()->WriteLog("%s play remote camera\n", __FUNCTION__);
                    globalToolManager->GetPaasSDK()->StartRenderStream(event->strUser.toStdWString(), wnd, (vlive::VHStreamType)event->mStreamType);
                    if (event->mbHasVideo) {
                        if (render->GetUserCameraState()) {
                            render->SetEnableUpdateRendWnd(false);
                        }
                        else {
                            render->SetEnableUpdateRendWnd(true);
                        }
                    }
                    if (mRenderMap.size() == 1) {
                        SetMainWdgRenderCameraUser(event->strUser);
                    }
                }
                else {
               }
                globalToolManager->GetDataManager()->WriteLog("%s find:%s  to play\n", __FUNCTION__, event->strUser.toStdString().c_str());
                SetSpeakUserOffLine(event->strUser, false);
            }
        }
        else if (event->mStreamType == VHStreamType_Desktop) {
			//订阅到桌面共享流
            RecvRemoteMediaStream();
			SetPlayDeskTopAndMediaFileState(true);
			ui.DesktopWdg->show();
            HWND wnd = (HWND)(ui.DesktopWdg->winId());
            if (IsWindow(wnd)) {
            	globalToolManager->GetDataManager()->WriteLog("%s play remote mDesktopWdg\n", __FUNCTION__);
                globalToolManager->GetPaasSDK()->StartRenderStream(event->strUser.toStdWString(), wnd, (vlive::VHStreamType)event->mStreamType);
            }
        }
        else if (event->mStreamType == VHStreamType_MediaFile) {
            RecvRemoteMediaStream();
			SetPlayDeskTopAndMediaFileState(true);
            if (event->mbHasVideo) {
                ui.MediaFilePlayWdg->show();
                HWND wnd = (HWND)(ui.MediaFilePlayWdg->winId());
                if (IsWindow(wnd)) {
                    globalToolManager->GetDataManager()->WriteLog("%s play remote mMediaFilePlayWdg\n", __FUNCTION__);
                    globalToolManager->GetPaasSDK()->StartRenderStream(event->strUser.toStdWString(), wnd, (vlive::VHStreamType)event->mStreamType);
                }
            }
            else {
                ui.MediaFilePlayWdg->hide();
            }
        }
    }
    globalToolManager->GetDataManager()->WriteLog("%s end\n", __FUNCTION__);
}

//添加远端视频成员窗体
int WebRtcLiveWdg::AppendRenderUser(const QString& uid, const bool audio,
    const bool video, bool showInMainView, bool show /*= true*/)
{
    int iRef = 0;
    //globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
    //如果已经存在了窗口，不再创建。
    if (IsExistRenderWnd(uid) || uid.isEmpty()) {
        return -1;
    }
    //globalToolManager->GetDataManager()->WriteLog("%s showInMainView %d  uid:%s -- mainView:%s\n", __FUNCTION__, showInMainView, uid.toStdString().c_str(), mActiveParam.mainShow.toStdString().c_str());
    VhallRenderWdg *renderView = new VhallRenderWdg(this);
    if (renderView) {
        QString strJoinId = globalToolManager->GetDataManager()->GetThridPartyUserId();
        renderView->InitUserInfo(uid, showInMainView, strJoinId);
        renderView->SetUserCameraState(video);
        SetMainViewCameraState(uid, video);
        renderView->SetUserMicState(audio);
        SetMainViewMicState(uid, audio);
        if (video) {
            renderView->SetViewState(RenderView_None);
        }
        else {
            renderView->SetViewState(RenderView_NoCamera);
        }

        connect(renderView, &VhallRenderWdg::sig_setToSpeaker, this, &WebRtcLiveWdg::slot_setToSpeaker);
        connect(renderView, &VhallRenderWdg::sig_ClickedCamera, this, &WebRtcLiveWdg::slot_OnClickedCamera);
        connect(renderView, &VhallRenderWdg::sig_ClickedMic, this, &WebRtcLiveWdg::slot_ClickedMic);
        connect(renderView, &VhallRenderWdg::sig_setInMainView, this, &WebRtcLiveWdg::slot_SetMainWdgRender);

        renderView->ResetViewSize(false);
        QListWidgetItem *renderViewItem = new QListWidgetItem(ui.listWidget_topView);
        ui.listWidget_topView->setItemWidget(renderViewItem, renderView);
        ui.listWidget_topView->addItem(renderViewItem);

        if (ui.listWidget_topView->count() > 1) {
            ui.widget_topRenderView->show();
        }


        AddRenderToMap(uid, renderView);
        if (show) {
            renderView->show();
        }
        else {
            renderView->hide();
        }

        QMutexLocker l(&mSpeakUserMapMutex);
        std::map<QString, SpeakUser*>::iterator iter = mSpeakUserMap.find(uid);
        if (iter == mSpeakUserMap.end()) {
            mSpeakUserMap.insert(std::make_pair(uid, new SpeakUser(uid/*, role, nickName*/)));
        }
    }

    if (ui.listWidget_topView->count() > 0) {
        ui.listWidget_topView->show();
    }

    return iRef;
}

void WebRtcLiveWdg::AddRenderToMap(const QString& id, VhallRenderWdg*render) {
    QMutexLocker l(&mRenderViewMutex);
    QMap<QString, VhallRenderWdg*>::iterator iter = mRenderMap.find(id);
    if (iter == mRenderMap.end()) {
        mRenderMap.insert(id, render);
    }

    if (mRenderMap.size() > 1) {
        ui.widget_topRenderView->show();
    }
}

void WebRtcLiveWdg::SetMainWdgRenderCameraUser(const QString& user_id) {
    if (globalToolManager->GetPaasSDK()->IsUserPushingDesktopStream() 
		|| globalToolManager->GetPaasSDK()->IsUserPushingMediaFileStream() 
		|| globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop) 
		|| globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile)) {
        return;
    }


    if (mCurMainShowUid != user_id) {//当前大屏幕不等于当前登录者
        VhallRenderWdg *render = GetRenderWnd(user_id);
        for (int i = 0; i < ui.listWidget_topView->count(); i++) {
            QListWidgetItem *item = ui.listWidget_topView->item(i);
            if (NULL != item) {
                QWidget *w = ui.listWidget_topView->itemWidget(item);
                if (NULL != w) {
                    VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                    if (itemWidget->isCurrentUser(user_id)) {
                        item->setHidden(true);
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < ui.listWidget_topView->count(); i++) {
            QListWidgetItem *item = ui.listWidget_topView->item(i);
            if (NULL != item) {
                QWidget *w = ui.listWidget_topView->itemWidget(item);
                if (NULL != w) {
                    VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                    if (itemWidget->isCurrentUser(mCurMainShowUid)) {
                        item->setHidden(false);
                        break;
                    }
                }
            }
        }

        HWND wnd = ui.widget_mainView->GetRenderWndID();
        ui.widget_mainView->InitUserInfo(user_id, true, user_id);
        ui.widget_mainView->SetUserCameraState(render->GetUserCameraState());
        ui.widget_mainView->SetUserMicState(render->GetUserMicState());
        if (render->GetUserCameraState()) {
            ui.widget_mainView->SetViewState(RenderView_None);
        }
        else {
            ui.widget_mainView->SetViewState(RenderView_NoCamera);
        }
        globalToolManager->GetPaasSDK()->StartRenderStream(user_id.toStdWString(), wnd, VHStreamType_AVCapture);

        if (!mCurMainShowUid.isEmpty()) {
            VhallRenderWdg *show_render = GetRenderWnd(mCurMainShowUid);
            if (show_render) {
                show_render->show();
                HWND showWnd = show_render->GetRenderWndID();
                globalToolManager->GetPaasSDK()->StartRenderStream(mCurMainShowUid.toStdWString(), showWnd, VHStreamType_AVCapture);
            }

            for (int i = 0; i < ui.listWidget_topView->count(); i++) {
                QListWidgetItem *item = ui.listWidget_topView->item(i);
                if (NULL != item) {
                    QWidget *w = ui.listWidget_topView->itemWidget(item);
                    if (NULL != w) {
                        VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                        if (itemWidget->isCurrentUser(mCurMainShowUid)) {
                            item->setHidden(false);
                            break;
                        }
                    }
                }
            }
        }
        mCurMainShowUid = user_id;
		ui.widget_mainView->show();
    }
    else {//当前大屏等于当前登录者
        VhallRenderWdg *render = GetRenderWnd(user_id);
        for (int i = 0; i < ui.listWidget_topView->count(); i++) {
            QListWidgetItem *item = ui.listWidget_topView->item(i);
            if (NULL != item) {
                QWidget *w = ui.listWidget_topView->itemWidget(item);
                if (NULL != w) {
                    VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                    if (itemWidget->isCurrentUser(user_id)) {
                        item->setHidden(true);
                        break;
                    }
                }
            }
        }

        HWND wnd = ui.widget_mainView->GetRenderWndID();
        ui.widget_mainView->InitUserInfo(user_id, true, user_id);
        ui.widget_mainView->SetUserCameraState(render->GetUserCameraState());
        ui.widget_mainView->SetUserMicState(render->GetUserMicState());
        if (render->GetUserCameraState()) {
            ui.widget_mainView->SetViewState(RenderView_None);
        }
        else {
            ui.widget_mainView->SetViewState(RenderView_NoCamera);
        }
        globalToolManager->GetPaasSDK()->StartRenderStream(user_id.toStdWString(), wnd, VHStreamType_AVCapture);
        mCurMainShowUid = user_id;
        ui.widget_mainView->show();
    }
}

void WebRtcLiveWdg::SetPlayDeskTopAndMediaFileState(bool playing) {
    if (playing) {
        if (ui.listWidget_topView->count() > 0) {
            for (int i = 0; i < ui.listWidget_topView->count(); i++) {
                QListWidgetItem *item = ui.listWidget_topView->item(i);
                if (NULL != item) {
                    QWidget *w = ui.listWidget_topView->itemWidget(item);
                    if (NULL != w) {
                        VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                        if (itemWidget) {
                            if (itemWidget->isCurrentUser(mCurMainShowUid)) {
                                item->setHidden(false);
                                HWND wnd = itemWidget->GetRenderWndID();
                                globalToolManager->GetPaasSDK()->StartRenderStream(itemWidget->GetUserID().toStdWString(), wnd, VHStreamType_AVCapture);
                                break;
                            }
                        }
                    }
                }
            }
            ui.widget_mainView->hide();
        }

        if (ui.listWidget_topView->count() > 0 && ui.widget_topRenderView->isHidden()) {
            ui.widget_topRenderView->show();
        }
    }
    else {
        if (ui.listWidget_topView->count() >= 1) {
            for (int i = 0; i < ui.listWidget_topView->count(); i++) {
                QListWidgetItem *item = ui.listWidget_topView->item(i);
                if (NULL != item) {
                    QWidget *w = ui.listWidget_topView->itemWidget(item);
                    if (NULL != w) {
                        VhallRenderWdg *itemWidget = dynamic_cast<VhallRenderWdg *>(w);
                        if (itemWidget->isCurrentUser(mCurMainShowUid)) {
                            item->setHidden(true);
                        }
                        else {
                            item->setHidden(false);
                        }
                    }
                }
            }

            VhallRenderWdg *render = GetRenderWnd(mCurMainShowUid);
            HWND wnd = ui.widget_mainView->GetRenderWndID();
            ui.widget_mainView->InitUserInfo(mCurMainShowUid, true, mCurMainShowUid);
            if (render) {
                ui.widget_mainView->SetUserCameraState(render->GetUserCameraState());
                ui.widget_mainView->SetUserMicState(render->GetUserMicState());
                if (render->GetUserCameraState()) {
                    ui.widget_mainView->SetViewState(RenderView_None);
                }
                else {
                    ui.widget_mainView->SetViewState(RenderView_NoCamera);
                }
            }
            globalToolManager->GetPaasSDK()->StartRenderStream(mCurMainShowUid.toStdWString(), wnd, VHStreamType_AVCapture);
            ui.widget_mainView->show();
            if (ui.listWidget_topView->count() == 1) {
                ui.widget_topRenderView->hide();
            }
        }
        else {
			ui.widget_mainView->hide();
        }
    }
}

int WebRtcLiveWdg::GetRenderSize() {
    return mRenderMap.size();
}

void WebRtcLiveWdg::HandleGetLocalStream(QEventStream* msgEvent)
{
    mbEnableStopStream = true;
    if (msgEvent)
    {
        QString strJoinId = globalToolManager->GetDataManager()->GetThridPartyUserId();
        if (!IsExistRenderWnd(strJoinId)) {
            bool bShowMain = false;
            bool bIsCameraOpen = globalToolManager->GetPaasSDK()->IsCameraOpen();
            bool bIsMicOpen = globalToolManager->GetPaasSDK()->IsMicOpen();
            AppendRenderUser(strJoinId, bIsMicOpen, bIsCameraOpen, bShowMain);
        }
        VhallRenderWdg *render = GetRenderWnd(strJoinId);
        if (render) {
            HWND wnd = render->GetRenderWndID();
            if (wnd) {
                render->show();
                if (mCurCameraDevID.isEmpty()) {
                }
                else {
                    render->SetViewState(RenderView_None);
                }
                QString user_id = globalToolManager->GetDataManager()->GetThridPartyUserId();
                globalToolManager->GetPaasSDK()->StartRenderStream(user_id.toStdWString(), wnd, VHStreamType_AVCapture);
                if (globalToolManager->GetDataManager()->GetIsLiving())
                {
                    globalToolManager->GetPaasSDK()->StartPushLocalStream();
					//StartPushStream();
                }
                
                SetMainWdgRenderCameraUser(user_id);

                if (mCurMainShowUid == globalToolManager->GetDataManager()->GetThridPartyUserId()) {
                    ui.widget_mainView->SetViewState(RenderView_None);
                }
            }
        }
    }
}


void WebRtcLiveWdg::CreateScreenShareWgd()
{
	if (NULL == m_pScreenShareToolWgd)
	{
		m_pScreenShareToolWgd = new CScreenShareToolWgd(this);
		if (m_pScreenShareToolWgd) {
			m_pScreenShareToolWgd->hide();
			m_pScreenShareToolWgd->setTencentSDK(false);
			//重置
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ClickedMicBtn, this, &WebRtcLiveWdg::slot_MicButtonClicked);
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ClickedStopShareBtn, this, &WebRtcLiveWdg::slot_BtnStopScreenClicked);
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ClickedCameraBtn, this, &WebRtcLiveWdg::slot_CameraBtnClicked);
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ClickedPlayerBtn, this, &WebRtcLiveWdg::slot_PlayerButtonClicked);
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ClickedStartLiveBtn, this, &WebRtcLiveWdg::slot_ScreenShareStartLive);
			connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_OnClickedSet, this, &WebRtcLiveWdg::slot_OnSetting);
			//connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::Sig_ChatClicked, this, &WebRtcLiveWdg::slotChatClicked);
		}
	}

}

void WebRtcLiveWdg::HandleGetDesktopStream(QEventStream* msgEvent)
{
    if (VHCapture::VHCapture_OK == msgEvent->mIVHCapture)
    {
        //m_bIsDesktopSharCapturing = true;
		mbIsDeskTop = true;

		CreateScreenShareWgd();
		this->hide();
		QRect rect = QApplication::desktop()->availableGeometry();
		if (m_pScreenShareToolWgd) {
			Permission per = globalToolManager->GetPaasSDK()->GetPermission();
			m_pScreenShareToolWgd->SetHostUser(per.publish_inav_stream);//怎么判断是否是主持
			m_pScreenShareToolWgd->move((rect.width() - m_pScreenShareToolWgd->width()) / 2, 0);
			m_pScreenShareToolWgd->show();
		}

        mbEnableStopDesktopStream = true;
        if (globalToolManager->GetDataManager()->GetIsLiving())//如果开始直播
        {
            globalToolManager->GetPaasSDK()->StartPushDesktopStream();
        }
    }
    else
    {
        //m_bIsDesktopSharCapturing = false;
    }

}

void WebRtcLiveWdg::HandleGetMediaFileStream(QEventStream* msgEvent)
{
    if (VHCapture::VHCapture_OK == msgEvent->mIVHCapture)
    {
        globalToolManager->GetDataManager()->WriteLog("%s\n", __FUNCTION__);
		SetPlayDeskTopAndMediaFileState(true);
        if (globalToolManager->GetPaasSDK()->IsSupportMediaFormat(CaptureStreamAVType_Video)) {
            ui.MediaFilePlayWdg->show();
            HWND id = (HWND)(ui.MediaFilePlayWdg->winId());
            QString user_id = globalToolManager->GetDataManager()->GetThridPartyUserId();
            globalToolManager->GetPaasSDK()->StartRenderStream(user_id.toStdWString(), id, VHStreamType_MediaFile);
            ui.MediaFilePlayWdg->setUpdatesEnabled(false);
            globalToolManager->GetDataManager()->WriteLog("%s mMediaStream play\n", __FUNCTION__);
        }
        else {
            ui.MediaFilePlayWdg->hide();
        }
        slot_ClickedMic(globalToolManager->GetDataManager()->GetThridPartyUserId(), true);
        if (globalToolManager->GetDataManager()->GetIsLiving()) {
            globalToolManager->GetPaasSDK()->StartPushMediaFileStream();
        }    
    }
    else
    {
        MediaFilePlayFaild();
    }
}

void WebRtcLiveWdg::MediaFilePlayFaild()
{
    FadeOutTip(QString::fromWCharArray(L"插播视频失败，请检测插播文件是否正确"));
    globalToolManager->GetDataManager()->WriteLog("%s CustomEvent_GetLocalFileStreamErr\n", __FUNCTION__);
    //关闭循环播放定时器
    m_qCycleTimer.stop();
    //隐藏UI
    ShowVedioPlayWidget(FALSE);
    ForceHide(true);
    m_bPlayCurrentFile = true;
    StopPlayMediaFile();
}

void WebRtcLiveWdg::HandleGetStream(QEvent* event) {
    if (event == NULL) {
        return;
    }
    QEventStream *msgEvent = (QEventStream*)event;

    if (msgEvent->mStreamType< VHStreamType_Desktop) {
        if (msgEvent->mIVHCapture != VHCapture_OK) {
            QString msg = QStringLiteral("设备打开失败,请检测当前采集设备");
            FadeOutTip(msg);
            VhallRenderWdg* render = GetRenderWnd(globalToolManager->GetDataManager()->GetThridPartyUserId());
            if (render) {
                render->SetViewState(RenderView_NoCamera);
            }
            if (mCurMainShowUid == globalToolManager->GetDataManager()->GetThridPartyUserId()) {
                ui.widget_mainView->SetViewState(RenderView_NoCamera);
            }
        }
        else {
            HandleGetLocalStream(msgEvent);
        }
    }
    else if (VHStreamType_Desktop == msgEvent->mStreamType) {
        if (msgEvent->mIVHCapture != VHCapture_OK) {
            QString msg = QStringLiteral("桌面采集失败");
            FadeOutTip(msg);
        }
        else {
            HandleGetDesktopStream(msgEvent);
        }
    }
    else if (VHStreamType_MediaFile == msgEvent->mStreamType) {
        if (msgEvent->mIVHCapture != VHCapture_OK) {
            QString msg = QStringLiteral("插播文件采集失败");
            FadeOutTip(msg);
        }
        else {
            HandleGetMediaFileStream(msgEvent);
        }
    }
}

VhallRenderWdg *WebRtcLiveWdg::GetRenderWnd(const QString& uid) {
    QMutexLocker l(&mRenderViewMutex);
	QMap<QString, VhallRenderWdg*>::iterator iter = mRenderMap.find(uid);
    if (iter != mRenderMap.end()) {
        return iter.value();
    }
    return NULL;
}

void WebRtcLiveWdg::SetPlayMeidaFileVolume(const int volume)
{
    //if (mMediaStream) {
    //	mMediaStream->SetAudioInVolume(volume);
    //}
}


int WebRtcLiveWdg::InitMediaStream(const char *szMediaFile) {
    QFile file(szMediaFile);
    if (!file.exists()) {
        //QCoreApplication::postEvent(mpMainUILogic, new QEvent(CustomEvent_GetLocalFileStreamErr));
        //return -1;
    }

    //globalToolManager->GetPaasSDK()->StartPlayMediaFile(szMediaFile);
    int iRef = globalToolManager->GetPaasSDK()->StartPlayMediaFile(szMediaFile);
    if (VhallLive_OK == iRef)
    {
        //globalToolManager->GetPaasSDK()->SetCurrentMicVol(iVolume);
    }
    else
    {
        FadeOutTip(QString::fromWCharArray(L"插播视频错误，错误码：%1").arg(iRef));
        return -1;
    }
    return 0;
}

int WebRtcLiveWdg::OpenPlayMediaFile(const char *szMediaFile) {
    return InitMediaStream(szMediaFile);
}

QString WebRtcLiveWdg::dulation2Str(long long currDulation, long long maxDulation) {
    QString dulationStr = "";
    if (currDulation > maxDulation) {
        currDulation = maxDulation;
    }

    dulationStr = dulation2StrPri(currDulation) + "/" + dulation2StrPri(maxDulation);
    return dulationStr;
}

//时间戳转换
QString WebRtcLiveWdg::dulation2StrPri(long long dulation) {
    dulation /= 1000;
    int H = (dulation / 3600) % 60;
    int M = (dulation / 60) % 60;
    int S = dulation % 60;

    QString sH = QString::number(H);
    if (H < 10) {
        sH = "0" + sH;
    }
    QString sM = QString::number(M);
    if (M < 10) {
        sM = "0" + sM;
    }
    QString sS = QString::number(S);
    if (S < 10) {
        sS = "0" + sS;
    }
    return QString("%1:%2:%3").arg(sH, sM, sS);
}

void WebRtcLiveWdg::PlaySelectedFile() {
    m_bPlayCurrentFile = true;
}



void WebRtcLiveWdg::slot_EnableSpeak()
{
    if (NULL != mpEnableSpeakTimer)
    {
        if (mpEnableSpeakTimer->isActive())
        {
            mpEnableSpeakTimer->stop();
        }
    }
    ui.widget_ctrl->SetToSpeakEnble(true);
}

void WebRtcLiveWdg::slot_MicVolumnChanged(int volume) {
    globalToolManager->GetPaasSDK()->SetCurrentMicVol(volume);
    ChangeLocalMicDevState(0 != volume, globalToolManager->GetDataManager()->GetThridPartyUserId());
}

void WebRtcLiveWdg::ChangeLocalMicDevState(bool open, const QString& uid) {
    globalToolManager->GetDataManager()->WriteLog("%s open:%d\n", __FUNCTION__, open);
    //mbIsMicOpen = open;

    if (open)
    {
        globalToolManager->GetPaasSDK()->OpenMic();
    }
    else
    {
        globalToolManager->GetPaasSDK()->OpenMic();
    }

    //if (mLocalStream) {
    //	mLocalStream->MuteAudio(!open); //设置开启状态，则本地应该为非静音
    //}
    ui.widget_ctrl->OnOpenMic(open);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetMicState(open);

    VhallRenderWdg* render = GetRenderWnd(uid);
    if (render) {
        render->SetUserMicState(open);
        SetMainViewMicState(uid, open);
    }
}

void WebRtcLiveWdg::slot_OnClickedCamera(QString id, bool curState)
{
    if (globalToolManager->GetDataManager()->GetThridPartyUserId() == id) {
        slot_CameraBtnClicked();
    }
    else if (globalToolManager->GetPaasSDK()->OperateRemoteUserVideo(id.toStdWString(), curState) == VhallLive_OK) {
        VhallRenderWdg* render = GetRenderWnd(id);
        if (render) {
            render->SetUserCameraState(!curState);
            SetMainViewCameraState(id, !curState);
        }
    }
}

void WebRtcLiveWdg::slot_setToSpeaker(QString user_id) {
    std::string id = globalToolManager->GetPaasSDK()->GetUserStreamID(user_id.toStdWString(), VHStreamType_AVCapture);
    if (id.length() > 0) {
        int nRet = globalToolManager->GetPaasSDK()->SetMainView(id);
        if (nRet == VhallLive_SERVER_NOT_READY) {
            FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
        }
    }
}

void WebRtcLiveWdg::slot_ClickedMic(QString id, bool curState)
{
    if (globalToolManager->GetDataManager()->GetThridPartyUserId() == id) {
        slot_MicButtonClicked();
    }
    else if (globalToolManager->GetPaasSDK()->OperateRemoteUserAudio(id.toStdWString(), curState) == VhallLive_OK) {
        VhallRenderWdg* render = GetRenderWnd(id);
        if (render) {
            render->SetUserMicState(!curState);
            SetMainViewMicState(id, !curState);
        }
    }
}

void WebRtcLiveWdg::SetMainViewMicState(const QString user_id, bool open) {
    if (user_id == mCurMainShowUid) {
        ui.widget_mainView->SetUserMicState(open);
    }
}

void WebRtcLiveWdg::SetMainViewCameraState(const QString userId, bool open) {
    if (userId == mCurMainShowUid) {
        ui.widget_mainView->SetUserCameraState(open);
    }
}

void WebRtcLiveWdg::slot_SetMainWdgRender(QString uid) {

    SetMainWdgRenderCameraUser(uid);
}

void WebRtcLiveWdg::slot_PublishInavAnotherClicked()
{
	//确认推流房间
	if (NULL == m_pPassRoomWdg)
	{
		m_pPassRoomWdg = new ByPassRoomWdg(this);
	}


	if (NULL != m_pPassRoomWdg && QDialog::Accepted == m_pPassRoomWdg->exec())
	{
		if (m_iPublishInavAnother)
		{
			QString live_room_id = globalToolManager->GetDataManager()->GetLiveRoomId();
			globalToolManager->GetPaasSDK()->StopPublishInavAnother(live_room_id.toStdString().c_str());
		}
		else
		{
			QString live_room_id = globalToolManager->GetDataManager()->GetLiveRoomId();
            int nRet = globalToolManager->GetPaasSDK()->StartPublishInavAnother(live_room_id.toStdString().c_str(), (LayoutMode)miLayoutMod, mLocalStreamId.toStdString().c_str());
            if (nRet == VhallLive_SERVER_NOT_READY) {
                FadeOutTip(QStringLiteral("服务器端混流未就绪,请稍后再试"));
                ui.widget_ctrl->setPubLishInavAnotherState(false);
            }
            else if (nRet != VhallLive_OK) {
                FadeOutTip(QStringLiteral("设置混流失败，请稍后再试"));
                ui.widget_ctrl->setPubLishInavAnotherState(false);
            }
		}
	}
	else
	{
		ui.widget_ctrl->EnablePlguinsBtn(true);
	}
}

void WebRtcLiveWdg::slo_ShowVedioPlayWidget()
{
	ShowVedioPlayWidget(TRUE);
}

void WebRtcLiveWdg::slotDealStopPlay()
{
	//关闭循环播放定时器
	m_qCycleTimer.stop();
	//隐藏UI
	ShowVedioPlayWidget(FALSE);
	ForceHide(true);
	m_bPlayCurrentFile = true;
	//m_bIsMediaPlaying = false;
	StopPlayMediaFile();
}

void WebRtcLiveWdg::on_pushButton_preViousView_clicked(bool checked /*= false*/)
{
	int iVale = ui.listWidget_topView->horizontalScrollBar()->value();
	ui.listWidget_topView->horizontalScrollBar()->setValue(iVale - 10);
}

void WebRtcLiveWdg::on_pushButton_nextView_clicked(bool checked /*= false*/)
{
	int iVale = ui.listWidget_topView->horizontalScrollBar()->value();
	ui.listWidget_topView->horizontalScrollBar()->setValue(iVale - 10);
}

void WebRtcLiveWdg::slot_move(QPoint point)
{
	if(m_pVedioPlayUI && m_pVedioPlayUI->isVisible())
		ReposVedioPlay(true);
}

void WebRtcLiveWdg::slot_PlayerVolumnChanged(int volume)
{

    globalToolManager->GetPaasSDK()->SetCurrentPlayVol(volume);
    ui.widget_ctrl->OnOpenPlayer(0 != volume);
	if (m_pScreenShareToolWgd)
		m_pScreenShareToolWgd->SetPlayerState(0 != volume);
}

void WebRtcLiveWdg::slot_OnClose()
{
	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCDNAME__);
	globalToolManager->GetPaasSDK()->LeaveVHMediaRoom();
	globalToolManager->GetPaasSDK()->LogoutHVRoom();
	m_bClose = true;
}

void WebRtcLiveWdg::slot_OnFresh() {

}

void WebRtcLiveWdg::slot_OnFullSize() {
    if (mIsFullSize) {
        showNormal();
        mIsFullSize = false;
    }
    else {
        showMaximized();
        mIsFullSize = true;
    }
	calVedioPlayUISize();
}

void WebRtcLiveWdg::slot_OnMinSize() {
    showMinimized();
}

void WebRtcLiveWdg::slot_OnSetting() {
    CreateSettingDlg();
    //globalToolManager->GetDataManager()->WriteLog("%s  ---------------------------------- \n", __FUNCTION__);
}

void WebRtcLiveWdg::slot_OnShare() {

}

bool WebRtcLiveWdg::IsPlayMediaFile()
{
	bool bRef = false;
	bRef = globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_MediaFile);
	globalToolManager->GetDataManager()->WriteLog("%s ：%d", __FUNCTION__, bRef);
	return bRef;
}

bool WebRtcLiveWdg::IsPlayDesktopShare() {
	
	bool bRef = false;
	bRef = globalToolManager->GetPaasSDK()->IsCapturingStream(VHStreamType_Desktop);
	globalToolManager->GetDataManager()->WriteLog("%s ：%d", __FUNCTION__, bRef);
	return bRef;
}

bool WebRtcLiveWdg::IsExistRenderWnd(const QString& uid) {
    QMutexLocker l(&mRenderViewMutex);
    QMap<QString, VhallRenderWdg*>::iterator iter = mRenderMap.find(uid);
    if (iter != mRenderMap.end()) {
        return true;
    }
    return false;
}

void WebRtcLiveWdg::SetSpeakUserOffLine(const QString &uid, bool offline)
{
    QMutexLocker l(&mSpeakUserMapMutex);
    std::map<QString, SpeakUser*>::iterator iter = mSpeakUserMap.find(uid);
    QString  strJoinId = globalToolManager->GetDataManager()->GetThridPartyUserId();

    if (iter != mSpeakUserMap.end() && strJoinId.compare(uid) != 0) {
        iter->second->bOffLine = offline;
        iter->second->offLineTime = QDateTime::currentMSecsSinceEpoch();
    }
}

void WebRtcLiveWdg::resizeEvent(QResizeEvent *event)
{
	CBaseWnd::resizeEvent(event);
	calVedioPlayUISize();
	
}

void WebRtcLiveWdg::moveEvent(QMoveEvent *event)
{
	//CBaseWnd::moveEvent(event);
	calVedioPlayUISize();	
}

void WebRtcLiveWdg::calVedioPlayUISize()
{
	if (NULL!=m_pVedioPlayUI)
	{
		QWidget* pContentUI = ui.bgWdg;
		QPoint pos = mapToGlobal(pContentUI->pos());
		int x = pos.x();
		int iHeight = pContentUI->height();
		int iUiHeight = m_pVedioPlayUI->height();
		m_pVedioPlayUI->setFixedWidth(pContentUI->width());
		int y = pos.y() + pContentUI->height() - m_pVedioPlayUI->height();
		globalToolManager->GetDataManager()->WriteLog("%s ：m_pVedioPlayUI->move(%d, %d) ", __FUNCTION__, x, y);
		m_pVedioPlayUI->move(x, y);
	}
}

void WebRtcLiveWdg::SetAgreePush(bool bAgreePush)
{
	m_AgreePush = bAgreePush;
}