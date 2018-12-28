#include "VhallIALiveSettingDlg.h"
#include "NoiseTooltip.h"
#include "Pathmanager.h"
#include "ConfigSetting.h"     
#include "../Unility/CustomEventType.h"
#include "../Unility/Unility.h"
#include <QVariant>


extern ToolManager *globalToolManager;

CREATE_WND_FUNCTION(VhallIALiveSettingDlg);

#define SET_BUTTON_DOWN_STYLE   "\
                           QPushButton , QPushButton{ \
                           border-image: url(:/sysButton/sysButtonIcon_down); \
                           font-size : 14px;\
                           font-family : 微软雅黑; \
                           color : #ffffff;} \
                           "

#define SET_BUTTON_UP_STYLE    "\
                           QPushButton , QPushButton:pressed{ \
                           border-image: url(:/sysButton/sysButtonIcon_up); \
                           font-size : 14px;\
                           font-family : 微软雅黑; \
                           color : #a7a7a7;} \
                           QPushButton:hover{border-image: url(:/sysButton/sysButtonIcon_hover);}\
"

#define LAYOUT_FLOAT_ENABLE "border-image: url(:/vhallActive/img/vhallactive/float_select.png);"
#define LAYOUT_FLOAT_DISABLE "border-image: url(:/vhallActive/img/vhallactive/float.png);"
#define LAYOUT_TILED_ENABLE  "border-image: url(:/vhallActive/img/vhallactive/tiled_select.png);"
#define LAYOUT_TILED_DISABLE "border-image: url(:/vhallActive/img/vhallactive/tiled.png);"
#define LAYOUT_GRID_ENABLE "border-image: url(:/vhallActive/img/vhallactive/grid_select.png);"
#define LAYOUT_GRID_DISABLE "border-image: url(:/vhallActive/img/vhallactive/grid.png);"


CameraInfo::CameraInfo()
{

}

//CameraInfo::CameraInfo(const QString& strId, const int& iIdex, std::list<vhall::VideoDevProperty> DevPropertyList)
//{
//	m_strId = strId;
//	m_iIdex = iIdex;
//
//	init(DevPropertyList);
//}

CameraInfo::CameraInfo(const QString& strId, const int& iIdex)
{
	m_strId = strId;
	m_iIdex = iIdex;
}
CameraInfo::CameraInfo(const QString& strId, const int& iIdex, std::vector<std::shared_ptr<vhall::VideoFormat>> FormatList)
{
	m_strId = strId;
	m_iIdex = iIdex;

	init(FormatList);
}

CameraInfo::CameraInfo(const CameraInfo& Info)
{
	m_strId = Info.m_strId;
	m_iIdex = Info.m_iIdex;

	vhall::VideoFormat dev;
	foreach(dev, Info.m_FormatList)
	{
		m_FormatList.append(dev);
	}

}

CameraInfo::~CameraInfo()
{
	m_strId = "";
	m_iIdex = -1;
	m_FormatList.clear();
}

void CameraInfo::init(std::vector<std::shared_ptr<vhall::VideoFormat>> ver)
{
	//vhall::VideoFormat format;
	std::vector<std::shared_ptr<vhall::VideoFormat>>::iterator it = ver.begin();
	while (it != ver.end())
	{
		vhall::VideoFormat format;
		format.width = (*it)->width;
		format.height = (*it)->height;
		format.maxFPS = (*it)->maxFPS;
		format.videoType = (*it)->videoType;
		format.interlaced = (*it)->interlaced;

		m_FormatList.append(format);
		it++;
	}
}

VhallIALiveSettingDlg::VhallIALiveSettingDlg(QWidget *parent,bool host)
	: CBaseWnd(parent)
   , mbIsHost(host)
{
	ui.setupUi(this);
	mMapProfileIndex.insert(VIDEO_PROFILE_UNDEFINED, "UNEFINED");
	mMapProfileIndex.insert(VIDEO_PROFILE_120P_0, "120P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_120P_1, "120P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_180P_0, "180P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_180P_1, "180P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_240P_0, "240P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_240P_1, "240P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_360P_0, "360P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_360P_1, "360P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_480P_0, "480P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_480P_1, "480P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_720P_0, "720P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_720P_1, "720P_1");
	mMapProfileIndex.insert(VIDEO_PROFILE_1080P_0, "1080P_0");
	mMapProfileIndex.insert(VIDEO_PROFILE_1080P_1, "1080P_1");
	//mMapProfileIndex.insert(VIDEO_PROFILE_720P_DESKTOP, "720P_DESKTOP");
	//mMapProfileIndex.insert(VIDEO_PROFILE_1080P_DESKTOP, "1080P_DESKTOP");

   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	ui.widget_title->SetTitleType(eVhallIALiveSetting);

	connect(ui.pushButton_audio, SIGNAL(clicked()), this, SLOT(slot_OnSelectAudioPage()));
	connect(ui.pushButton_video, SIGNAL(clicked()), this, SLOT(slot_OnSelectVideoPage()));
	connect(ui.pushButton_aboutUs, SIGNAL(clicked()), this, SLOT(slot_OnSelectAboutUsPage()));
	connect(ui.pushButton_changeLayoutMode, SIGNAL(clicked()), this, SLOT(slot_OnChangeLayoutPage()));

	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(slot_OnClickedApply()));
	connect(ui.widget_title, SIGNAL(sig_ClickedClose()), this, SLOT(slot_OnClickedClose()));
	connect(ui.comboBox_cameraSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentCameraSelectChanged(int)));
	connect(ui.comboBox_micDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentMicSelectChanged(int)));
	connect(ui.comboBox_playerDev, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_CurrentPlayerSelectChanged(int)));

	connect(ui.pushButton_tiledMode, SIGNAL(clicked()), this, SLOT(slot_SelectTiledMode()));
	connect(ui.pushButton_floatMode, SIGNAL(clicked()), this, SLOT(slot_SelectFloatMode()));
	slot_OnSelectVideoPage();
   //ui.pushButton_apply->hide();
   if (mbIsHost) {
      ui.widget_layout->show();
   }
   else {
      ui.widget_layout->hide();
   }
   ui.label_appName->hide();
   ui.label_version->hide();
   m_pNoiseTips = new NoiseTooltip(this);
   if (m_pNoiseTips) {
      m_pNoiseTips->hide();
      m_pNoiseTips->setText(QString::fromWCharArray(L"该布局调整非上麦观众的视频画面布\n局，目前支持主次浮窗、主次平铺、\n均分布局三种方式"));
   }
   ui.label_tips->installEventFilter(this);
   ui.label_layoutChangeTips->hide();
   //ui.pushButton_changeLayoutMode->hide();
   QString toolConfPath = CPathManager::GetAppDataPath() + QString::fromStdWString(VHALL_TOOL_CONFIG);
   int hideLogo = ConfigSetting::ReadInt(toolConfPath, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
   if (hideLogo != 0) {
       ui.pushButton_aboutUs->hide();
   }
}

