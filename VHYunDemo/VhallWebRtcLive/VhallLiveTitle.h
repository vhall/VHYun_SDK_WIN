#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "ui_VhallLiveTitle.h"
enum TitleType
{
	eVhallIALiveNone = 0,
	eVhallIALiveHost = 1,
    eVhallIALiveGuest = 2,
	eVhallIALiveSetting = 3,
};

class VhallLiveTitle : public QWidget
{
	Q_OBJECT

public:
	VhallLiveTitle(QWidget *parent = Q_NULLPTR);
	~VhallLiveTitle();

	void SetTitleType(TitleType type);
	void SetLiveInfo(const QString& streamID, const QString& roomName, const QString& userName);
	void SetUserImageUrl(const QString& url);
   void SetEnableCloseBtn(bool enable);
   void SetHideMaxBtn(bool hide);
private slots:
	void slot_OnClose();
	void slot_OnFresh();
	void slot_OnFullSize();
	void slot_OnMinSize();
	void slot_OnSetting();
	void slot_OnShare();

signals:
	void sig_ClickedClose();
	void sig_ClickedFresh();
	void sig_ClickedFullSize();
	void sig_ClickedMinSize();
	void sig_ClickedSetting();
	void sig_ClickedShare();

	void signal_move(QPoint point);

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	Ui::VhallLiveTitle ui;
private:
	QPoint mPressPoint;
	bool mIsMoved = false;
	bool m_bIsEnableMove = true;
};
