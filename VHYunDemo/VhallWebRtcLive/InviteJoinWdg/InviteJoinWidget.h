#ifndef INVITEJOINWIDGET_H
#define INVITEJOINWIDGET_H

#include <QDialog>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

#include "ui_InviteJoinWidget.h"
#include "title_button.h"
#include "../Unility/Unility.h"


class InviteJoinWidget : public CBaseWnd
{
   Q_OBJECT

public:
   InviteJoinWidget(QWidget *parent = 0);
   ~InviteJoinWidget();

   virtual void Notice(const CSTDMapParam &);
   virtual void Notice(const CSTDMapParamPtr &);
   CREATE_WND();

   void CenterWindow(QWidget* parent);
   void InitTimeOut(int timeout);

   QString GetUid();
   void SetUid(const QString& strUid);
private slots:
   void Slot_GuestAgreeHostInvite(); 
   void Slot_GuestRefuseHostInvite();
   void Slot_ShowTimeOut();

signals:
   void Sig_GuestAgreeHostInvite();
   void Sig_GuestRefuseHostInvite();
protected:
   void paintEvent(QPaintEvent *);
   bool eventFilter(QObject *o, QEvent *e);
   void showEvent(QShowEvent *);
private:
   Ui::InviteJoinWidget ui;
   QPixmap mpixmap;
   TitleButton *m_pCloseBtn =  NULL;
   QPoint pressPoint;
   QPoint startPoint;

   QTimer *m_pTimer = NULL;
   int nTimeCount;
	int mnMaxTimeOut = 0;

	QString mStrUid = "";
};

#endif // INVITEJOINWIDGET_H
