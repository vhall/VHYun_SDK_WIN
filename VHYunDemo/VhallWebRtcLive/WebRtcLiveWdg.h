#ifndef H_WEBRTCLIVEWDG_H
#define H_WEBRTCLIVEWDG_H




#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QDateTime>

#include "ui_WebRtcLiveWdg.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"

class  CScreenShareToolWgd;
class VedioPlayUI;
class VideoChoiceUI;
class VHFadeOutTipWidget;
class VhallIALiveSettingDlg;
class VhallRenderWdg;
class VhallWaiting;
class VHRoomMemberWdg;
class ApplyJoinWdg;
class InviteJoinWidget;
class AudioSliderUI;
class ByPassRoomWdg;

class WebRtcLiveWdg : public CBaseWnd, public DeviceChangedListener
{
    Q_OBJECT

public:
    WebRtcLiveWdg(QWidget *parent = Q_NULLPTR);
    ~WebRtcLiveWdg();
    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
    CREATE_WND();

	void Init();

	virtual bool IsPlayMediaFile();
	virtual bool IsPlayDesktopShare();

	virtual void OnDevChanged(const QString &cameraDevId, const int cameraIndex, const QString &micDevId,  const int micIndex, const QString &playerDevId, const int playerIndex, const int iLayoutMode);
    virtual void Notice(QEvent* event);

	void ClearAllOnLineUser();
	void ClearAllKickOutUSer();
	void SetAgreePush(bool bAgreePush);
protected:
    virtual void customEvent(QEvent *);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void moveEvent(QMoveEvent *event);
private:
	bool isPushing();
	void calVedioPlayUISize();
	void CreateScreenShareWgd();
	//添加远端视频成员窗体
	int AppendRenderUser(const QString& uid, /*const QString &role, const QString& nickName, */
		const bool audio, const bool video, bool showInMainView, bool show = true);
	VhallRenderWdg *GetRenderWnd(const QString& uid);

	//房间连接
    void HandleWebRtcRoomConnetSuc();
    void HandleWebRtcRoomConnetErr();
	//房间重连
    void HandleWebRtcRoomReConneting();
    void HandleWebRtcRoomReConneted();   
    //网络异常
    void HandleWebRtcRoomRecover();
    void HandleWebRtcRoomNetWorkChangedCover();
    void HandleWebRtcRoomNetWorkChanged();


    void HandleRecvKickInvaMsg(QEvent* event);

	void HandleInitVhallSDK();

	void HandleDeviceEvent(QEvent* event);
    void HandleGetVHMemberList(QEvent* event);
    void HandleUserPublishCallback(QEvent* event);
    void HandleGetKickOutMemberList(QEvent* event);
    void HandleErrorEventRoomEvent(QEvent* event);
    void HandleSuccessEventRoomEvent(QEvent* event);
	void HandleSubScribSucEvent(QEvent* event);
	void HandleStartLiving();
	void HandleGetStream(QEvent* event);
	void HandleGetLocalStream(QEventStream* msgEvent);
	void HandleGetDesktopStream(QEventStream* msgEvent);
	void HandleGetMediaFileStream(QEventStream* msgEvent);

	void HandlePushLocalStreamSuc(QEvent* event);
	void HandlePushLocalStreamErro(QEvent* event);
	void HandleStopPushLocalStreamSuc(QEvent* event);
	void HandleStopLiving();
	void HandleRecvApplyInavPublishMsg(QEvent* event);
	void HandleRecvAuditInavPublishMsg(QEvent* event);
	void HandleStreamRemoved(QEvent* event);
    void HandleUserOnLine(QEvent* event);
	//开启旁路直播成功
	void HandlePublishInavAnother(QEvent* event);
	//开启旁路直播失败
	void HandlePublishInavAnotherError(QEvent*event);//RoomEvent_Start_PublishInavAnother

