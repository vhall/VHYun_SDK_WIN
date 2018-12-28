#include "VhallLiveTitle.h"

VhallLiveTitle::VhallLiveTitle(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.pushButton_close->hide();
	ui.pushButton_fresh->hide();
	ui.pushButton_fullSize->hide();
	ui.pushButton_minSize->hide();
	ui.pushButton_setting->hide();
	ui.pushButton_share->hide();
	ui.label_logo->hide();
	ui.pushButton_close->loadPixmap(":/sysButton/close_button");
	ui.pushButton_fresh->loadPixmap(":/sysButton/refresh_button");
	ui.pushButton_fullSize->loadPixmap(":/sysButton/max_button");
	ui.pushButton_minSize->loadPixmap(":/sysButton/min_button");
	ui.pushButton_setting->loadPixmap(":/sysButton/main_setting");
	ui.pushButton_share->loadPixmap(":/sysButton/shared_button");

	connect(ui.pushButton_close,SIGNAL(sigClicked()),this,SLOT(slot_OnClose()));
	connect(ui.pushButton_fresh, SIGNAL(sigClicked()), this, SLOT(slot_OnFresh()));
	connect(ui.pushButton_fullSize, SIGNAL(sigClicked()), this, SLOT(slot_OnFullSize()));
	connect(ui.pushButton_minSize, SIGNAL(sigClicked()), this, SLOT(slot_OnMinSize()));
	connect(ui.pushButton_setting, SIGNAL(sigClicked()), this, SLOT(slot_OnSetting()));
	connect(ui.pushButton_share, SIGNAL(sigClicked()), this, SLOT(slot_OnShare()));

	ui.pushButton_close->setToolTip(QStringLiteral("关闭"));
	ui.pushButton_fresh->setToolTip(QStringLiteral("刷新"));
	ui.pushButton_fullSize->setToolTip(QStringLiteral("最大化"));
	ui.pushButton_minSize->setToolTip(QStringLiteral("最小化"));
	ui.pushButton_setting->setToolTip(QStringLiteral("设置"));
	ui.pushButton_share->setToolTip(QStringLiteral("分享"));
}

VhallLiveTitle::~VhallLiveTitle()
{
}


void VhallLiveTitle::SetEnableCloseBtn(bool enable) {
   ui.pushButton_close->setEnabled(enable);
}

void VhallLiveTitle::SetLiveInfo(const QString& streamID, const QString& roomName, const QString& userName) {
	ui.label_userName->setText(userName);
}

void VhallLiveTitle::SetHideMaxBtn(bool hide) {
   ui.pushButton_fullSize->setHidden(hide);
}

void VhallLiveTitle::SetTitleType(TitleType type) {
	switch (type)
	{
	case eVhallIALiveNone: {
		this->setStyleSheet("background-color: rgb(43, 44, 46);");
		ui.pushButton_close->hide();
		ui.pushButton_fresh->hide();
		ui.pushButton_fullSize->hide();
		ui.pushButton_minSize->hide();
		ui.pushButton_setting->hide();
		ui.label_logo->hide();
		ui.pushButton_share->hide();
		ui.label_userName->hide();
	}
		break;
	case eVhallIALiveHost: 
	case eVhallIALiveGuest:{
		this->setStyleSheet("background-color: rgb(43, 44, 46);");
		ui.pushButton_close->show();
		ui.pushButton_fullSize->show();
		ui.pushButton_minSize->show();
		ui.pushButton_setting->show();
		ui.label_userName->show();
        ui.label_logo->show();
	}
		break;
	case eVhallIALiveSetting: {
		this->setStyleSheet("background-color: rgb(43, 44, 46);");
		ui.pushButton_close->show();
		ui.pushButton_fresh->hide();
		ui.pushButton_fullSize->hide();
		ui.pushButton_minSize->hide();
		ui.pushButton_setting->hide();
		ui.label_logo->hide();
		ui.pushButton_share->hide();	
		ui.label_userName->hide();
	}
		break;
	default:
		break;
	}
}

void VhallLiveTitle::slot_OnClose() {
	emit sig_ClickedClose();
}

void VhallLiveTitle::slot_OnFresh() {
	emit sig_ClickedFresh();
}

void VhallLiveTitle::slot_OnFullSize() {
	emit sig_ClickedFullSize();
}

void VhallLiveTitle::slot_OnMinSize() {
	emit sig_ClickedMinSize();
}

void VhallLiveTitle::slot_OnSetting() {
	emit sig_ClickedSetting();
}

void VhallLiveTitle::slot_OnShare() {
	emit sig_ClickedShare();
}


void VhallLiveTitle::mousePressEvent(QMouseEvent *event) {
	if (event) {
		mPressPoint = event->pos();
		mIsMoved = true;
	}
}

void VhallLiveTitle::mouseMoveEvent(QMouseEvent *event) {
	if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
		QWidget* parent_widget = this->nativeParentWidget();
		QString objectName = parent_widget->objectName();
		QPoint parent_point = parent_widget->pos();
		parent_point.setX(parent_point.x() + event->x() - mPressPoint.x());
		parent_point.setY(parent_point.y() + event->y() - mPressPoint.y());
		parent_widget->move(parent_point);
		emit signal_move(parent_point);
	}
}

void VhallLiveTitle::mouseReleaseEvent(QMouseEvent *) {
	if (mIsMoved) {
		mIsMoved = false;
	}
}

