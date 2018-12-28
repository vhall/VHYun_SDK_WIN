#include "MemberCtrlMaskWdg.h"
#include "VhallUI_define.h"

MemberCtrlMaskWdg::MemberCtrlMaskWdg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	connect(ui.pushButton_camera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
	connect(ui.pushButton_mainView, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetMainView()));
	connect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
	connect(ui.pushButton_setSpeaker, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetToSpeaker()));
	connect(ui.pushButton_KickOutRoom, SIGNAL(clicked()), this, SLOT(slot_OnClickedKickOffRoom()));

   connect(ui.pushButton_camera, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedCamerafocusIn()));
   connect(ui.pushButton_mainView, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedSetMainViewfocusIn()));
   connect(ui.pushButton_mic, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedMicfocusIn()));
   connect(ui.pushButton_setSpeaker, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedSetToSpeakerfocusIn()));
   connect(ui.pushButton_KickOutRoom, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedKickOffRoomfocusIn()));

   connect(ui.pushButton_camera, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedCamerafocusOut()));
   connect(ui.pushButton_mainView, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedSetMainViewfocusOut()));
   connect(ui.pushButton_mic, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedMicfocusOut()));
   connect(ui.pushButton_setSpeaker, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedSetToSpeakerfocusOut()));
   connect(ui.pushButton_KickOutRoom, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedKickOffRoomfocusOut()));

	mpTips = new CToolTips(this);
	if (mpTips) {
		mpTips->hide();
		mpTips->setSuspension();
	}
	ui.pushButton_mainView->hide();
	setAttribute(Qt::WA_TranslucentBackground, true);
}

MemberCtrlMaskWdg::~MemberCtrlMaskWdg()
{
   disconnect(ui.pushButton_camera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
   disconnect(ui.pushButton_mainView, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetMainView()));
   disconnect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
   disconnect(ui.pushButton_setSpeaker, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetToSpeaker()));
   disconnect(ui.pushButton_KickOutRoom, SIGNAL(clicked()), this, SLOT(slot_OnClickedKickOffRoom()));

   disconnect(ui.pushButton_camera, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedCamerafocusIn()));
   disconnect(ui.pushButton_mainView, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedSetMainViewfocusIn()));
   disconnect(ui.pushButton_mic, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedMicfocusIn()));
   disconnect(ui.pushButton_setSpeaker, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedSetToSpeakerfocusIn()));
   disconnect(ui.pushButton_KickOutRoom, SIGNAL(sig_Enter()), this, SLOT(slot_OnClickedKickOffRoomfocusIn()));

   disconnect(ui.pushButton_camera, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedCamerafocusOut()));
   disconnect(ui.pushButton_mainView, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedSetMainViewfocusOut()));
   disconnect(ui.pushButton_mic, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedMicfocusOut()));
   disconnect(ui.pushButton_setSpeaker, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedSetToSpeakerfocusOut()));
   disconnect(ui.pushButton_KickOutRoom, SIGNAL(sig_Leave()), this, SLOT(slot_OnClickedKickOffRoomfocusOut()));
   if (mpTips) {
      delete mpTips;
      mpTips = NULL;
   }
}

void MemberCtrlMaskWdg::SetCurrentUserRole(int state) {
	switch (state)
	{
	case Host_InMainView: {
		ShowToKickOutRoom(false);
		break;
	}
	case Host_InListView: {
		ShowToKickOutRoom(false);
		break;
	}
	case Guest_InMainView: {
		ShowToKickOutRoom(true);
		break;
	}
	case Guest_InListView: {
		ShowToKickOutRoom(true);
		break;
	}
	case Other_InMainView: {
		ShowToKickOutRoom(true);
		break;
	}
	case Other_InListView: {
		ShowToKickOutRoom(true);
		break;
	}
	default:
		break;
	}
}


void MemberCtrlMaskWdg::ShowMainSpeaker(bool show) {
	if (show) {
		ui.pushButton_setSpeaker->show();
	}
	else {
		ui.pushButton_setSpeaker->hide();
	}
}

//设置是否显示下麦
void MemberCtrlMaskWdg::ShowToKickOutRoom(bool show) {
	if (show) {
		ui.pushButton_KickOutRoom->show();
	}
	else {
		ui.pushButton_KickOutRoom->hide();
	}
}

//设置麦克风状态
void MemberCtrlMaskWdg::SetMicState(bool open) {
	if (open) {
		ui.pushButton_mic->setStyleSheet(MIC_OPEN_STATE_CSS);
	}
	else {
		ui.pushButton_mic->setStyleSheet(MIC_CLOSE_STATE_CSS);
	}
	mbMicOpen = open;
}

