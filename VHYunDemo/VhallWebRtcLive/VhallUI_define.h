#pragma once
#include <QString>
#include <QEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "vlive_def.h"

#define CLOSE_MIC                   QStringLiteral("�ر���˷�")
#define OPEN_MIC                    QStringLiteral("����˷�")
#define CLOSE_CAMERA                QStringLiteral("�ر�����ͷ")
#define OPEN_CAMERA                 QStringLiteral("������ͷ")
#define INVITE_USER_JOIN            QStringLiteral("��������")
#define INVITE_USER_KICK_OFF        QStringLiteral("����")
#define ENABLE_SEND_MSG             QStringLiteral("������")
#define DISABLE_SEND_MSG            QStringLiteral("����")
#define USER_KICK_OUT               QStringLiteral("�߳�")
#define LOCK_USER_VIEW              QStringLiteral("ѡ������")
#define UNLOCK_USER_VIEW            QStringLiteral("ȡ��ѡ��")
#define ALREADY_PLAY_MEDIA_FILE     QStringLiteral("���ڲ�����Ƶ�ļ������Ժ�����")
#define ALREADY_SCREEN_SHARE        QStringLiteral("��ǰ�������湲�����Ժ�����")

//class VhallLiveDeviceInfo {
//public:
//	VhallLiveDeviceInfo(QString id, QString name, int index) {
//		devId = id;
//		devName = name;
//		devIndex = index;
//	}
//
//	QString devId;		//�豸id
//	QString devName;  //����
//	int devIndex;		//����
//};

enum ResponseInviteType {
	ResponseInviteType_None = 0,
	ResponseInviteType_Accept = 1,
	ResponseInviteType_Refuse = 2,
	ResponseInviteType_Err = 3,
};


enum RenderViewState {
	RenderView_None = 0,
	RenderView_NoJoin = 1,
	RenderView_HasCamera = 2,
	RenderView_NoCamera = 3,
};

enum VhallSDKDevRequest {
	DevRequest_StartLive = 1,	//������ҳ���ȡ�豸�б�
	DevRequest_GetDevList = 2,	    //����ʱ��ȡ�豸�б�
	DevRequest_InitSDK = 3,
};

//������ɫ��ǰ���ݽ�״̬��
enum SpeakState {	 
	Host_InMainView = 1,
	Host_InListView = 2,
	Guest_InMainView = 3,
	Guest_InListView = 4,
	Other_InMainView = 5,
	Other_InListView = 6,
};


//enum BROADCAST_LAYOUT {
//	BROADCAST_LAYOUT_SPLIT_GRID = 4,   //
//	BROADCAST_LAYOUT_SPLIT_OVERLAP = 8,
//	BROADCAST_LAYOUT_SPLIT_TOPSIDE = 14,
//	BROADCAST_LAYOUT_SPLIT_BOTTOMSIDE = 13,
//	BROADCAST_LAYOUT_SPLIT_TOPSIDE_5 = 18,
//	BROADCAST_LAYOUT_SPLIT_BOTTOMSIDE_5 = 17,  //�ϲ�һ����  �ڶ������С
//	BROADCAST_LAYOUT_SPLIT_ONLYONE = 0,   //һ����
//
//   //V2.0 http://wiki.vhallops.com/pages/viewpage.action?pageId=917942
//   // ��·��������
//   CANVAS_LAYOUT_PATTERN_GRID_1 = 0,
//   CANVAS_LAYOUT_PATTERN_GRID_2_H = 1,
//   CANVAS_LAYOUT_PATTERN_GRID_3_E = 2,
//   CANVAS_LAYOUT_PATTERN_GRID_3_D = 3,
//   CANVAS_LAYOUT_PATTERN_GRID_4_M = 4,
//   CANVAS_LAYOUT_PATTERN_GRID_5_D = 5,
//   CANVAS_LAYOUT_PATTERN_GRID_6_E = 6,
//   CANVAS_LAYOUT_PATTERN_GRID_9_E = 7,
//   CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR = 8,
//   CANVAS_LAYOUT_PATTERN_FLOAT_2_1DL = 9,
//   CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL = 10,
//   CANVAS_LAYOUT_PATTERN_FLOAT_6_5D = 11,
//   CANVAS_LAYOUT_PATTERN_FLOAT_6_5T = 12,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1T4D = 13,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1D4T = 14,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1L4R = 15,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1R4L = 16,
//   CANVAS_LAYOUT_PATTERN_TILED_6_1T5D = 17,
//   CANVAS_LAYOUT_PATTERN_TILED_6_1D5T = 18,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1L8R = 19,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1R8L = 20,
//   CANVAS_LAYOUT_PATTERN_TILED_13_1L12R = 21,
//   CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID = 22,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1D8T = 23,
//   CANVAS_LAYOUT_PATTERN_TILED_13_1TL12GRID = 24,
//   CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID_E = 25,
//   CANVAS_LAYOUT_PATTERN_CUSTOM = 27
//};


