#pragma once
#include <QString>
class DataManagerInterface 
{
public:
    DataManagerInterface() {};
    virtual ~DataManagerInterface() {};
    virtual bool Init() = 0;

    virtual void WriteLog(char* pszMessage, ...) = 0;

    virtual void SetAppid(QString appid) = 0;
    virtual QString GetAppid() = 0;

	//virtual void SetJoinUid(QString appid) = 0;
	//virtual QString GetJoinUid() = 0;

    virtual void SetThridPartyUserId(QString third_party_user_id) = 0;
    virtual QString GetThridPartyUserId() = 0;

    virtual void SetInteractionRoomId(QString roomid) = 0;
    virtual QString GetInteractionRoomId() = 0;

    virtual void SetAccessToken(QString token) = 0;
    virtual QString GetAccessToken() = 0;

    virtual QString GetStartUrl() = 0;
    virtual QString GetInavInfoUrl() = 0;

    //virtual void SetPushStreamPermission(bool host) = 0;
    //virtual bool GetPushStreamPermission() = 0;

	virtual void SetIsLiving(bool bIsLiving = false) = 0;
	virtual bool GetIsLiving() = 0;

	virtual QString GetLiveRoomId() = 0;
	virtual void SetLiveRoomId(QString roomId) = 0;

};