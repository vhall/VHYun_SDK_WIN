#include "VLive.h"
#include <QtWidgets/QApplication>
#include "../Unility/Unility.h"
#include "LoginWdg.h"
#include "LoadingWdg.h"
#include "VLiveCustomEvent.h"
#include "ExceptionDump.h"

#include <QDebug>
#include <QFile>
std::wstring gCurrentVersion = L"0.0.0.0";
std::string gCurStreamID = "0";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //¼ÓÔØqss
    //QFile qss(":/Style.qss");
    //if (qss.open(QFile::ReadOnly)) {
    //    a.setStyleSheet(qss.readAll());
    //    qss.close();
    //}

    a.setWindowIcon(QIcon(":/login/vlive.ico"));
    
	CExceptionDump g_oDmpFile;
    VLiveCustomEvent eventReciver;
    ToolManager::GetInstance()->Init();
     
    ToolManager::GetInstance()->GetLibManager()->InitDll();
    ToolManager::GetInstance()->RegisterQEventReciver(&eventReciver);

    ToolManager::GetInstance()->GetWndManager()->RegisterWndFactory(WND_ID_LOGIN, REGISTER_WND(LoginWdg));
    ToolManager::GetInstance()->GetWndManager()->RegisterWndFactory(WND_ID_LOADING, REGISTER_WND(LoadingWdg));
    CBaseWnd* wnd1 = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
    wnd1->show();
    //a.exec(); 
    ToolManager::GetInstance()->AppExec();
    //ToolManager::GetInstance()->AppQuit();

    ToolManager::GetInstance()->Release();
    ToolManager::ReleaseInstance();
    return 0;
}
