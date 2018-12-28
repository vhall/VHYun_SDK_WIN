#pragma once

#include <QDialog>
#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QList>

#include "ui_VhallIALiveSettingDlg.h"
#include "VhallUI_define.h"
#include "LiveEventInterface.h"
#include "../Unility/Unility.h"

class VhallWaiting;
class VhallIALiveSettingDlg ;
class NoiseTooltip;
//class VhallIALive;

enum SettingPageIndex {
	video_page = 0,
	audio_page = 1,
	layout_page = 2,
	aboutUs_page = 3,
};

enum CloseType {
   CloseWithSave = 0,
   CloseNone = 1,
   CloseWithSaveLayoutMode = 2,
};

class ReleaseThread :public QThread {
   Q_OBJECT
public:
   ReleaseThread(QObject *parent = NULL);
   ~ReleaseThread();

   void SetSubscribe(VhallIALiveSettingDlg* sub, CloseType type);

protected:
   virtual void run();
   VhallIALiveSettingDlg *mParamPtr = NULL;
   CloseType mCloseType;
};

class CameraInfo
{
public:
	CameraInfo();
	CameraInfo(const QString& strId, const int& iIdex);
	//CameraInfo(const QString& strId, const int& iIdex, std::list<vhall::VideoDevProperty> DevPropertyList);
	CameraInfo(const QString& strId, const int& iIdex, std::vector<std::shared_ptr<vhall::VideoFormat>> DevPropertyList);
	CameraInfo(const CameraInfo& Info);
	~CameraInfo();
	void init(std::vector<std::shared_ptr<vhall::VideoFormat>> ver);
	QList<vhall::VideoFormat> FormatList()
	{
		return m_FormatList;
	}
	int index() {
		return m_iIdex;
	}
	QString Id() {
		return m_strId;
	}
private:
	QString m_strId = "";
	int m_iIdex=-1;
	QList<vhall::VideoFormat> m_FormatList;
};
Q_DECLARE_METATYPE(CameraInfo)
Q_DECLARE_METATYPE(vhall::VideoDevProperty)
Q_DECLARE_METATYPE(vhall::VideoFormat)

class VhallIALiveSettingDlg : public CBaseWnd
{
	Q_OBJECT

public:
	VhallIALiveSettingDlg(QWidget *parent = nullptr, bool host = false);
	~VhallIALiveSettingDlg();
	CREATE_WND();

   int GetLayOutMode();
   void SetLayOutMode(int mode);
   void SetLastLayoutMode();

   virtual void Notice(const CSTDMapParam &);
   virtual void Notice(const CSTDMapParamPtr &);
   void init();
   void RegisterListener(DeviceChangedListener* listener);
   void Apply();
   void setPushPermission(bool bPermi);
protected:
	virtual void customEvent(QEvent*);
   virtual void showEvent(QShowEvent *event);
   virtual bool eventFilter(QObject *obj, QEvent *event);
private slots:
	void slot_OnSelectAudioPage();
	void slot_OnSelectVideoPage();
	void slot_OnSelectAboutUsPage();
	void slot_OnChangeLayoutPage();
	void slot_OnClickedApply();
	//void slot_OnClickedOK();
	void slot_OnClickedClose();

	void slot_CurrentCameraSelectChanged(int index);
	void slot_CurrentMicSelectChanged(int index);
	void slot_CurrentPlayerSelectChanged(int index);

	void slot_SelectFloatMode();
	void slot_SelectTiledMode();
	void slot_SelectGridMode();
	//  void HandleGetPlayerErr();
	void slot_OnIDPage();


private:
	Ui::VhallIALiveSettingDlg ui;

	QString mCurCameraDevID;
	QString mCurMicDevID;
	QString mCurPlayerID;
   bool mbInitSuc = false;
   bool mbEnableUserSelectCamera = false;
   bool mbEnableUserSelectMic = false;
   bool mbEnableUserSelectPlayer = false;

   //bool mbGetCameraEnd = true;
   //bool mbGetMicEnd = true;
   //bool mbGetPlayerEnd = true;

   int mLastCameraIndex = -1;
   int mLastMicIndex = -1;
   int mLastPlayerIndex = -1;

   LayoutMode meLayoutMode = CANVAS_LAYOUT_PATTERN_FLOAT_6_5D;
   LayoutMode meLastLayoutMode = CANVAS_LAYOUT_PATTERN_FLOAT_6_5D;
   bool mbIsHost = false;
   NoiseTooltip* m_pNoiseTips = NULL;
   //VhallIALive* mpVhallAlive;

   DeviceChangedListener* mpDevChangedListener = NULL;

   std::list<vhall::VideoDevProperty> mcameraList;
   QMap<int, QString> mMapProfileIndex;
};