	//结束旁路直播成功
	void HandleStopPublishInavAnother(QEvent* event);//RoomEvent_Stop_PublishInavAnother
	void HandleLogout(QEvent* roomEvent);
	//结束旁路直播失败
	void HandleStopPublishInavAnotherError(QEvent*event);

    void HandleRecvAskForInav(QEvent* event);

	void ForceHide(bool bHide);
	void ReposVedioPlay(bool bShow);

	void  FadeOutTip(QString str);

    void HandleAskforInavPublish(QEventRoomEvent* roomEvent, bool sucessed);
    void HandleKickInavStream(QEventRoomEvent* roomEvent, bool sucessed);
    void HandleAuditInavPublish(QEventRoomEvent* roomEvent, bool sucessed);
    void HandleKickInva(QEventRoomEvent* roomEvent, bool sucessed);
	void RemoveAllRender();
	void StopAllRenderStream();
	
	//控制打开桌面共享页面
	void OpenScreenShare();
	//void CloseScreenShare();

	VHRoomMemberWdg* GetRoomMemberFromOnLineList(QString userId);
    VHRoomMemberWdg* GetRoomMemberFromKickOutLineList(QString userId);
    void RemoveMemberFromOnLineUser(QString userId);
    void RemoveMemberFromKickOutUser(QString userId);

    void AddMemeberIntoOnLineList(QString userId,int state);
    void AddMemberIntoKickOutLineList(QString userId, int state);
	void MediaFilePlayFaild();
private slots:
	
	void slot_ScreenShareStartLive();
	void slot_OnClose();
	void slot_OnFresh();
	void slot_OnFullSize();
	void slot_OnMinSize();
	void slot_OnSetting();
	void slot_OnShare();

    void slot_joinroom();
    void slot_exitroom();

    void slot_showMemberList();
    void slot_RePublishLocalStreamTimeout();
    void slot_RePublishDesktopStreamTimeout();
    void slot_RePublishMediaFileStreamTimeout();

    void slot_ReSetMediaFileMainViewTimer();
    void slot_ReSetDesktopMainViewTimer();

	void slot_CameraBtnClicked();
	void slot_MicButtonClicked();
	void slot_PlayerButtonClicked();
	void slot_BtnScreenClicked();
	void slot_BtnStopScreenClicked();
	void slot_BtnStreamClicked();
	void slot_MicButtonEnter();
	void slot_MicButtonLeave();
	void slot_PlayerButtonEnter();
	void slot_PlayerButtonLeave();
	void slot_OnPlayFileClicked();
	void slot_OnToSpeakClicked();
    void slot_OnTurnToOnLinePage();
    void slot_OnTurnToKickOutPage();
	void SlotRepos(bool bRePos);
	void SlotForceHide(bool bHide);
	void Slot_VedioPlayClick();
	void OnCyclePlay();
	void PlaySelectedFile();
	void StopPlayFile();
	void AddPlayFiles();
	void slot_EnableSpeak();
	void slot_MicVolumnChanged(int volume);
	void slot_PlayerVolumnChanged(int volume);
	void slot_OnClickedCamera(QString id, bool curState);
    void slot_setToSpeaker(QString userId);
	void slot_ClickedMic(QString, bool);
    void SetMainViewMicState(const QString userId ,bool open);
    void SetMainViewCameraState(const QString userId, bool open);
    void slot_SetMainWdgRender(QString);
	void slot_PublishInavAnotherClicked();
	void slo_ShowVedioPlayWidget();
	void ShowVedioPlayWidget(BOOL bShow);
	void slotDealStopPlay();

	void on_pushButton_preViousView_clicked(bool checked = false);
	void on_pushButton_nextView_clicked(bool checked = false);
	void slot_move(QPoint point);
private:
	void RemoveMediaFileStreamMsg(const QString& uid);
	void RemoveDesktopStreamMsg(const QString& uid);
	void RemoveLocalStreamMsg(const QString& uid);
	void CreatePlayMediaFileUI();
	void CreateSettingDlg(const bool& bShow = true);
	//bool IsEnableUsing();
	bool IsEnalbePlayMedia();
	