VhallIALiveSettingDlg::~VhallIALiveSettingDlg()
{
   ui.label_tips->removeEventFilter(this);
}


void VhallIALiveSettingDlg::init()
{
	mbEnableUserSelectCamera = false;
	mbEnableUserSelectMic = false;
	mbEnableUserSelectPlayer = false;

    VHPaasInteractionRoom* pInterActionRoom = globalToolManager->GetPaasSDK();
	if (pInterActionRoom ) {

		mcameraList.clear();
		pInterActionRoom->GetCameraDevices(mcameraList);

		//摄像设备显示
		ui.comboBox_cameraSelect->clear();
		std::list<vhall::VideoDevProperty>::iterator it = mcameraList.begin();

		mbEnableUserSelectCamera = (mcameraList.size()>0);
		for (; it != mcameraList.end(); it++) {
			//CameraInfo info(QString::fromStdString(it->mDevId), it->mIndex, it->mDevFormatList);
			ui.comboBox_cameraSelect->addItem(QString::fromStdString(it->mDevName), QVariant::fromValue(*it));
            mLastCameraIndex = 0;
		}
		ui.comboBox_cameraSelect->setCurrentIndex(mLastCameraIndex);

		//麦克风显示
		std::list<vhall::AudioDevProperty> micList;
		pInterActionRoom->GetMicDevices(micList);
		mbEnableUserSelectMic = (micList.size()>0);
		ui.comboBox_micDev->clear();

        std::list<vhall::AudioDevProperty>::iterator itMic = micList.begin();
        while (itMic != micList.end()) {
			CameraInfo info(QString::fromStdString(itMic->mDevGuid), itMic->mIndex);
            ui.comboBox_micDev->addItem(QString::fromStdString(itMic->mDevName), QVariant::fromValue(info));
            mLastMicIndex = 0;
            itMic++;
        }
		ui.comboBox_micDev->setCurrentIndex(mLastMicIndex);

		//扬声器
		std::list<vhall::AudioDevProperty> playerList;
		pInterActionRoom->GetPlayerDevices(playerList);

		ui.comboBox_playerDev->clear();
		mbEnableUserSelectPlayer = (playerList.size()>0);
        std::list<vhall::AudioDevProperty>::iterator player = playerList.begin();
        while (player != playerList.end()) {
            //QVariant var(QString::fromStdString(player->mDevGuid));
			CameraInfo info(QString::fromStdString(player->mDevGuid), player->mIndex);
            ui.comboBox_playerDev->addItem(QString::fromStdString(player->mDevName), QVariant::fromValue(info));
            mLastPlayerIndex = 0;
            player++;
        }
		ui.comboBox_playerDev->setCurrentIndex(mLastPlayerIndex);
	}
}

