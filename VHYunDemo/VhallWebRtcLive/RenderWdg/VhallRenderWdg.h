#pragma once

#include <QWidget>
#include <QMouseEvent>

#include "ui_VhallRenderWdg.h"
#include "VhallRenderMaskWdg.h"
#include "MemberCtrlMaskWdg.h"
#define SMALL_RENDER_VIEW_SIZE	QSize(175, 131)
#define MAIN_RENDER_VIEW_SIZE	QSize(294, 243)

#define MAIN_RENDER_VIEW_WIDTH 320
#define MAIN_RENDER_VIEW_HEIGHT 240

class VhallRenderWdg : public QWidget
{
	Q_OBJECT
public:
	VhallRenderWdg(QWidget *parent = Q_NULLPTR);
	~VhallRenderWdg();

  /***
  *
  * 参数说明：
  *  userId：参会者id
  *  userName：参会者昵称
  *  userRole：参会者角色
  *  mInRoomRole：登录用户角色
  *  bShowInMainView: 是否显示在大窗中
  *  loginUid： 登录用户id
  ****/
	void InitUserInfo(const QString& userId,bool bShowInMainView,const QString& loginUid);
	bool isCurrentUser(const QString& uid);
	HWND GetRenderWndID();
	void SetViewState(int state);
	void ResetViewSize(bool isHost);


	void SetUserCameraState(bool open);
	void SetUserMicState(bool open);
    bool GetUserCameraState();
	void ShowSetSpeakBtn(bool show);
	void ShowSetKickOutBtn(bool show);
	QString GetUserID();
   //QString GetNickName();
    bool GetUserMicState();
    void SetEnableUpdateRendWnd(bool enable);
   

private slots:
	void slot_OnClickedCamera(bool);
	void slot_OnClickedMic(bool);
	void slot_OnClickedSetMainView();
	void slot_OnClickedKickOffRoom();
	void slot_OnClickedSetToSpeaker();
signals:
	void sig_setInMainView(QString);
	void sig_setToSpeaker(QString);
	void sig_ClickedCamera(QString, bool);
	void sig_ClickedMic(QString, bool);
	void sig_NotToSpeak(QString);//下麦

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);

private:
	//QString mRenderRoleName;  //显示区域用户角色
	QString mRenderUserId;	  //显示区域用户ID
	//QString mRenderUserName;  //显示区域用户昵称

	HWND mRenderWndID = NULL;
	
	bool mIsMainView = false;	//当前窗体是否显示在主窗体。
	//QString mLoginRoomRole;			//当前登录用户在房间中的角色。
	QString mLoginUid;         //登陆当前房间的用户id
private:
	Ui::VhallRenderWdg ui;
	MemberCtrlMaskWdg *mpMemberCtrlWdg = NULL;     
    VhallRenderMaskWdg *mpVhallRenderMaskWdg = NULL;
	int mnStateType = 0;
   bool mbCameraOpen = true;
   bool mbMicOpen = true;
};
