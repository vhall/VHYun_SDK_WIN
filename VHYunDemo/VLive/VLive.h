#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VLive.h"

class VLive : public QMainWindow
{
    Q_OBJECT

public:
    VLive(QWidget *parent = Q_NULLPTR);

private:
    Ui::VLiveClass ui;
};