//设置摄像头状态
void MemberCtrlMaskWdg::SetCameraState(bool open) {
	if (open) {
		ui.pushButton_camera->setStyleSheet(CAMERA_OPEN_STATE_CSS);
	}
	else {
		ui.pushButton_camera->setStyleSheet(CAMERA_CLOSE_STATE_CSS);
	}
	mbCameraOpen = open;
}

void MemberCtrlMaskWdg::SetSpeakState(bool kickout) {
	if (kickout) {
		ui.pushButton_KickOutRoom->setStyleSheet(USER_KICKOUT_STATE_CSS);
	}
	else {
		ui.pushButton_KickOutRoom->setStyleSheet(USER_TOSPEAK_STATE_CSS);
	}
}

void MemberCtrlMaskWdg::hide()
{
	mpTips->hide();
	QWidget::hide();
}

void MemberCtrlMaskWdg::slot_OnClickedCamera() {
	emit sig_clickedCamera(mbCameraOpen);
}

void MemberCtrlMaskWdg::slot_OnClickedMic() {
	emit sig_clickedMic(mbMicOpen);
}

void MemberCtrlMaskWdg::slot_OnClickedSetMainView() {
	emit sig_clickedSetMainView();
}

void MemberCtrlMaskWdg::slot_OnClickedKickOffRoom() {
	emit sig_clickedKickOffRoom();
}

void MemberCtrlMaskWdg::slot_OnClickedSetToSpeaker() {
	emit sig_clickedSetToSpeaker();
}

void MemberCtrlMaskWdg::slot_OnClickedCamerafocusIn() {
   OnEnteCamera();
}
void MemberCtrlMaskWdg::slot_OnClickedSetMainViewfocusIn() {
   OnEnteMainView();
}
void MemberCtrlMaskWdg::slot_OnClickedMicfocusIn() {
   OnEnteMic();
}
void MemberCtrlMaskWdg::slot_OnClickedSetToSpeakerfocusIn() {
   OnEnteToSpeaker();
}
void MemberCtrlMaskWdg::slot_OnClickedKickOffRoomfocusIn() {
   OnEnteKickOffRoom();
}

void MemberCtrlMaskWdg::slot_OnClickedCamerafocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedSetMainViewfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedMicfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedSetToSpeakerfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedKickOffRoomfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}

void MemberCtrlMaskWdg::leaveEvent(QEvent *event) {
	hide();
	QWidget::leaveEvent(event);
}

void MemberCtrlMaskWdg::focusOutEvent(QFocusEvent *event) {
	hide();
	QWidget::focusOutEvent(event);
}

void MemberCtrlMaskWdg::OnEnteCamera() {
	if (mpTips) {
		QPoint pos = ui.pushButton_camera->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint(( ui.pushButton_camera->width() - mpTips->width()) / 2, ui.pushButton_camera->height() + 4));
		if (mbCameraOpen) {
			mpTips->SetText(QString::fromWCharArray(L"关闭摄像头"));
		}
		else {
			mpTips->SetText(QString::fromWCharArray(L"打开摄像头"));
		}
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteMainView() {
	if (mpTips) {
		QPoint pos = ui.pushButton_mainView->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_mainView->width() - mpTips->width()) / 2, ui.pushButton_mainView->height() + 4));
		mpTips->SetText(QString::fromWCharArray(L"设为主画面"));
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteMic() {
	if (mpTips) {
		QPoint pos = ui.pushButton_mic->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_mic->width() - mpTips->width()) / 2, ui.pushButton_mic->height() + 4));
		if (mbMicOpen) {
			mpTips->SetText(QString::fromWCharArray(L"关闭麦克风"));
		}
		else {
			mpTips->SetText(QString::fromWCharArray(L"打开麦克风"));
		}
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteToSpeaker() {
	if (mpTips) {
		QPoint pos = ui.pushButton_setSpeaker->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_setSpeaker->width() - mpTips->width()) / 2, ui.pushButton_setSpeaker->height() + 4));
		mpTips->SetText(QString::fromWCharArray(L"画面切换"));
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteKickOffRoom() {
	if (mpTips) {
		QPoint pos = ui.pushButton_KickOutRoom->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_KickOutRoom->width() - mpTips->width()) / 2, ui.pushButton_KickOutRoom->height() + 4));
		mpTips->SetText(QString::fromWCharArray(L"下麦"));
		mpTips->show();
	}
}

