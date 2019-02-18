#include "CInterActiveCtrlWdg.h"
//#include "CameraMgrUI.h"
#include "../Unility/Unility.h"
#include <QTimer> 

extern ToolManager *globalToolManager;

#define TO_SPEAK "QPushButton{color:rgb(255, 247, 247);border-image:url(:/sysButton/img/sysButton/okButton.png);}QPushButton:hover{border-image:url(:/sysButton/img/sysButton/okButtonOn.png);}QPushButton:pressed{border-image:url(:/sysButton/img/sysButton/okButton.png);}"

#define OFF_SPEAK "QPushButton{border-image:url(:/interactivity/offSpeakeHover);}\
QPushButton:hover{border-image:url(:/interactivity/offSpeake);}\
QPushButton:pressed{border-image:url(:/interactivity/offSpeakeHover);}\
QPushButton:disabled{border-image: url(:/interactivity/offSpeakeHover);}"

CInterActiveCtrlWdg::CInterActiveCtrlWdg(QWidget *parent)
: QWidget(parent){
   ui.setupUi(this);
   //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool | Qt::WindowStaysOnTopHint);
   InitBtnState(true);
   InitConnect();
   DisMouseTracking(); 
   ui.pushButton_cameraList->hide();
   ui.label->hide();
   mpTimer = new QTimer;
   connect(mpTimer, &QTimer::timeout, this, &CInterActiveCtrlWdg::slotEnabelStartLive);
}

CInterActiveCtrlWdg::~CInterActiveCtrlWdg() {

}

void CInterActiveCtrlWdg::InitBtnState(bool bHost) {
	ui.pushButton_Camera->changeImage(":/CtrlBtn/img/CtrlButton/camera_open.png");
	ui.pushButton_mic->changeImage(":/CtrlBtn/img/CtrlButton/mic_0.png");
	ui.pushButton_player->changeImage(":/CtrlBtn/img/CtrlButton/player_open.png");
	ui.pushButton_screenShare->changeImage(":/CtrlBtn/img/CtrlButton/desktop_share.png");
	ui.pushButton_cameraList->changeImage(":/CtrlBtn/img/CtrlButton/arrow.png");
	ui.pushButton_micList->changeImage(":/CtrlBtn/img/CtrlButton/arrow.png");
	ui.pushButton_playerList->changeImage(":/CtrlBtn/img/CtrlButton/arrow.png");
	ui.pushButton_startLive->setToolTip(QString::fromWCharArray(L"上麦"));
	ui.label->setText("00:00:00");
	ui.pushButton_playFile->changeImage(":/CtrlBtn/img/CtrlButton/insertMediaFile.png");
	SetEixtUnseeHandsUpList(false);
	ui.labMember->hide();

	//if (bHost) {
	//	ShowToSpeakBtn(false);
	//	ShowStartLiveBtn(true);

	//} else {
	//	ShowToSpeakBtn(true);
	//	ShowStartLiveBtn(false);
	//}
}

void CInterActiveCtrlWdg::InitConnect() {
	connect(ui.pushButton_Camera, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedCameraCtrlBtn()));
	connect(ui.pushButton_mic, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedMicCtrlBtn()));
	connect(ui.pushButton_player, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedPlayerCtrlBtn()));
	connect(ui.pushButton_screenShare, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedScreenShareCtrlBtn()));
	connect(ui.pushButton_startLive, SIGNAL(clicked()), this, SLOT(Slot_OnClickedStartLiveCtrlBtn()));
	connect(ui.pushButton_cameraList, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedShowCameraListBtn()));
	connect(ui.pushButton_micList, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedShowMicListBtn()));
	connect(ui.pushButton_playerList, SIGNAL(sigClicked()), this, SLOT(Slot_OnClickedShowPlayerBtn()));
	connect(ui.pushButton_mic, SIGNAL(sigEnter()), this, SLOT(Slot_OnMicBtnEnter()));
	connect(ui.pushButton_mic, SIGNAL(sigLeave()), this, SLOT(Slot_OnMicBtnLeave()));
	connect(ui.pushButton_player, SIGNAL(sigEnter()), this, SLOT(Slot_OnPlayerBtnEnter()));
	connect(ui.pushButton_player, SIGNAL(sigLeave()), this, SLOT(Slot_OnPlayerBtnLeave()));
	connect(ui.pushButton_playFile, SIGNAL(sigClicked()), this, SLOT(slotPlayFileClicked()));
	connect(ui.btnMemberList, SIGNAL(sigClicked()), this, SIGNAL(sigMemberListClicked()));
	//connect(ui.pushButton_toSpeak, SIGNAL(clicked()), this, SIGNAL(sigToSpeakClicked()));

	connect(ui.pushButton_toSpeak, &QPushButton::clicked, this, &CInterActiveCtrlWdg::sigToSpeakClicked);
	connect(ui.pbtnPublishInavAnother, &QPushButton::clicked, this, &CInterActiveCtrlWdg::slotPublishInavAnotherClicked/*&CInterActiveCtrlWdg::sigPublishInavAnotherClicked*/);
}