//enum VhallLiveStreamType {
//	VhallLiveStreamType_Video = 2,
//	VHStreamType_Desktop = 3,
//	VHStreamType_MediaFile = 4,
//};

class IPlayMediaFileObj {
public:
   virtual bool IsPlayMediaFile() = 0;
   virtual bool IsPlayDesktopShare() = 0;
};

class DeviceChangedListener {
public:
	virtual void OnDevChanged(const QString &cameraDevId, const int cameraIndex, const QString &micDevId, const int micIndex, const QString &playerDevId, const int playerIndex, const int iLayoutMode) = 0;
};

class CustomDeleteLiveMsgEvent : public QEvent {
public:
   CustomDeleteLiveMsgEvent(QEvent::Type type, bool bExit, int reaSon ) :
		QEvent(type) {
        mbExit = bExit;
        mReason = reaSon;
	};
   bool mbExit;
   int mReason;
};


class CustomOnHttpResMsgEvent : public QEvent {
public:
   CustomOnHttpResMsgEvent(QEvent::Type type, QString url, QString msg, int code, QString uid, bool bNoticeMsg) :
      QEvent(type) {
      mUrl = url;
      mMsg = msg;
      mCode = code;
      mUid = uid;
      mbNoticeMsg = bNoticeMsg;
   };
   QString mUrl;
   QString mMsg;
   int mCode;
   QString mUid;
   bool mbNoticeMsg;
};

class CustomReConnectEvent : public QEvent {
public:
   CustomReConnectEvent(QEvent::Type type, void* ptr) :
      QEvent(type) {
      msg = ptr;
   };
   void* msg;
};

class CustomDevChangedEvent : public QEvent {
public:
	CustomDevChangedEvent(QEvent::Type type, QString cameraDevId, int cameraIndex, QString micDevId, int micIndex, QString playerDevId, int playerIndex) :
		QEvent(type) {
		this->cameraDevId = cameraDevId;
		this->cameraIndex = cameraIndex;
		this->micDevId = micDevId;
		this->micIndex = micIndex;
		this->playerDevId = playerDevId;
		this->playerIndex = playerIndex;
	};

	QString cameraDevId;
	int cameraIndex;
	QString micDevId;
	int micIndex;
	QString playerDevId;
	int playerIndex;
};


class VhallActiveJoinParam {
public:
	VhallActiveJoinParam() {
		bExitToLivelist = true;
		bIsLiving = false;
	}
	void Clear() {
		bExitToLivelist = true;
		device_audio = 0;
		device_video = 0;
		handsUp = 0;
		currPresenter.clear();
		bizRole = 0;
		reportUrl.clear();
		mainShow.clear();
		speaker_list.clear();
		invite_list.clear();
		against_url.clear();
		room_token.clear();
	}
	bool bExitToLivelist;
	bool bIsLiving;			//�����Ƿ��Ѿ���ʼֱ�� true:�Ѿ���ʼ
	int  device_audio;		//��˷�״̬1������0�ر�
	int  device_video;		//����ͷ״̬1������0�ر�
	int  handsUp;           //�Ƿ��ܾ���1���ԣ�0������
	int  bizRole;			//sdk����
	QString currPresenter;	//��ǰ�����˲λ�ID
	QString mainShow;			//��ǰ������λ�ID
	QString mainShowRole;	//��ǰ������λ��Ա�Ľ�ɫ
	QString speaker_list;	//����λ��б�
	QString invite_list;		//�ѷ������б�
	QString join_uid;			//��ǰ�û�id
	QString role_name;		//��ǰ�û���ɫ  ��ǰ�λ��ɫhost�����ˣ�guest�α���assistant����user����
	QString against_url;		//sdk����
	QString room_token;		//sdk����
	QString reportUrl;		//sdk����
	QString chatUrl;			//���� url
	QString thirdPushStreamUrl;
	QString hostNickName;
	QString hostJoinUid;
	QString msgToken;
	QString userNickName;
	QString imageUrl;		 //ͷ��url
	QString streamName;	 //�ID	��366476354
	QString webinarName;	 //����ƣ����� Testֱ������
	QString pluginUrl;	 //�ĵ����url
   QString watchLayout; //���沼�֡�
};

class SpeakUser {
public:
	SpeakUser(){}
	SpeakUser(QString id, /*QString role, QString name,*/int audio = 1, int video = 1) {
		userId = id;
		//roleName = role;
		//nickName = name;
      nAudio = audio;
      nVideo = video;
	}
	QString userId;
	//QString roleName;
	//QString nickName;
   int nAudio;
   int nVideo;
	bool bOffLine = false;
	unsigned long long offLineTime;
};

