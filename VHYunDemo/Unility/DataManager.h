#pragma once

#include <QObject>
#include <QMutex>

#include "DataManagerInterface.h"

class DataManager : public QObject, public DataManagerInterface
{
   Q_OBJECT

public:
    DataManager(QObject *parent = nullptr);
    virtual ~DataManager();
    virtual bool Init();

    virtual void WriteLog(char* pszMessage, ...);

    virtual void SetAppid(QString appid);
    virtual QString GetAppid();

	//virtual void SetJoinUid(QString JoinUid);
	//virtual QString GetJoinUid();

    virtual void SetThridPartyUserId(QString third_party_user_id) ;
    virtual QString GetThridPartyUserId();

    virtual void SetInteractionRoomId(QString roomid);
    virtual QString GetInteractionRoomId();

    virtual void SetAccessToken(QString token);
    virtual QString GetAccessToken();

    virtual QString GetStartUrl();
    virtual QString GetInavInfoUrl();

	virtual QString GetLiveRoomId();
	virtual void SetLiveRoomId(QString roomId);

    virtual void SetPushStreamPermission(bool host);
    virtual bool GetPushStreamPermission();

	virtual void SetIsLiving(bool bIsLiving = false);
	virtual bool GetIsLiving();

private:
    bool bInit = false;
    QString mAppid;
    QString mThridPartyUserId;
    QString mInteractionRoomId;
    QString mAccessToken;
	QString mStrLiveRoomId;
	//QString mJoinUid;

	QMutex mStreamMutexCS;


    std::atomic_bool mbIsHost = false;
    std::atomic_bool mbIsLiving = false;

};
