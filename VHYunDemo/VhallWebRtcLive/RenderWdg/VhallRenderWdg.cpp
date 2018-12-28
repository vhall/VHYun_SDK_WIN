#include "VhallRenderWdg.h"
#include "VhallUI_define.h"
#include <QDebug>

VhallRenderWdg::VhallRenderWdg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mpMemberCtrlWdg = new MemberCtrlMaskWdg(this);
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->hide();
		connect(mpMemberCtrlWdg, SIGNAL(sig_clickedCamera(bool)), this, SLOT(slot_OnClickedCamera(bool)));
		connect(mpMemberCtrlWdg, SIGNAL(sig_clickedMic(bool)), this, SLOT(slot_OnClickedMic(bool)));
		connect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetMainView()), this, SLOT(slot_OnClickedSetMainView()));
		connect(mpMemberCtrlWdg, SIGNAL(sig_clickedKickOffRoom()), this, SLOT(slot_OnClickedKickOffRoom()));
		connect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetToSpeaker()), this, SLOT(slot_OnClickedSetToSpeaker()));
	}
   mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
   if (mpVhallRenderMaskWdg) {
	   mpVhallRenderMaskWdg->SetResetSize(true);
       mpVhallRenderMaskWdg->hide();
   }
   ui.widget_2->hide();
   ui.widget_rendView->hide();
   SetEnableUpdateRendWnd(false);
}

VhallRenderWdg::~VhallRenderWdg()
{
   if (mpVhallRenderMaskWdg) {
      delete mpVhallRenderMaskWdg;
      mpVhallRenderMaskWdg = NULL;
   }

   if (mpMemberCtrlWdg) {
      disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedCamera(bool)), this, SLOT(slot_OnClickedCamera(bool)));
      disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedMic(bool)), this, SLOT(slot_OnClickedMic(bool)));
      disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetMainView()), this, SLOT(slot_OnClickedSetMainView()));
      disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedKickOffRoom()), this, SLOT(slot_OnClickedKickOffRoom()));
      disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetToSpeaker()), this, SLOT(slot_OnClickedSetToSpeaker()));
	  delete mpMemberCtrlWdg;
	  mpMemberCtrlWdg = NULL;
   }
}

bool VhallRenderWdg::isCurrentUser(const QString& uid)
{
	return mRenderUserId.compare(uid) == 0;
}

void VhallRenderWdg::InitUserInfo(const QString& userId, bool bShowInMainView, const QString& loginUid) {
	mLoginUid = loginUid;
	mIsMainView = bShowInMainView;
	mRenderUserId = userId;
	//当前登录用户是主持人，根据上麦用户角色判断是否显示对应设置图标。
	if (mpMemberCtrlWdg) {
        mpMemberCtrlWdg->SetCurrentUserRole(Host_InListView);
	}
    if (mpVhallRenderMaskWdg) {
        mpVhallRenderMaskWdg->SetUserName(userId);
    }
}

void VhallRenderWdg::SetEnableUpdateRendWnd(bool enable) {
   ui.widget_rendView->setUpdatesEnabled(enable);
}

HWND VhallRenderWdg::GetRenderWndID() {
	HWND id = (HWND)(ui.widget_rendView->winId());
	return id;
}

void VhallRenderWdg::SetViewState(int state) {
	switch (state) {
	    case RenderView_NoJoin: {
            ui.widget_rendView->hide();
            ui.widget_ctrl->show();
		    ui.label_viewState->setStyleSheet("border-image: url(:/vhallActive/img/vhallactive/nojoin_2.png);");
		    ui.label_viewState->show();
            ui.label_camera->hide();
		    break;
	    }
	    case RenderView_NoCamera: {
            ui.widget_rendView->hide();
            ui.widget_ctrl->show();
		    ui.label_camera->setStyleSheet("border-image: url(:/vhallActive/img/vhallactive/closeCamera_2.png);");
		    ui.label_camera->show(); 
            ui.label_viewState->hide();
		    break;
	    }
       default: {
            ui.widget_rendView->show();
            ui.widget_ctrl->hide();
            break;
       }
	}
	mnStateType = state;
}

void VhallRenderWdg::ResetViewSize(bool isHostView) {
 
}

void VhallRenderWdg::SetUserCameraState(bool open) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->SetCameraState(open);
		if (open) {
			SetViewState(RenderView_None);
		}
		else {
			SetViewState(RenderView_NoCamera);
		}
        mbCameraOpen = open;
	}
}

bool VhallRenderWdg::GetUserCameraState() {
   return mbCameraOpen;
}

bool VhallRenderWdg::GetUserMicState() {
   return mbMicOpen;
}

void VhallRenderWdg::SetUserMicState(bool open) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->SetMicState(open);
        mbMicOpen = open;      
	}
}

void VhallRenderWdg::ShowSetSpeakBtn(bool show) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->ShowMainSpeaker(show);
	}
}
void VhallRenderWdg::ShowSetKickOutBtn(bool show) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->ShowToKickOutRoom(show);
	}
}

QString VhallRenderWdg::GetUserID() {
	return mRenderUserId;
}


void VhallRenderWdg::enterEvent(QEvent *event) {
	if (mpMemberCtrlWdg ) {
      ui.widget_rendView->hide();
      ui.widget_ctrl->show();
	  mpMemberCtrlWdg->setFixedSize(this->width(), this->height());
	  mpMemberCtrlWdg->move(0, 0);
	  mpMemberCtrlWdg->show();
      ui.label_viewState->hide();
      ui.label_camera->hide();
	}
   if (mpVhallRenderMaskWdg) {
      mpVhallRenderMaskWdg->show();
      mpVhallRenderMaskWdg->move(0, this->height() - mpVhallRenderMaskWdg->height());
   }
	QWidget::enterEvent(event);
}

void VhallRenderWdg::leaveEvent(QEvent *event) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->hide();
		if (mnStateType == RenderView_NoJoin) {
         ui.label_viewState->show();
         ui.label_camera->hide();
		}
      else if(mnStateType == RenderView_NoCamera){
         ui.label_viewState->hide();
         ui.label_camera->show();
      }
	}

   if (mnStateType != RenderView_NoJoin && mnStateType != RenderView_NoCamera) {
      ui.widget_rendView->show();
      ui.widget_ctrl->hide();
   }
   if (mpVhallRenderMaskWdg) {
      mpVhallRenderMaskWdg->hide();
   }
	QWidget::enterEvent(event);
}

void VhallRenderWdg::slot_OnClickedCamera(bool curState) {
	emit sig_ClickedCamera(mRenderUserId, curState);
}

void VhallRenderWdg::slot_OnClickedMic(bool curState) {
	emit sig_ClickedMic(mRenderUserId, curState);
}

void VhallRenderWdg::slot_OnClickedSetMainView() {
	emit sig_setToSpeaker(mRenderUserId);
}

void VhallRenderWdg::slot_OnClickedKickOffRoom() {
	emit sig_NotToSpeak(mRenderUserId);
}

void VhallRenderWdg::slot_OnClickedSetToSpeaker() {
	emit sig_setInMainView(mRenderUserId);
}


