#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include "vhall_log.h"

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    InitLog();
}

DataManager::~DataManager()
{
}

bool DataManager::Init() {
    return true;
}

void DataManager::WriteLog(char* pszMessage, ...) {
    va_list vargs;
    QString strTmp, strRet;
    va_start(vargs, pszMessage);
    strRet += strTmp.vsprintf(pszMessage, vargs);
    va_end(vargs);
    LOGD(strRet.toStdString().c_str());
}

void DataManager::SetAppid(QString appid) {
    this->mAppid = appid;
}

QString DataManager::GetAppid() {
    return this->mAppid;
}


//void DataManager::SetJoinUid(QString appid) {
//	this->mJoinUid = appid;
//}
//
//QString DataManager::GetJoinUid() {
//	return this->mJoinUid;
//}

void DataManager::SetThridPartyUserId(QString third_party_user_id) {
    this->mThridPartyUserId = third_party_user_id;
}
QString DataManager::GetThridPartyUserId() {
    return this->mThridPartyUserId;
}

void DataManager::SetInteractionRoomId(QString roomid) {
    this->mInteractionRoomId = roomid;
}
QString DataManager::GetInteractionRoomId() {
    return this->mInteractionRoomId;
}

void DataManager::SetAccessToken(QString token) {
    this->mAccessToken = token;
}
QString DataManager::GetAccessToken() {
    return  this->mAccessToken;
}

QString DataManager::GetStartUrl() {
    return  "";//UrlGenerator::GetInvaStartUrl();
}

QString DataManager::GetInavInfoUrl() {
    return  "";//UrlGenerator::GetInvaStartUrl();
}

QString DataManager::GetLiveRoomId()
{
	return mStrLiveRoomId;
}

void DataManager::SetLiveRoomId(QString roomId)
{
	mStrLiveRoomId = roomId;
}

void DataManager::SetPushStreamPermission(bool host) {
    mbIsHost = host;
}

bool DataManager::GetPushStreamPermission() {
    return mbIsHost;
}

void DataManager::SetIsLiving(bool bIsLiving /*= false*/) {
	mbIsLiving = bIsLiving;
}

bool DataManager::GetIsLiving(){
	return mbIsLiving;
}

