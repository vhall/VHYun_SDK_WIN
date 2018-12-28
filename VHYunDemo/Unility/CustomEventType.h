#pragma once
#include <QEvent>
#include <QString>

const QEvent::Type  CustomEvent_Login = (QEvent::Type)5000;
const QEvent::Type  CustomEvent_LoginErr = (QEvent::Type)5001;
const QEvent::Type  CustomEvent_ShowLogin = (QEvent::Type)5002;

const QEvent::Type  CustomEvent_WebRtcRoomConnetSuc = (QEvent::Type)7000;
const QEvent::Type  CustomEvent_WebRtcRoomConnetErr = (QEvent::Type)7001;
const QEvent::Type  CustomEvent_WebRtcRoomReConneting = (QEvent::Type)7002;
const QEvent::Type  CustomEvent_WebRtcRoomReConneted = (QEvent::Type)7003;
const QEvent::Type  CustomEvent_WebRtcRoomRecover = (QEvent::Type)7023;
const QEvent::Type  CustomEvent_WebRtcRoomNetWorkChanged = (QEvent::Type)7024;
const QEvent::Type  CustomEvent_WebRtcRoomNetWorkChangedCover = (QEvent::Type)7025;
const QEvent::Type  CustomEvent_WebRtcRoomDisConnected = (QEvent::Type)7026;
const QEvent::Type  CustomEvent_WebRtcRoomMixedReady = (QEvent::Type)7027;
const QEvent::Type  CustomEvent_GetVHMemberList = (QEvent::Type)7029;
const QEvent::Type  CustomEvent_GetVHKickOutMemberList = (QEvent::Type)7030;
const QEvent::Type  CustomEvent_OnRecvApplyInavPublishMsg = (QEvent::Type)7031;
const QEvent::Type  CustomEvent_OnRecvAuditInavPublishMsg = (QEvent::Type)7032;
const QEvent::Type  CustomEvent_OnRecvAskforInavPublishMsg = (QEvent::Type)7033;
const QEvent::Type  CustomEvent_OnRecvKickInavStreamMsg = (QEvent::Type)7034;
const QEvent::Type  CustomEvent_OnRecvKickInavMsg = (QEvent::Type)7035;
const QEvent::Type  CustomEvent_OnUserPublishCallback = (QEvent::Type)7036;
const QEvent::Type  CustomEvent_OnRecvInavCloseMsg = (QEvent::Type)7037;
const QEvent::Type  CustomEvent_OnErrorRoomEvent = (QEvent::Type)7038;
const QEvent::Type  CustomEvent_OnSuccessRoomEvent = (QEvent::Type)7039;
const QEvent::Type  CustomEvent_GetLocalStreamSuc = (QEvent::Type)30003;
const QEvent::Type  CustomEvent_DevChanged = (QEvent::Type)30007;
const QEvent::Type  CustomEvent_SubScribSuc = (QEvent::Type)30010;
const QEvent::Type  CustomEvent_STREAM_REMOVED = (QEvent::Type)30013;
const QEvent::Type  CustomEvent_PushLocalStreamSuc = (QEvent::Type)30015;
const QEvent::Type  CustomEvent_PushLocalStreamError = (QEvent::Type)30016;
const QEvent::Type  CustomEvent_StopPushLocalStreamSuc = (QEvent::Type)30070;
const QEvent::Type  CustomEvent_DestoryWnd = (QEvent::Type)30071;
const QEvent::Type  CustomEvent_KICKOUT_ROOM = (QEvent::Type)30072;
const QEvent::Type  CustomEvent_CHANGE_LAYOUT = (QEvent::Type)30073;
const QEvent::Type  CustomEvent_User_OnLine = (QEvent::Type)30074;
const QEvent::Type  CustomEvent_StopPushLocalStreamErr = (QEvent::Type)30075;
class CustomRecvMsgEvent : public QEvent {
public:
	CustomRecvMsgEvent(QEvent::Type type, QString data, bool hasVideo = true) :
		QEvent(type) {
		msg = data;
		mbHasVideo = hasVideo;
	};
	QString msg;
	bool mbHasVideo;
};

class QEventStream : public QEvent {
public:
    QEventStream(QEvent::Type type, std::string userid, std::string streamId, int mediaType, int iVhCapture, bool hasVideo = true, bool hasAudio = true) :
        QEvent(type) {
        strUser = userid;
        strStream = streamId;
        mStreamType = mediaType;
		mIVHCapture = iVhCapture;
        mbHasVideo = hasVideo;
        mbHasAudio = hasAudio;
    };
    std::string strUser;
    std::string strStream;
    int mStreamType;    
	int mIVHCapture;//VHCapture
    bool mbHasVideo;        //流中是否包含视频 true是
    bool mbHasAudio;
};

class QEventStreamError : public QEvent {
public:
	QEventStreamError(QEvent::Type type, int streamType, const int codeErr, const std::string& msg) :
		QEvent(type) {
		iCodeErr = codeErr;
		mStreamType = streamType;
		strMsg = msg;
	};
	int iCodeErr;
	int mStreamType;
	std::string strMsg;
};

class QEventVHMember : public QEvent {
public:
    QEventVHMember(QEvent::Type type,std::string userid, int status = -1) :
        QEvent(type) {
        thirdPartyUserId = userid;
        memberStatusType = status;
    };
    std::string thirdPartyUserId;
    int memberStatusType;
};

class QEventUserPublish : public QEvent {
public:
    QEventUserPublish(QEvent::Type type, std::string userid, std::string stream_id, int status = -1) :
        QEvent(type) {
        thirdPartyUserId = userid;
        strStreamId = stream_id;
        memberStatusType = status;
    };
    std::string thirdPartyUserId;
    std::string strStreamId;
    int memberStatusType;
};

class QEventDestoryWnd : public QEvent {
public:
    QEventDestoryWnd(QEvent::Type type, QString wndId) :
        QEvent(type) {
        mWndId = wndId;
    };
    QString mWndId;
};

class QEventRoomEvent : public QEvent {
public:
    QEventRoomEvent(QEvent::Type type,int cbType, std::string userId = std::string(), int processType = 0, int respCode = 0, std::string resp = std::string()) :
        QEvent(type) {
        mRespCode = respCode;
        mRespMsg = resp;
        mCallBackCodeType = cbType;
        mUserId = userId;
        mProcessType = processType;
    };

    std::string mRespMsg;//请求响应信息
    int mCallBackCodeType;
    int mRespCode;
    std::string mUserId;
    int mProcessType;
};


class QEventConfigBroadCast : public QEvent {
public:
    QEventConfigBroadCast(QEvent::Type type, std::string msg) :
        QEvent(type) {
        strMsg = msg;
    };
    std::string strMsg;
};

class QEventPublishStream : public QEvent {
public:
    QEventPublishStream(QEvent::Type type, std::string msg) :
        QEvent(type) {
        strStream = msg;
    };
    std::string strStream;
};

class CustomEventType : public QEvent {
public:
    CustomEventType(QEvent::Type type, QString data);
public:
    QString msg;
};

