#include "ByPassRoom.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QJsonObject>
#include "VhallWebRtcLive.h"
#include "../WebRtcLiveWdg.h"

#define MAX_TIME_OUT 15

CREATE_WND_FUNCTION(ByPassRoomWdg);
ByPassRoomWdg::ByPassRoomWdg(QWidget *parent)
: CBaseWnd(parent)
{
    ui.setupUi(this);
    mnMaxTimeOut = MAX_TIME_OUT;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    //m_pTimer = new QTimer(this);
    //connect(m_pTimer,SIGNAL(timeout()),this,SLOT(Slot_ShowTimeOut()));
    connect(ui.pushButtonAgree, SIGNAL(clicked()), this, SLOT(Slot_GuestAgreeHostInvite()));
    connect(ui.toolButtonRefuse, SIGNAL(clicked()), this, SLOT(Slot_GuestRefuseHostInvite()));   
    //ui.toolButtonAgree->setStyleSheet(
    //    "QToolButton{border-image: url(\":/interactivity/replyYes\");}"
    //    "QToolButton:hover{border-image: url(\":/interactivity/replyYesHover\");}"
    //    "QToolButton:pressed {border-image: url(\":/interactivity/replyYes\");}"
    //    "QToolButton:disabled {border-image: url(\":/interactivity/replyYes\");}"
    //);

    //ui.toolButtonRefuse->setStyleSheet(
    //    "QToolButton{border-image: url(\":/interactivity/replyNo\");}"
    //    "QToolButton:hover{border-image: url(\":/interactivity/replyNoHover\");}"
    //    "QToolButton:pressed {border-image: url(\":/interactivity/replyNo\");}"
    //    "QToolButton:disabled {border-image: url(\":/interactivity/replyNo\");}"
    //);
   
    mpixmap = QPixmap(":/interactivity/askBackground");

    m_pCloseBtn = new TitleButton();
    m_pCloseBtn->loadPixmap(":/sysButton/close_button");
    connect(m_pCloseBtn,SIGNAL(clicked()),this,SLOT(Slot_GuestRefuseHostInvite()));
    ui.layout_close->addWidget(m_pCloseBtn);
    ui.widget_title->installEventFilter(this);
}

ByPassRoomWdg::~ByPassRoomWdg() {
}

void ByPassRoomWdg::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find(USER_ID);
    if (iter != mapParam.end()) {
        SetUid(iter->second);
    }
}

void ByPassRoomWdg::Notice(const CSTDMapParamPtr &) {

}

void ByPassRoomWdg::InitTimeOut(int timeout) {
	mnMaxTimeOut = timeout;
}

QString ByPassRoomWdg::GetUid()
{
	return mStrUid;
}

int ByPassRoomWdg::exec()
{
	QString strId = globalToolManager->GetDataManager()->GetLiveRoomId();
	ui.lineEdit->setText(strId);
	return CBaseWnd::exec();
}

void ByPassRoomWdg::SetUid(const QString& strUid)
{
	mStrUid = strUid;
}

void ByPassRoomWdg::CenterWindow(QWidget* parent) {
   int x = 0;
   int y = 0;
   if (NULL == parent) {
      const QRect rect = QApplication::desktop()->availableGeometry();
      x = rect.left() + (rect.width() - width()) / 2;
      y = rect.top() + (rect.height() - height()) / 2;
   } else {
      QPoint point(0, 0);
      point = parent->mapToGlobal(point);
      x = point.x() + (parent->width() - width()) / 2;
      y = point.y() + (parent->height() - height()) / 2;
   }
   move(x, y);
}


void ByPassRoomWdg::Slot_GuestAgreeHostInvite() {
    //if (m_pTimer) {
    //    m_pTimer->stop();
    //}
	//CBaseWnd* pRtcLiveWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	//((WebRtcLiveWdg*)pRtcLiveWdg)->SetAgreePush(true);
 //   //emit Sig_GuestAgreeHostInvite();
 //   //ͬ��������Ҫ����֮���͡�
 //   globalToolManager->GetDataManager()->WriteLog("%s StartPushLocalStream user id:%s", __FUNCTION__, mStrUid.toStdWString().c_str());
 //   globalToolManager->GetPaasSDK()->StartPushLocalStream();
 //   globalToolManager->GetWndManager()->DestoryWnd(WND_ID_INVITE_TO_SPEAK + mStrUid);

	globalToolManager->GetDataManager()->SetLiveRoomId( ui.lineEdit->text().trimmed() );
	this->accept();
}

void ByPassRoomWdg::Slot_GuestRefuseHostInvite() {
    //if (m_pTimer) {
    //    m_pTimer->stop();
    //}
    //emit Sig_GuestRefuseHostInvite();
    //�ܾ�����
    //globalToolManager->GetDataManager()->WriteLog("%s PushStreamEvent_Refused user id:%s", __FUNCTION__, mStrUid.toStdWString().c_str());
    //globalToolManager->GetPaasSDK()->UserPublishCallback("", PushStreamEvent_Refused);
    //globalToolManager->GetWndManager()->DestoryWnd(WND_ID_INVITE_TO_SPEAK + mStrUid);
	reject();
}
void ByPassRoomWdg::paintEvent(QPaintEvent *){
   QPainter painter(this);
   painter.drawPixmap(rect(),this->mpixmap);
}

bool ByPassRoomWdg::eventFilter(QObject *o, QEvent *e) {
    if(o==ui.widget_title)
    {
       if(e->type()==QEvent::MouseButtonPress) {
           this->pressPoint=this->cursor().pos();
           this->startPoint=this->pos();
       }
       else if(e->type()==QEvent::MouseMove) {
           int dx=this->cursor().pos().x()-this->pressPoint.x();
           int dy=this->cursor().pos().y()-this->pressPoint.y();
           this->move(this->startPoint.x()+dx,this->startPoint.y()+dy);
       }
    }
    return QWidget::eventFilter(o,e);
}

//void ByPassRoomWdg::Slot_ShowTimeOut() {
//   if (nTimeCount == 0) {
//      emit Sig_GuestRefuseHostInvite();
//      m_pTimer->stop();
//   } else {
//      QString notice = QStringLiteral("%1s���Զ��ܾ�").arg(nTimeCount);
//      ui.label_TimeNotice->setText(notice);
//      nTimeCount--;
//   }
//}

//void ByPassRoomWdg::showEvent(QShowEvent *) {
//   if (m_pTimer) {
//        m_pTimer->stop();
//        m_pTimer->start(1000);
//   }
//   ui.label_TimeNotice->setText(QStringLiteral("%1s���Զ��ܾ�").arg(mnMaxTimeOut));
//   nTimeCount = mnMaxTimeOut;
//}


