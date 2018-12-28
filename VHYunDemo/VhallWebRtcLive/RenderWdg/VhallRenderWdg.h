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
  * ����˵����
  *  userId���λ���id
  *  userName���λ����ǳ�
  *  userRole���λ��߽�ɫ
  *  mInRoomRole����¼�û���ɫ
  *  bShowInMainView: �Ƿ���ʾ�ڴ���
  *  loginUid�� ��¼�û�id
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
	void sig_NotToSpeak(QString);//����

protected:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);

private:
	//QString mRenderRoleName;  //��ʾ�����û���ɫ
	QString mRenderUserId;	  //��ʾ�����û�ID
	//QString mRenderUserName;  //��ʾ�����û��ǳ�

	HWND mRenderWndID = NULL;
	
	bool mIsMainView = false;	//��ǰ�����Ƿ���ʾ�������塣
	//QString mLoginRoomRole;			//��ǰ��¼�û��ڷ����еĽ�ɫ��
	QString mLoginUid;         //��½��ǰ������û�id
private:
	Ui::VhallRenderWdg ui;
	MemberCtrlMaskWdg *mpMemberCtrlWdg = NULL;     
    VhallRenderMaskWdg *mpVhallRenderMaskWdg = NULL;
	int mnStateType = 0;
   bool mbCameraOpen = true;
   bool mbMicOpen = true;
};
