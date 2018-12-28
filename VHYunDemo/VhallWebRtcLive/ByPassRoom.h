#ifndef BYPASSROOMWDG_H
#define BYPASSROOMWDG_H

#include <QDialog>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

#include "ui_ByPassRoom.h"
#include "title_button.h"
#include "../Unility/Unility.h"


class ByPassRoomWdg : public CBaseWnd
{
   Q_OBJECT

public:
   ByPassRoomWdg(QWidget *parent = 0);
   ~ByPassRoomWdg();

   virtual void Notice(const CSTDMapParam &);
   virtual void Notice(const CSTDMapParamPtr &);
   CREATE_WND();

   void CenterWindow(QWidget* parent);
   void InitTimeOut(int timeout);

   QString GetUid();
   void SetUid(const QString& strUid);
public slots:
	int exec();
private slots:
   void Slot_GuestAgreeHostInvite(); 
   void Slot_GuestRefuseHostInvite();
   //void Slot_ShowTimeOut();

signals:
   void Sig_GuestAgreeHostInvite();
   void Sig_GuestRefuseHostInvite();
protected:
   void paintEvent(QPaintEvent *);
   bool eventFilter(QObject *o, QEvent *e);
   //void showEvent(QShowEvent *);
private:
   Ui::ByPassRoomWdg ui;
   QPixmap mpixmap;
   TitleButton *m_pCloseBtn =  NULL;
   QPoint pressPoint;
   QPoint startPoint;

   //QTimer *m_pTimer = NULL;
   int nTimeCount;
	int mnMaxTimeOut = 0;

	QString mStrUid = "";
};

#endif // BYPASSROOMWDG_H