void CInterActiveCtrlWdg::SetHideDevList() {
	ui.pushButton_cameraList->hide();
	ui.pushButton_micList->hide();
	ui.pushButton_playerList->hide();
}

void CInterActiveCtrlWdg::ChageSpacerNomal(bool bNomal/* = false*/)
{
	if (bNomal)//正常
	{
		ui.hztLiveTool->changeSize(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
		ui.spacerPlayer->changeSize(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
		ui.pushButton_cameraList->hide();
		ui.pushButton_micList->hide();
	}
	else
	{
		ui.hztLiveTool->changeSize(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
		ui.spacerPlayer->changeSize(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
		ui.pushButton_cameraList->show();
		ui.pushButton_micList->show();
	}
	
}

void CInterActiveCtrlWdg::OnOpenCamera(bool bOpen) {
   if (bOpen) {
      ui.pushButton_Camera->changeImage(":/CtrlBtn/img/CtrlButton/camera_open.png");
   } else {
      ui.pushButton_Camera->changeImage(":/CtrlBtn/img/CtrlButton/camera_close.png");
   }
}

void CInterActiveCtrlWdg::SetBackGroundColor(QString styleSheet) {
	this->setStyleSheet(styleSheet);
}

//void CInterActiveCtrlWdg::ShowStartLiveBtn(bool show) {
//	//if (show) {
//	//	ui.pushButton_startLive->show();
//	//}
//	//else {
//	//	ui.pushButton_startLive->hide();
//	//}
//}

//void CInterActiveCtrlWdg::ShowToSpeakBtn(bool show) {
//	//if (show) {
//	//	ui.pushButton_toSpeak->show();
//	//}
//	//else {
//	//	ui.pushButton_toSpeak->hide();
//	//}
//}	

void CInterActiveCtrlWdg::SetToSpeakEnble(bool bEnble /*= true*/)
{
	ui.pushButton_toSpeak->setEnabled(bEnble);
}

void CInterActiveCtrlWdg::ShowPublishInavAnother(bool bShow)
{
	bShow ? ui.pbtnPublishInavAnother->show(): ui.pbtnPublishInavAnother->hide();
}



//void CInterActiveCtrlWdg::ShowToSpeak(bool bShow)
//{
//	bShow ? ui.pushButton_toSpeak->show() : ui.pushButton_toSpeak->hide();
//}


void CInterActiveCtrlWdg::EnablePlguinsBtn(bool enable)
{
	ui.pbtnPublishInavAnother->setEnabled(enable);
}

void CInterActiveCtrlWdg::setPubLishInavAnotherState(bool bCurState)
{
	if (bCurState)//开启旁路直播成功
	{
		ui.pbtnPublishInavAnother->setText(QStringLiteral("关闭旁路直播"));
	}
	else//关闭旁路直播成功
	{
		ui.pbtnPublishInavAnother->setText(QStringLiteral("开启旁路直播"));
	}
	ui.pbtnPublishInavAnother->setEnabled(true);
}

//void CInterActiveCtrlWdg::CreateMgrUI()
//{
//	if (NULL == m_pCameraMgrUI)
//	{
//		m_pCameraMgrUI = new CameraMgrUI(this);
//		if (NULL == m_pCameraMgrUI || !m_pCameraMgrUI->Create()) {
//			//ASSERT(FALSE);
//			return ;
//		}
//		//m_pCameraMgrUI->InitCameraMgrUI();
//	}
//}

//bool CInterActiveCtrlWdg::InitCameraMgrUI(std::list<VhallLiveDeviceInfo> &micDevList)
//{
//	CreateMgrUI();
//	if (NULL== m_pCameraMgrUI)
//	{
//		m_pCameraMgrUI->ClearItem();
//		//m_pCameraModifyUI->Clear();
//		m_pCameraMgrUI->resize(m_pCameraMgrUI->width(), 100);
//		//m_pCameraList->Clear();
//
//
//		//for (int i = 0; i < micDevList.size(); i++) {
//	    for (std::list< VhallLiveDeviceInfo >::iteratorit = micDevList.begin(); it != micDevList.end(); ++it)
//			CameraItem* item = new CameraItem(m_pCameraMgrUI);
//			if (!item) {
//				//ASSERT(FALSE);
//				return false;
//			}
//
//			item->SetCameraDevice(*it);
//			item->SetItemIndex(it->);
//			m_pCameraMgrUI->AddItem(item);
//			//m_pCameraModifyUI->AddItem(micDevList[i]);
//			//m_pCameraList->AddItem(QString::fromWCharArray(micDevList[i].m_sDeviceDisPlayName), micDevList[i], i);
//		}
//
//		//设备不存在文本显示
//		//m_pCameraMgrUI->ShowNoDeivice(0 == m_CameraItemMap.size() ? true : false);
//		m_pCameraMgrUI->ShowNoDeivice(0 == micDevList.size() ? true : false);
//	}
//
//}

void CInterActiveCtrlWdg::intShowStartLive(bool bShow)
{
	if (bShow) {
		ui.pushButton_startLive->show();
	}
	else {
		ui.pushButton_startLive->hide();
	}
}

void CInterActiveCtrlWdg::initToSpeakBtnState(bool toSpeak)
{
	if (toSpeak) {
		ui.pushButton_toSpeak->show();
		ui.pushButton_startLive->hide();
	}
	else {
		ui.pushButton_toSpeak->hide();
	}
}

void CInterActiveCtrlWdg::SetToSpeakBtnState(bool toSpeak) {
	if (toSpeak) {//true  上麦
		ui.pushButton_toSpeak->hide();
		ui.pushButton_startLive->show();
	}
	else {
		ui.pushButton_startLive->hide();
		ui.pushButton_toSpeak->show();
	}
   this->update();
}

//bool CInterActiveCtrlWdg::GetToSpeakBtnState() {
//   return mbIsSpeaking;
//}

void CInterActiveCtrlWdg::OnOpenMic(bool bOpen) {
   if (bOpen) {
      ui.pushButton_mic->changeImage(":/CtrlBtn/img/CtrlButton/mic_0.png");
   } else {
      ui.pushButton_mic->changeImage(":/CtrlBtn/img/CtrlButton/mic_close.png");
   }
   this->update();
}

void CInterActiveCtrlWdg::OnOpenPlayer(bool bOpen) {
   if (bOpen) {
      ui.pushButton_player->changeImage(":/CtrlBtn/img/CtrlButton/player_open.png");
   } else {
      ui.pushButton_player->changeImage(":/CtrlBtn/img/CtrlButton/player_close.png");
   }
   this->update();
}

void CInterActiveCtrlWdg::OnOpenScreenShare(bool bOpen) {
   if (bOpen) {
      ui.pushButton_screenShare->changeImage(":/CtrlBtn/img/CtrlButton/desktop_share.png");
   } else {
      ui.pushButton_screenShare->changeImage(":/CtrlBtn/img/CtrlButton/desktop_share.png");
   }
}

void CInterActiveCtrlWdg::OnStartLive(bool start) {
   if (start) {
       ui.pushButton_startLive->setText(QStringLiteral("下麦"));
   } else {
       emit SigStopLive();
       ui.pushButton_startLive->setText(QStringLiteral("上麦"));
	   globalToolManager->GetDataManager()->SetIsLiving(false);
   }
   ui.pushButton_startLive->setEnabled(false);
   globalToolManager->GetPaasSDK()->ReFreshPermission();
   ui.pushButton_startLive->setEnabled(true);
}

void CInterActiveCtrlWdg::Slot_OnClickedCameraCtrlBtn() {

	QPoint globalPoint = ui.pushButton_Camera->mapToGlobal(QPoint(0, 0));
	int iglobalX = globalPoint.x() + ui.pushButton_Camera->width() / 2;
	int iglobalY = globalPoint.y();

   emit Sig_OnClickedCameraCtrlBtn();
}

void CInterActiveCtrlWdg::Slot_OnClickedMicCtrlBtn() {
   emit Sig_OnClickedMicCtrlBtn();
}

void CInterActiveCtrlWdg::Slot_OnClickedPlayerCtrlBtn() {
   emit Sig_OnClickedPlayerCtrlBtn();
}

void CInterActiveCtrlWdg::Slot_OnClickedScreenShareCtrlBtn() {

   emit Sig_OnClickedScreenShareCtrlBtn();

}

void CInterActiveCtrlWdg::slotEnabelStartLive(){
	mpTimer->stop();
	ui.pushButton_startLive->setEnabled(true);
}

void CInterActiveCtrlWdg::Slot_OnClickedStartLiveCtrlBtn() {
	ui.pushButton_startLive->setEnabled(false);
	//ui.pushButton_startLive->setEnabled(false);
	mpTimer->start(3000);
   emit Sig_OnClickedStartLiveCtrlBtn();
}

//void CInterActiveCtrlWdg::Slot_OnClickedLeaveActiveBtn() {
//   emit Sig_OnClickedLeaveActiveBtn();
//}


void CInterActiveCtrlWdg::Slot_OnClickedShowCameraListBtn() {
	QPoint globalPoint = ui.pushButton_cameraList->mapToGlobal(QPoint(0, 0));
	int  iGlobalX = globalPoint.x() + ui.pushButton_cameraList->width() / 2;
	int  iGlobalY = globalPoint.y();

   emit Sig_OnClickedShowCameraListBtn();
}

void CInterActiveCtrlWdg::Slot_OnClickedShowMicListBtn() {
   emit Sig_OnClickedShowMicListBtn();
}

void CInterActiveCtrlWdg::Slot_OnClickedShowPlayerBtn() {
   emit Sig_OnClickedShowPlayerBtn();
}

void CInterActiveCtrlWdg::Slot_OnMicBtnEnter() {
   emit Sig_OnMicBtnEnter();
}
void CInterActiveCtrlWdg::Slot_OnMicBtnLeave() {
   emit Sig_OnMicBtnLeave();
}
void CInterActiveCtrlWdg::Slot_OnPlayerBtnEnter() {
   emit Sig_OnPlayerBtnEnter();
}
void CInterActiveCtrlWdg::Slot_OnPlayerBtnLeave() {
   emit Sig_OnPlayerBtnLeave();
}

void CInterActiveCtrlWdg::OnSetLiveTime(const QString& time) {
   ui.label->setText(time);
}

QString CInterActiveCtrlWdg::GetLiveTime() {
   return ui.label->text();
}

QPoint CInterActiveCtrlWdg::GetMicBtnGlobalPos() {
   return ui.pushButton_mic->mapToGlobal(QPoint(15, 0));
}

QPoint CInterActiveCtrlWdg::GetPlayBtnGlobalPos() {
   return ui.pushButton_player->mapToGlobal(QPoint(18, 0));
}

QPoint CInterActiveCtrlWdg::GetMicListBtnGlobalPos() {
   return ui.pushButton_micList->mapToGlobal(QPoint(0, 0));
}
QPoint CInterActiveCtrlWdg::GetPlayListBtnGlobalPos() {
   return ui.pushButton_playerList->mapToGlobal(QPoint(0, 0));
}
QPoint CInterActiveCtrlWdg::GetCameraListBtnGlobalPos() {
   return ui.pushButton_cameraList->mapToGlobal(QPoint(0, 0));
}

int CInterActiveCtrlWdg::GetCameraListBtnWidth() {
   return ui.pushButton_cameraList->width();
}

int CInterActiveCtrlWdg::GetMicListBtnWidth() {
   return ui.pushButton_micList->width();
}

int CInterActiveCtrlWdg::GetPlayListBtnWidth() {
   return ui.pushButton_playerList->width();
}

void CInterActiveCtrlWdg::SetMicListBtnEnable(bool bEnable) {
   ui.pushButton_micList->setEnabled(bEnable);
}

void CInterActiveCtrlWdg::ShowTop(bool top) {
   if (top) {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool | Qt::WindowStaysOnTopHint);
   } else {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
   }
}

void CInterActiveCtrlWdg::mouseMoveEvent(QMouseEvent *) {
   if (m_bIsFull) {
      emit Sig_MouseEnterCtrlWdg();
   }
}

void CInterActiveCtrlWdg::SetFullState(bool isFull) {
   m_bIsFull = isFull;
   if (m_bIsFull){
      EnableMouseTracking();
   }
   else {
      DisMouseTracking();
   }
}

void CInterActiveCtrlWdg::DisMouseTracking() {
   this->setMouseTracking(false);
   ui.label->setMouseTracking(false);
   ui.pushButton_Camera->setMouseTracking(false);
   ui.pushButton_cameraList->setMouseTracking(false);
   //ui.pushButton_LeaveActive->setMouseTracking(false);
   ui.pushButton_mic->setMouseTracking(false);
   ui.pushButton_micList->setMouseTracking(false);
   ui.pushButton_player->setMouseTracking(false);
   ui.pushButton_playerList->setMouseTracking(false);
   ui.pushButton_startLive->setMouseTracking(false);
   ui.widget_2->setMouseTracking(false);
   ui.widget_3->setMouseTracking(false);
   ui.widget_4->setMouseTracking(false);
   ui.widget->setMouseTracking(false);
   ui.widget_cameraList->setMouseTracking(false);
   ui.widget_micList->setMouseTracking(false);
   ui.widget_playerList->setMouseTracking(false);
}

void CInterActiveCtrlWdg::EnableMouseTracking() {
   this->setMouseTracking(true);
   ui.label->setMouseTracking(true);
   ui.pushButton_Camera->setMouseTracking(true);
   ui.pushButton_cameraList->setMouseTracking(true);
   //ui.pushButton_LeaveActive->setMouseTracking(true);
   ui.pushButton_mic->setMouseTracking(true);
   ui.pushButton_micList->setMouseTracking(true);
   ui.pushButton_player->setMouseTracking(true);
   ui.pushButton_playerList->setMouseTracking(true);
   ui.pushButton_startLive->setMouseTracking(true);
   ui.widget_2->setMouseTracking(true);
   ui.widget_3->setMouseTracking(true);
   ui.widget_4->setMouseTracking(true);
   ui.widget->setMouseTracking(true);
   ui.widget_cameraList->setMouseTracking(true);
   ui.widget_micList->setMouseTracking(true);
   ui.widget_playerList->setMouseTracking(true);
}

void CInterActiveCtrlWdg::Slot_LiveToolClicked()
{
	//STRU_MAINUI_CLICK_CONTROL loControl;
	//loControl.m_eType = control_LiveTool;
	//SingletonMainUIIns::Instance().PostCRMessage(
	//	MSG_MAINUI_CLICK_CONTROL, &loControl, sizeof(STRU_MAINUI_CLICK_CONTROL));

	//QJsonObject body;
	//SingletonMainUIIns::Instance().reportLog(L"livepluginunit", eLogRePortK_LivePlugInUnit, body);
}

void CInterActiveCtrlWdg::ShowMemberListBtn()
{
	ui.btnMemberList->show();
	ui.labMember->show();
}

void CInterActiveCtrlWdg::SetEnableToSpeakBtn(bool enable) {
   ui.pushButton_toSpeak->setEnabled(enable);
   ui.pushButton_toSpeak->update();
}

void CInterActiveCtrlWdg::EnableStartLive(bool enable) {
   ui.pushButton_startLive->setEnabled(enable);
}


void CInterActiveCtrlWdg::SetEixtUnseeHandsUpList(bool bExit)
{
	if (bExit){
		ui.btnMemberList->changeImage(":/CtrlBtn/img/CtrlButton/btnmemberWithHU.png");
	}
	else{
		ui.btnMemberList->changeImage(":/CtrlBtn/img/CtrlButton/btnmember.png");
	}
}

void CInterActiveCtrlWdg::slotPlayFileClicked() {
   emit sig_OnPlayFileClicked();
}

void CInterActiveCtrlWdg::slotPublishInavAnotherClicked()
{
	ui.pbtnPublishInavAnother->setEnabled(false);
	emit sigPublishInavAnotherClicked();
}

//void CInterActiveCtrlWdg::slotToSpeakClicked()
//{
//	emit sigToSpeakClicked(mbIsSpeaking);
//}