void VhallIALiveSettingDlg::RegisterListener(DeviceChangedListener* listener) {
	mpDevChangedListener = listener;
}

void VhallIALiveSettingDlg::setPushPermission(bool bPermi)
{
	bPermi ? ui.pushButton_changeLayoutMode->show() : ui.pushButton_changeLayoutMode->hide();
}

void VhallIALiveSettingDlg::Notice(const CSTDMapParam &mapParam) {
}

void VhallIALiveSettingDlg::Notice(const CSTDMapParamPtr &mapParam) {
}

void VhallIALiveSettingDlg::slot_OnIDPage() {
	ui.pushButton_audio-> setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_video-> setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_aboutUs-> setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_changeLayoutMode-> setStyleSheet(SET_BUTTON_UP_STYLE);
}

void VhallIALiveSettingDlg::slot_OnChangeLayoutPage() {
   //TRACE6("%s \n", __FUNCTION__);
   ui.stackedWidget->setCurrentIndex(layout_page);
   ui.pushButton_audio->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_video->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_aboutUs->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_changeLayoutMode->setStyleSheet(SET_BUTTON_DOWN_STYLE);
}

void VhallIALiveSettingDlg::slot_OnSelectAudioPage() {
   //TRACE6("%s \n", __FUNCTION__);
	ui.stackedWidget->setCurrentIndex(audio_page);
	ui.pushButton_audio->setStyleSheet(SET_BUTTON_DOWN_STYLE);
	ui.pushButton_video->setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_aboutUs->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_changeLayoutMode->setStyleSheet(SET_BUTTON_UP_STYLE);
}

void VhallIALiveSettingDlg::slot_OnSelectVideoPage() {
   //TRACE6("%s \n", __FUNCTION__);
	ui.stackedWidget->setCurrentIndex(video_page);
	ui.pushButton_audio->setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_video->setStyleSheet(SET_BUTTON_DOWN_STYLE);
	ui.pushButton_aboutUs->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_changeLayoutMode->setStyleSheet(SET_BUTTON_UP_STYLE);
}

