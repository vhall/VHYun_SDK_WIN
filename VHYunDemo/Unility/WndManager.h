#pragma once

#include <QObject>
#include <mutex>
#include <map>
#include <QWidget>
#include <QString>
#include<map>
#include<string>
#include <QDialog>
#include "WndDef.h"

#define     WND_ID_LOGIN                "WND_ID_LOGIN"
#define     WND_ID_LOADING              "WND_ID_LOADING"
#define     WND_ID_WEBRTC_MAIN          "WND_ID_WEBRTC_MAIN"
#define     WND_ID_WEBRTC_COTROL        "WND_ID_WEBRTC_COTROL"
#define     WND_ID_WEBRTC_SETTING       "WND_ID_WEBRTC_SETTING"
#define     WND_ID_INVITE_TO_SPEAK      "WND_ID_INVITE_TO_SPEAK" //—˚«Î…œ¬Û¥∞ø⁄
#define     WND_ID_ASK_FOR_SPEAK        "WND_ID_ASK_FOR_SPEAK"    //…Í«Î…œ¬Û¥∞ø⁄
#define     WND_ID_MAINUI               "WND_ID_MAINUI"
#define     WND_ID_END                  "WND_ID_END"

#define START_WEB_RTC_LIVE      "StartWebRtcLive"
#define WEBRTC_ROOM_HOST        "WEBRTC_ROOM_HOST"
#define PARAM_TRUE              "TRUE"
#define PARAM_FALSE             "FALSE"
#define USER_ID                 "user_id"

#define CSTDMapParam  std::map<QString, QString> 
#define CSTDMapParamPtr  std::map<QString, void*> 

class CBaseWnd : public QDialog
{
public:
    CBaseWnd(QWidget *parent = Q_NULLPTR);
    virtual ~CBaseWnd();
    virtual void Notice(const CSTDMapParam &) = 0;
    virtual void Notice(const CSTDMapParamPtr &) {};
    virtual void Notice(QEvent* event) {};

	virtual void ClearAllOnLineUser() {};
	virtual void ClearAllKickOutUSer() {};

protected:
    virtual void showEvent(QShowEvent *event);
};


typedef CBaseWnd* (*CreateFunction)();
#define CREATE_WND()    static CBaseWnd* CreateFunction();
#define REGISTER_WND(T) T::CreateFunction
#define CREATE_WND_FUNCTION(T) \
CBaseWnd* T::CreateFunction() { \
    return new T();             \
}                               \


class WndManager : public QObject
{
   Q_OBJECT

public:
    WndManager(QObject *parent = nullptr);
    ~WndManager();
    void RegisterWndFactory(QString wndId, CreateFunction);
    CBaseWnd* FindWnd(QString wndId, QString strWndIndex = QString());
    bool IsWndExist(QString wndId );
    bool HideWnd(QString wndId );
    bool ShowWnd(QString wndId );
    bool DestoryWnd(QString wndId );
    void DestoryAllWnd();
private:
    std::mutex mWndMutex;
    std::map<QString, CreateFunction> mWndFactory;
    std::map<QString, CBaseWnd*> mWndMap;
};
