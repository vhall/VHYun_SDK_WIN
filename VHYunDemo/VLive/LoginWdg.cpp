#include "LoginWdg.h"
#include "pathmanager.h"
#include "ConfigSetting.h"

#include <QCryptographicHash> 
#include <QUrl>
#include <QDesktopServices>
extern std::wstring gCurrentVersion;
extern std::string gCurStreamID;

LoginWdg::LoginWdg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_ToLoginRoom,SIGNAL(clicked()),this,SLOT(slot_JoinRoom()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_Close()));
    connect(ui.pushButton_min, SIGNAL(clicked()), this, SLOT(slot_Min()));
    //connect(ui.pushButton_Setting, SIGNAL(clicked()), this, SLOT(slot_Setting()));
    connect(ui.pushButton_accesstoken, SIGNAL(clicked()), this, SLOT(slot_GetAccessToken()));
    connect(ui.pushButton_appid, SIGNAL(clicked()), this, SLOT(slot_GetAppId()));
    connect(ui.pushButton_thridPartyId, SIGNAL(clicked()), this, SLOT(slot_GetThirdPartyID()));


    ui.pushButton_close->loadPixmap(":/sysButton/close_button");
    ui.pushButton_min->loadPixmap(":/sysButton/min_button");
    //ui.pushButton_Setting->loadPixmap(":/sysButton/main_setting");
    ui.label_notice->hide();
    QString applicationDirPathStr = QCoreApplication::applicationDirPath() + QString("/config.ini");
    QString appid = ToolManager::GetInstance()->GetConfigSettingInterface()->ReadString(applicationDirPathStr, "default", "appid", "d317f559");
    QString token = ToolManager::GetInstance()->GetConfigSettingInterface()->ReadString(applicationDirPathStr, "default", "token", "vhall");
    QString third_user_id = ToolManager::GetInstance()->GetConfigSettingInterface()->ReadString(applicationDirPathStr, "default", "third_user_id", "qiuyu");
    QString live_room_id = ToolManager::GetInstance()->GetConfigSettingInterface()->ReadString(applicationDirPathStr, "default", "live_room_id", "lss_772f6eda");
    QString inav_room_id = ToolManager::GetInstance()->GetConfigSettingInterface()->ReadString(applicationDirPathStr, "default", "inav_room_id", "inav_e301abbf");

    ui.lineEdit_access_token->setText(token);
    ui.lineEdit_appid->setText(appid);
    ui.lineEdit_room_id->setText(inav_room_id);
    ui.lineEdit_third_party_user_id->setText(third_user_id);
    ui.lEdtLiveRoomId->setText(live_room_id);

}

LoginWdg::~LoginWdg()
{
}

void LoginWdg::slot_JoinRoom() {
    QString appid = ui.lineEdit_appid->text().trimmed();
    QString roomid = ui.lineEdit_room_id->text().trimmed();
    QString thrid_user_id = ui.lineEdit_third_party_user_id->text().trimmed();
    QString accesstoken = ui.lineEdit_access_token->text().trimmed();
    QString live_room_id = ui.lEdtLiveRoomId->text();

    ToolManager::GetInstance()->GetDataManager()->SetAccessToken(accesstoken);
    ToolManager::GetInstance()->GetDataManager()->SetAppid(appid);
    ToolManager::GetInstance()->GetDataManager()->SetThridPartyUserId(thrid_user_id);
    ToolManager::GetInstance()->GetDataManager()->SetInteractionRoomId(roomid);
	ToolManager::GetInstance()->GetDataManager()->SetLiveRoomId(live_room_id.trimmed());
	gCurStreamID = roomid.toStdString();
	QString ConfigPath = CPathManager::GetConfigPath();
	ConfigSetting::writeValue(ConfigPath, GROUP_DEFAULT, LOG_ID_BASE, thrid_user_id);

    bool bRet = ToolManager::GetInstance()->GetPaasSDK()->LoginHVRoom(accesstoken.toStdString(), appid.toStdString(), thrid_user_id.toStdString(), roomid.toStdString());
    if (bRet) {

    }
    else {
        ui.label_notice->setText(QStringLiteral("登录失败,请重试.."));
    }
    
    QString applicationDirPathStr = QCoreApplication::applicationDirPath() + QString("/config.ini");

    ToolManager::GetInstance()->GetConfigSettingInterface()->WriteValue(applicationDirPathStr, "default", "appid", appid);
    ToolManager::GetInstance()->GetConfigSettingInterface()->WriteValue(applicationDirPathStr, "default", "token", accesstoken);
    ToolManager::GetInstance()->GetConfigSettingInterface()->WriteValue(applicationDirPathStr, "default", "third_user_id", thrid_user_id);
	ToolManager::GetInstance()->GetConfigSettingInterface()->WriteValue(applicationDirPathStr, "default", "live_room_id", live_room_id);
    ToolManager::GetInstance()->GetConfigSettingInterface()->WriteValue(applicationDirPathStr, "default", "inav_room_id", roomid);


}

void LoginWdg::slot_Close() {
    ToolManager::GetInstance()->AppQuit();
}

void LoginWdg::slot_Setting() {
    CBaseWnd* wnd1 = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
    if (wnd1) {
        wnd1->show();
    }
}

void LoginWdg::slot_Min() {
    showMinimized();
}

void LoginWdg::Notice(const CSTDMapParam &mapParam) {

    std::list<std::string>::iterator iterlist = sendlist_.begin();
    if (iterlist != sendlist_.end()) {
        *iterlist = std::string("123");
    }


    std::map<QString, QString>::const_iterator iter = mapParam.find("LoginResult");
    if (iter != mapParam.end()) {
        QString msg = iter->second;//
        ui.label_notice->show();
        ui.label_notice->setText(msg);
    }
} 

void LoginWdg::slot_GetAccessToken() {
    const QUrl regUrl(QLatin1String("http://www.vhallyun.com/docs/show/1013"));
    QDesktopServices::openUrl(regUrl);
}

void LoginWdg::slot_GetAppId() {
    const QUrl regUrl(QLatin1String("http://www.vhallyun.com/test/api/test"));
    QDesktopServices::openUrl(regUrl);
}

void LoginWdg::slot_GetThirdPartyID() {
    const QUrl regUrl(QLatin1String("http://www.vhallyun.com/docs/show/1013"));
    QDesktopServices::openUrl(regUrl);
}


CBaseWnd* LoginWdg::CreateFunction() {
   return new LoginWdg();
}

void LoginWdg::mousePressEvent(QMouseEvent *event) {
    if (event) {
        mPressPoint = this->pos() - event->globalPos();
        mIsMoved = true;
    }
}

void LoginWdg::mouseMoveEvent(QMouseEvent *event) {
    if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
        //QWidget* parent_widget = this->nativeParentWidget();
        //QString objectName = parent_widget->objectName();
        //QPoint parent_point = parent_widget->pos();
        //parent_point.setX(parent_point.x() + event->x() - mPressPoint.x());
        //parent_point.setY(parent_point.y() + event->y() - mPressPoint.y());
        //this->move(mPressPoint);
        this->move(event->globalPos() + mPressPoint);
    }
}

void LoginWdg::mouseReleaseEvent(QMouseEvent *) {
    if (mIsMoved) {
        mIsMoved = false;
    }
}