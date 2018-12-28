#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "../Unility/Unility.h"
#include "ui_LoginWdg.h"

class LoginWdg : public CBaseWnd
{
    Q_OBJECT

public:
    LoginWdg(QWidget *parent = Q_NULLPTR);
    ~LoginWdg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);

private slots:
   void slot_JoinRoom();
   void slot_Setting();
   void slot_Close();
   void slot_Min();
   void slot_GetAccessToken();
   void slot_GetAppId();
   void slot_GetThirdPartyID();
 

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::LoginWdg ui;

    QPoint mPressPoint;
    bool mIsMoved = false;
    bool m_bIsEnableMove = true;

    std::list<std::string> sendlist_;

};
