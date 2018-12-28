#pragma once

#include <QWidget>
#include "ui_LoadingWdg.h"
#include "../Unility/Unility.h"
#include <QDialog>
class LoadingWdg : public CBaseWnd
{
    Q_OBJECT

public:
    LoadingWdg(QWidget *parent = Q_NULLPTR);
    ~LoadingWdg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
private:
    Ui::LoadingWdg ui;
};