	void DealStopPlay(void* apData, DWORD adwLen);
	void HandleVhallActiveCyclePlay();
	QString dulation2Str(long long currDulation, long long maxDulation);
	QString dulation2StrPri(long long dulation);
	void SetPlayMeidaFileVolume(const int volume);
	int OpenPlayMediaFile(const char *szMediaFile);
	int InitMediaStream(const char *szMediaFile);

	bool IsExistRenderWnd(const QString& uid);
	void AddRenderToMap(const QString &id, VhallRenderWdg*render);
    int GetRenderSize();
	void SetSpeakUserOffLine(const QString &uid, bool offline);

    void SetMainWdgRenderCameraUser(const QString& user_id);
    void SetPlayDeskTopAndMediaFileState(bool playing);

	void ResetPlayMediaFileAndDesktopShare();
	int GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos);
	void StopShowMediaVideo();

	void StopPlayMediaFile();
	void ChangeLocalMicDevState(bool open, const QString & uid);
	void openMic(bool bOpen);

private:
	void stopPushStream();

    Ui::WebRtcLiveWdg ui;
	VedioPlayUI* m_pVedioPlayUI = NULL;
	VideoChoiceUI *m_pVideoChoiceUI = NULL;
	//循环播放定时器
	QTimer m_qCycleTimer;
	VHFadeOutTipWidget *mpFadeOutTipWidget = NULL;
	VhallIALiveSettingDlg *mpSettingDlg = NULL;
	bool m_bPlayCurrentFile = false;
	bool mbEnableStopStream = true; //此标志位是用于判断开始桌面共享时GetLocalStream是否获取完毕。

	//InviteJoinWidget* mpInviteJoinWidget = nullptr;

    bool mIsFullSize;

	QString mCurMicDevID;
	QString mCurCameraDevID;
	QString mCurPlayerID;

	QMutex mRenderViewMutex;
	QMutex mStreamMutexCS;

	QMap<QString, VhallRenderWdg*> mRenderMap;
	//QWidget *mMediaFilePlayWdg = NULL;
	//QWidget *mDesktopWdg = NULL;
	//QWidget *mDesktopWdg = NULL;

	QMutex mSpeakUserMapMutex;
	std::map<QString, SpeakUser*> mSpeakUserMap;
	
	QMutex mDeviceMutex;
	QTimer* mpEnableSpeakTimer = NULL;  //限制频繁上下麦

	QString mLocalStreamId;//本地流id
    QString mPublishMainViewStreamId;

	QString mStrStreamId[VHStreamType_Count];

	bool mbIsDeskTop = false;
	bool mbEnableStopDesktopStream = true;  //此标志位是用于判断开始桌面共享时GetLocalStream是否获取完毕。
	//bool m_bIsDesktopSharCapturing = false;//桌面共享采集中
	//bool m_bIsMediaPlaying = false;			//视频插播播放中

	AudioSliderUI *mpMicVolumn = NULL;
	AudioSliderUI *mpPlayerVolumn = NULL;
	//int mMicVolume;
	bool m_iPublishInavAnother;
	bool m_IsPushingLocalStream = false;
	bool m_bClose = false;
    QString mCurMainShowUid;//当前大屏显示的用户

	CScreenShareToolWgd* m_pScreenShareToolWgd = NULL;

	ByPassRoomWdg* m_pPassRoomWdg = NULL;

    QTimer mRePublishLocalStreamTimer;
    QTimer mRePublishDesktopStreamTimer;
    QTimer mRePublishMediaFileStreamTimer;

    QTimer mReSetMediaFileMainViewTimer;
    QTimer mReSetDesktopMainViewTimer;

	int miLayoutMod = CANVAS_LAYOUT_PATTERN_FLOAT_6_5D;
	//bool m_SelfStopPush = false;
	bool m_AgreePush = false;
	bool mbStopLive = false;
};
#endif // !H_WEBRTCLIVEWDG_H