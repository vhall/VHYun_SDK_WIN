#ifndef CSCREENSHARETOOLWGD_H
#define CSCREENSHARETOOLWGD_H

#include <QWidget>
#include "ui_CScreenShareToolWgd.h"

#define DEF_WDG_WIDTH   67

class QPropertyAnimation;

class CScreenShareToolWgd : public QWidget {
   Q_OBJECT

public:
   CScreenShareToolWgd(QWidget *parent = 0);
   ~CScreenShareToolWgd();

   void SetHostUser(bool bHost);

   void SetMicState(bool open);
   void SetCameraState(bool open);
   void SetStartLive(bool start);
   void EnableStartLive(bool bEnable);
   void SetPlayerState(bool open);
   bool GetPlayerState();

   void setTencentSDK(bool bTen = true);
public slots:
   void Slot_OnClickedPlayerBtn();
protected:
   virtual void enterEvent(QEvent *);
   virtual void leaveEvent(QEvent *);
   virtual void showEvent(QShowEvent *);
private slots:
   void Slot_OnClickedMicBtn();
   void Slot_OnClickedStopShareBtn();
   void Slot_OnClickedCameraBtn();
   
   void Slot_OnClickedStartLive();
   void on_btnChat_clicked();
signals:
   void Sig_ClickedMicBtn();
   void Sig_ClickedStopShareBtn();
   void Sig_ClickedCameraBtn();
   void Sig_ClickedPlayerBtn();
   void Sig_ClickedStartLiveBtn();
   void Sig_ChatClicked();
   void Sig_OnClickedSet();
private:
   Ui::CScreenShareToolWgd ui;
   bool m_bEnterEvent = false;
   bool m_bHost = true;
   bool mbPlayerOpenState = false;
   bool mbTencentSDK;
};

#endif // CSCREENSHARETOOLWGD_H
