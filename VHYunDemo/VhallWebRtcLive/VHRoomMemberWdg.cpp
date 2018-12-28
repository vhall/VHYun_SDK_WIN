#include "VHRoomMemberWdg.h"
#include "callback_monitor.h"
#include "../Unility/Unility.h"
#include "VhallWebRtcLive.h"

VHRoomMemberWdg::VHRoomMemberWdg(MEMBER_LIST type,std::string userId, int status, QWidget *parent)
    : QWidget(parent)
    , mMemberType(type)
    , mUserID(QString::fromStdString(userId))
    , mStatus(0)
{
    ui.setupUi(this);
    ui.label_user_id->setText(mUserID);
	//if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) == 0) {//当前登陆的是本人
		ui.pushButton_invite->hide();
		ui.pushButton_kickout->hide();
		ui.pushButton_cancelKickOut->hide();
		ui.pushButton_StopActive->hide();
	//}
    SetUserStatus(status);
    connect(ui.pushButton_invite, SIGNAL(clicked()), this, SLOT(slot_ProcessOnLineUser()));
    connect(ui.pushButton_kickout, SIGNAL(clicked()), this, SLOT(slot_KickOutUser()));
    connect(ui.pushButton_cancelKickOut, SIGNAL(clicked()), this, SLOT(slot_CancelKickout()));
    connect(ui.pushButton_StopActive, SIGNAL(clicked()), this, SLOT(slot_StopActive()));
}

VHRoomMemberWdg::~VHRoomMemberWdg()
{
}

void VHRoomMemberWdg::SetUserStatus(int status) {
	if (mStatus == status) return;
    globalToolManager->GetDataManager()->WriteLog("%s status:%d", __FUNCTION__, status);
    EnableAllButton();
    mStatus = status;
	Permission per = globalToolManager->GetPaasSDK()->GetPermission();

    //用户状态 1 推流中 2 观看中 3 受邀中 4 申请中 5 被踢出
    switch (mStatus)
    {
    case MemberStatus_Pushing: {//1 推流中
        ui.label_status->setText(USER_STATUS_PUSHING_STREAM);
        ui.pushButton_invite->hide();
		ui.pushButton_cancelKickOut->hide();
		if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0)//当前登陆的是本人
		{
			if (per.kick_inav) 
				ui.pushButton_kickout->show();
			else
				ui.pushButton_kickout->hide();

			if (per.kick_inav_stream)
				ui.pushButton_StopActive->show();
			else
				ui.pushButton_StopActive->hide();
		}
		else//本人
		{
			ui.pushButton_kickout->hide();
			ui.pushButton_StopActive->hide();
		}
        break;
    }
    case MemberStatus_Watching: {//2 观看中 显示踢出
        ui.label_status->setText(USER_STATUS_WATCHING_STREAM);
		if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0) {//当前登陆的是本人
			per.audit_inav_publish? ui.pushButton_invite->show():ui.pushButton_invite->hide();
			per.kick_inav ?ui.pushButton_kickout->show():ui.pushButton_kickout->hide();

		}
		else
		{
			ui.pushButton_invite->hide();
			ui.pushButton_kickout->hide();
		}
        ui.pushButton_cancelKickOut->hide();
        ui.pushButton_StopActive->hide();
        break;
    }
    case MemberStatus_BeInvited: {// 3 受邀中 显示踢出 
        
        ui.label_status->setText(USER_STATUS_BE_INVITED);
		if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0) //当前登陆的是本人
			per.kick_inav ? ui.pushButton_kickout->show() : ui.pushButton_kickout->hide();
		else
			ui.pushButton_kickout->hide();

        ui.pushButton_invite->hide();
        ui.pushButton_cancelKickOut->hide();
        ui.pushButton_StopActive->hide();
        break;
    }
    case MemberStatus_Application: {// 4 申请中 显示踢出 和 同意 和 拒绝
        ui.label_status->setText(USER_STATUS_ASK_FOR_PUSH);
		if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0) //当前登陆的是本人
			per.kick_inav ? ui.pushButton_kickout->show() : ui.pushButton_kickout->hide();
		else
			ui.pushButton_kickout->hide();

		ui.pushButton_invite->hide();
		ui.pushButton_cancelKickOut->hide();
		ui.pushButton_StopActive->hide();
        break;
    }
    case MemberStatus_KickOut: { // 5  被踢出 取消踢出
        ui.label_status->setText(USER_STATUS_KICK_OUT);
        ui.pushButton_invite->hide();
        ui.pushButton_kickout->hide();
		ui.pushButton_StopActive->hide();
		if (mUserID.compare(globalToolManager->GetDataManager()->GetThridPartyUserId()) != 0) {//当前登陆的是本人
			per.kick_inav ? ui.pushButton_cancelKickOut->show() : ui.pushButton_cancelKickOut->hide();
		}
        break;
    }
    default:
        ui.label_status->setText(USER_STATUS_KICK_OUT);
        break;
    }
}

