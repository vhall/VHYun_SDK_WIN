#include "VhallRenderMaskWdg.h"
#include <QPainter>

VhallRenderMaskWdg::VhallRenderMaskWdg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);
}

VhallRenderMaskWdg::~VhallRenderMaskWdg()
{

}

void VhallRenderMaskWdg::SetUserName(const QString &userName) {
   ui.label_userName->setText(userName);
}

void VhallRenderMaskWdg::SetResetSize(bool mainView) {
	if (mainView) {
      ui.label_roleLogo->setFixedSize(QSize(20, 22));
      this->setFixedWidth(175);
    }
    else {
      ui.label_roleLogo->setFixedSize(QSize(10, 11));
      this->setFixedWidth(158);
    }
}

