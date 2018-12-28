#pragma once

#include <QWidget>
#include "ui_VHRoomMemberWdg.h"

//�û�״̬ 1 ������ 2 �ۿ��� 3 ������ 4 ������ 5 ���߳�
#define  USER_STATUS_PUSHING_STREAM     QStringLiteral("������")
#define  USER_STATUS_WATCHING_STREAM    QStringLiteral("�ۿ���")
#define  USER_STATUS_BE_INVITED         QStringLiteral("������")
#define  USER_STATUS_ASK_FOR_PUSH       QStringLiteral("��������")
#define  USER_STATUS_KICK_OUT           QStringLiteral("���߳�")

#define  USER_STATUS_CANCEL_KICK_OUT    QStringLiteral("ȡ���߳�")
#define  USER_STATUS_INVITE_USER        QStringLiteral("��������")
#define  USER_STATUS_STOP_PUSHING       QStringLiteral("����")
#define  USER_STATUS_AGREE              QStringLiteral("ͬ������")
#define  USER_STATUS_NOT_AGREE          QStringLiteral("�ܾ�����")

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
