#ifndef CINTERACTIVECTRLWDG_H
#define CINTERACTIVECTRLWDG_H

#include <QWidget>
#include "ui_CInterActiveCtrlWdg.h"
#include "vlive_def.h"

class CameraMgrUI;

class CInterActiveCtrlWdg : public QWidget {
   Q_OBJECT

public:
   CInterActiveCtrlWdg(QWidget *parent = 0);
   ~CInterActiveCtrlWdg();
   void InitBtnState(bool bHost);
   void InitConnect();
	void SetHideDevList();
   void ShowTop(bool top);

	//void ShowToSpeakBtn(bool show);
   void intShowStartLive(bool bShow);
   void initToSpeakBtnState(bool toSpeak);
   void SetToSpeakBtnState(bool toSpeak);
   void OnOpenCamera(bool bOpen);
   void OnOpenMic(bool bOpen);
   void OnOpenPlayer(bool bOpen);
   void OnOpenScreenShare(bool bOpen);
	//void ShowStartLiveBtn(bool show);
   void OnStartLive(bool start);
   void OnSetLiveTime(const QString& time);
   QString GetLiveTime();
   QPoint GetMicBtnGlobalPos();
   QPoint GetPlayBtnGlobalPos();
   QPoint GetMicListBtnGlobalPos();
   QPoint GetPlayListBtnGlobalPos();
   QPoint GetCameraListBtnGlobalPos();
   int GetCameraListBtnWidth();
   int GetMicListBtnWidth();
   int GetPlayListBtnWidth();
   //bool GetToSpeakBtnState();
   void SetMicListBtnEnable(bool bEnable);
   void SetFullState(bool isFull);
	void SetBackGroundColor(QString styleSheet);
   void ShowMemberListBtn();
   void SetEixtUnseeHandsUpList(bool bExit);

   void SetEnableToSpeakBtn(bool enable);

   void ChageSpacerNomal(bool bNomal = false);
   void EnableStartLive(bool enable);
   void SetEnablePlguinsBtn(bool enable);
   void EnablePlguinsBtn(bool enable);
   void SetToSpeakEnble(bool bEnble = true);
   //void CreateMgrUI();
   //bool InitCameraMgrUI(std::list<VhallLiveDeviceInfo> &micDevList);
   void setPubLishInavAnotherState(bool bCurState);
   
   void ShowPublishInavAnother(bool bShow);
   //void ShowToSpeak(bool bShow);
   //void ShowStartLive(bool bShow);//上/下麦
signals:
   void Sig_OnClickedCameraCtrlBtn();
   void Sig_OnClickedMicCtrlBtn();
   void Sig_OnClickedPlayerCtrlBtn();
   void Sig_OnClickedScreenShareCtrlBtn();
   void Sig_OnClickedStartLiveCtrlBtn();
   //void Sig_OnClickedLeaveActiveBtn();
   void Sig_OnClickedShowCameraListBtn();
   void Sig_OnClickedShowMicListBtn();
   void Sig_OnClickedShowPlayerBtn();
   void Sig_OnMicBtnEnter();
   void Sig_OnMicBtnLeave();
   void Sig_OnPlayerBtnEnter();
   void Sig_OnPlayerBtnLeave();
   void Sig_MouseEnterCtrlWdg();
   //void SigCurRecordState(int iCurState);
   void SigStopLive();
   void sig_OnPlayFileClicked();
   void sigMemberListClicked();
   void sigToSpeakClicked();
   void sigPublishInavAnotherClicked();
protected:
   virtual void mouseMoveEvent(QMouseEvent *);
   void DisMouseTracking();
   void EnableMouseTracking();   

private slots:
	//void slotToSpeakClicked();
	void Slot_OnClickedCameraCtrlBtn();
	void Slot_OnClickedMicCtrlBtn();
	void Slot_OnClickedPlayerCtrlBtn();
	void Slot_OnClickedScreenShareCtrlBtn();
	void Slot_OnClickedStartLiveCtrlBtn();
	//void Slot_OnClickedLeaveActiveBtn();
	void Slot_OnClickedShowCameraListBtn();
	void Slot_OnClickedShowMicListBtn();
	void Slot_OnClickedShowPlayerBtn();
	void Slot_OnMicBtnEnter();
	void Slot_OnMicBtnLeave();
	void Slot_OnPlayerBtnEnter();
	void Slot_OnPlayerBtnLeave();
	void Slot_LiveToolClicked();
	void slotPlayFileClicked();
	void slotPublishInavAnotherClicked();
	
private:
   Ui::CInterActiveCtrlWdg ui;
   bool m_bIsFull = false;
   bool m_bRecordShow = false;

   //bool mCurLiveState;//当前直播状态 True 正在直播
   //bool mbIsSpeaking = false;

   //bool m_bToSpeakState = true;
  //CameraMgrUI* m_pCameraMgrUI = NULL;              //摄像头列表界面
};

#endif // CINTERACTIVECTRLWDG_H