void VhallIALiveSettingDlg::slot_OnSelectAboutUsPage() {
   //TRACE6("%s \n", __FUNCTION__);
	ui.stackedWidget->setCurrentIndex(aboutUs_page);
	ui.pushButton_audio->setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_video->setStyleSheet(SET_BUTTON_UP_STYLE);
   ui.pushButton_changeLayoutMode->setStyleSheet(SET_BUTTON_UP_STYLE);
	ui.pushButton_aboutUs->setStyleSheet(SET_BUTTON_DOWN_STYLE);
}

void VhallIALiveSettingDlg::Apply() {
	//TRACE6("%s \n", __FUNCTION__);
	bool cameraChange = false;
	bool micChanged = false;
	bool playerChanged = false;
	QString selectCamera;
	QString selectMic;
	QString selectPlayer;
	////如果选择的设备生效，则设置生效的设备。如果不生效则选中上一次的设备。
	int cameraIndex = -1;
	

	vhall::VideoDevProperty dev;
	if (mbEnableUserSelectCamera) {
		dev = ui.comboBox_cameraSelect->currentData().value<vhall::VideoDevProperty>();
	}
	else {
		dev = ui.comboBox_cameraSelect->itemData(mLastCameraIndex).value<vhall::VideoDevProperty>();
		
	}
	
	selectCamera = QString::fromStdString(dev.mDevId);
	cameraIndex = ui.comboBox_cameraRatio->currentData().value<int>();
	int micIndex = -1;
	CameraInfo info;
	if (mbEnableUserSelectMic) {
		info = ui.comboBox_micDev->currentData().value<CameraInfo>();

	}
	else {
		info = ui.comboBox_micDev->itemData(mLastCameraIndex).value<CameraInfo>();
	}
	micIndex = info.index();
	selectMic = info.Id();

	int playerIndex = -1;
	if (mbEnableUserSelectPlayer) {
		info = ui.comboBox_playerDev->currentData().value<CameraInfo>();
	}
	else {
		info = ui.comboBox_playerDev->itemData(mLastCameraIndex).value<CameraInfo>();
	}

	selectPlayer = info.Id();
	playerIndex = info.index();

	//TRACE6("%s selectCamera:%s selectMic:%s selectPlayer:%s\n", __FUNCTION__, selectCamera.toStdString().c_str(), selectMic.toStdString().c_str(), selectPlayer.toStdString().c_str());
	if (mpDevChangedListener) {
		mpDevChangedListener->OnDevChanged(selectCamera, cameraIndex, selectMic, micIndex, selectPlayer, playerIndex, meLayoutMode);
	}
}

void VhallIALiveSettingDlg::slot_OnClickedApply() {
	Apply();
	accept();
}

//void VhallIALiveSettingDlg::slot_OnClickedOK() {
//   
//}

void VhallIALiveSettingDlg::slot_OnClickedClose() {
   ui.label_layoutChangeTips->hide();
   //TRACE6("%s end\n", __FUNCTION__);
   accept();
}

//LAYOUT_MODE VhallIALiveSettingDlg::GetLayOutMode() {
//   return meLayoutMode;
//}

//void VhallIALiveSettingDlg::SetLayOutMode(LAYOUT_MODE mode) {
//   meLayoutMode = mode;
//   if (meLayoutMode == LAYOUT_MODE_FLOAT) {
//      ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_ENABLE);
//      ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_DISABLE);
//   }
//   else if (meLayoutMode == LAYOUT_MODE_GRID) {
//      ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_DISABLE);
//      ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_DISABLE);
//   }
//   else if (meLayoutMode == LAYOUT_MODE_TILED) {
//      ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_DISABLE);
//      ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_ENABLE);
//   }
//}
//
//void VhallIALiveSettingDlg::SetLastLayoutMode() {
//   meLastLayoutMode = meLayoutMode;
//}