//邀请成员上麦
void VHRoomMemberWdg::slot_ProcessOnLineUser() {
    if (globalToolManager && globalToolManager->GetDataManager() && globalToolManager->GetPaasSDK()) {
        if (globalToolManager->GetDataManager()->GetThridPartyUserId() != mUserID) {
            globalToolManager->GetDataManager()->WriteLog("%s AskforInavPublish mUserID:%s", __FUNCTION__, mUserID.toStdWString().c_str());
            globalToolManager->GetPaasSDK()->AskforInavPublish(mUserID.toStdString());
            ui.pushButton_invite->setEnabled(false);
        }
    }
}

//将默认下麦
void VHRoomMemberWdg::slot_StopActive() {
    if (globalToolManager && globalToolManager->GetDataManager() && globalToolManager->GetPaasSDK()) {
        if (globalToolManager->GetDataManager()->GetThridPartyUserId() != mUserID) {
            globalToolManager->GetDataManager()->WriteLog("%s KickStream_KickOutUser mUserID:%s", __FUNCTION__, mUserID.toStdWString().c_str());
            globalToolManager->GetPaasSDK()->KickInavStream(mUserID.toStdString(), KickStream_KickOutUser);
			ui.pushButton_StopActive->setEnabled(false);
        }
    }
}

//踢出互动房间
void VHRoomMemberWdg::slot_KickOutUser() {
    if (globalToolManager && globalToolManager->GetDataManager() && globalToolManager->GetPaasSDK()) {
        if (globalToolManager->GetDataManager()->GetThridPartyUserId() != mUserID) {
            globalToolManager->GetPaasSDK()->KickInav(mUserID.toStdString(), KickStream_KickOutUser);
            ui.pushButton_kickout->setEnabled(false);
        }
    }
}

//取消踢出互动房间
void VHRoomMemberWdg::slot_CancelKickout() {
    if (globalToolManager && globalToolManager->GetDataManager() && globalToolManager->GetPaasSDK()) {
        if (globalToolManager->GetDataManager()->GetThridPartyUserId() != mUserID) {
            globalToolManager->GetPaasSDK()->KickInav(mUserID.toStdString(), KickStream_CancelKickOutUser);
            ui.pushButton_cancelKickOut->setEnabled(false);
        }
    }
}


QString VHRoomMemberWdg::GetThirdPartyUserId() {
    return mUserID;
}

int VHRoomMemberWdg::GetUserStatus() {
    return mStatus;
}

void VHRoomMemberWdg::EnableAllButton() {
    ui.pushButton_invite->setEnabled(true);
    ui.pushButton_kickout->setEnabled(true);
    ui.pushButton_cancelKickOut->setEnabled(true);
    ui.pushButton_StopActive->setEnabled(true);
}

//void VHRoomMemberWdg::ShowInvite(bool bShow)
//{
//	//mbRightInviteShow = bShow;
//	//bShow ? ui.pushButton_invite->show(): ui.pushButton_invite->hide();
//}
//
//void VHRoomMemberWdg::ShowStopActive(bool bShow)
//{
//	mbRightStopActive = bShow;
//	//bShow ? ui.pushButton_StopActive->show() : ui.pushButton_StopActive->hide();
//}

//void VHRoomMemberWdg::ShowKickout(bool bShow)
//{
//	if(bShow) 
//	{
//		ui.pushButton_kickout->show();
//		//ui.pushButton_cancelKickOut->show();
//	}
//	else
//	{
//		ui.pushButton_kickout->hide();	
//	}
//	ui.pushButton_cancelKickOut->hide();
//}

void VHRoomMemberWdg::CancelShowKickout(bool bShow)
{
	if (bShow)
	{
		ui.pushButton_cancelKickOut->show();
	}
	else
	{
		
		ui.pushButton_cancelKickOut->hide();
	}
	ui.pushButton_kickout->hide();
}