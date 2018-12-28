#include "LoadingWdg.h"
#include <QMovie>

LoadingWdg::LoadingWdg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
    //QMovie *movie = new QMovie(":/VLive/Resources/loading.gif");
    //ui.label->setMovie(movie);
    //movie->start();
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept()));
}

LoadingWdg::~LoadingWdg()
{

}

CBaseWnd* LoadingWdg::CreateFunction() {
    return new LoadingWdg();
}

void LoadingWdg::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find("msg");
    if (iter != mapParam.end()) {
        ui.label->setText(iter->second);
    }
}