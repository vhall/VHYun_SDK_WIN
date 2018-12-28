#pragma once

#include <QWidget>
#include "ui_VHRoomMemberWdg.h"

//用户状态 1 推流中 2 观看中 3 受邀中 4 申请中 5 被踢出
#define  USER_STATUS_PUSHING_STREAM     QStringLiteral("推流中")
#define  USER_STATUS_WATCHING_STREAM    QStringLiteral("观看中")
#define  USER_STATUS_BE_INVITED         QStringLiteral("受邀中")
#define  USER_STATUS_ASK_FOR_PUSH       QStringLiteral("申请上麦")
#define  USER_STATUS_KICK_OUT           QStringLiteral("被踢出")

#define  USER_STATUS_CANCEL_KICK_OUT    QStringLiteral("取消踢出")
#define  USER_STATUS_INVITE_USER        QStringLiteral("邀请上麦")
#define  USER_STATUS_STOP_PUSHING       QStringLiteral("下麦")
#define  USER_STATUS_AGREE              QStringLiteral("同意上麦")
#define  USER_STATUS_NOT_AGREE          QStringLiteral("拒绝上麦")

enum MEMBER_LIST{
    MEMBER_LIST_ONLINE_LIST_MEMBER = 0,
    MEMBER_LIST_PUSHINT_USER,
    MEMBER_LIST_KICK_OUT
};
class VHRoomMemberWdg : public QWidget
{
    Q_OBJECT

public:
    VHRoomMemberWdg(MEMBER_LIST type,std::string userId,int status,QWidget *parent = Q_NULLPTR);
    ~VHRoomMemberWdg();

    QString GetThirdPartyUserId();
    void SetUserStatus(int status);
    int GetUserStatus();
    void EnableAllButton();
	//void ShowInvite(bool bShow);
	//void ShowStopActive(bool bShow);
	//void ShowKickout(bool bShow);
	void CancelShowKickout(bool bShow);
 private slots:
    void slot_ProcessOnLineUser();
    void slot_KickOutUser();
    void slot_CancelKickout();
    void slot_StopActive();
private:
    Ui::VHRoomMemberWdg ui;
    QString mUserID;
    int mStatus = 0;
    MEMBER_LIST mMemberType;
	//bool mbRightInviteShow = false;
	//bool mbRightStopActive = false;
};