void VhallIALiveSettingDlg::showEvent(QShowEvent *event) {
   this->setAttribute(Qt::WA_Mapped);
   QWidget::showEvent(event);
}

void VhallIALiveSettingDlg::customEvent(QEvent* event) {

}


void VhallIALiveSettingDlg::slot_CurrentCameraSelectChanged(int index) {
	ui.comboBox_cameraRatio->clear();
	std::shared_ptr<vhall::VideoDevProperty> info = std::make_shared<vhall::VideoDevProperty>();
	*info = (ui.comboBox_cameraSelect->currentData().value<vhall::VideoDevProperty>());
	for (int profile = VIDEO_PROFILE_120P_0; profile < VIDEO_PROFILE_720P_DESKTOP; profile++) {
		if (globalToolManager->GetPaasSDK()->IsSupported(info, (VideoProfileIndex)profile))
		{
			ui.comboBox_cameraRatio->addItem(mMapProfileIndex[profile], QVariant::fromValue(profile));
		}
	}
	ui.comboBox_cameraRatio->setCurrentIndex(0);
}

void VhallIALiveSettingDlg::slot_CurrentMicSelectChanged(int index) {
   //TRACE6("%s \n", __FUNCTION__);
   if (!mbInitSuc) {
      return;
   }
   //TRACE6("%s end\n", __FUNCTION__);
}

void VhallIALiveSettingDlg::slot_CurrentPlayerSelectChanged(int index) {
   //TRACE6("%s \n", __FUNCTION__);
   if (!mbInitSuc) {
      return;
   }
   //TRACE6("%s end\n", __FUNCTION__);
}


void VhallIALiveSettingDlg::slot_SelectFloatMode() {
   
   ui.label_layoutChangeTips->hide();
   //TRACE6("%s LAYOUT_MODE_FLOAT\n", __FUNCTION__);
   meLayoutMode = CANVAS_LAYOUT_PATTERN_FLOAT_6_5D;
   //ui.pushButton_gridMode->setStyleSheet(LAYOUT_GRID_DISABLE);
   ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_ENABLE);
   ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_DISABLE);
}

void VhallIALiveSettingDlg::slot_SelectTiledMode() {
   ui.label_layoutChangeTips->hide();
   //TRACE6("%s LAYOUT_MODE_TILED\n", __FUNCTION__);
   meLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_6_1T5D;
   //ui.pushButton_gridMode->setStyleSheet(LAYOUT_GRID_DISABLE);
   ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_DISABLE);
   ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_ENABLE);
}

void VhallIALiveSettingDlg::slot_SelectGridMode() {
   ui.label_layoutChangeTips->hide();
   //TRACE6("%s LAYOUT_MODE_GRID\n", __FUNCTION__);
   meLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_1;
   //ui.pushButton_gridMode->setStyleSheet(LAYOUT_GRID_ENABLE);
   ui.pushButton_floatMode->setStyleSheet(LAYOUT_FLOAT_DISABLE);
   ui.pushButton_tiledMode->setStyleSheet(LAYOUT_TILED_DISABLE);
}


bool VhallIALiveSettingDlg::eventFilter(QObject *obj, QEvent *event) {
   if (obj && event) {
      if (event->type() == QEvent::Enter) {
         if (obj == ui.label_tips && m_pNoiseTips != NULL) {
            m_pNoiseTips->show();
            QPoint pos = this->mapToGlobal(ui.label_tips->pos());
            m_pNoiseTips->move(pos + QPoint(0, 90));
         }
      }
      else if (event->type() == QEvent::Leave) {
         if (obj == ui.label_tips && m_pNoiseTips != NULL) {
            m_pNoiseTips->hide();
         }
      }
   }
   return QWidget::eventFilter(obj, event);
}


