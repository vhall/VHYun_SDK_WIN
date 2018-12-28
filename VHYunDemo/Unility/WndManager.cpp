#include "WndManager.h"
#include "Unility.h"

CBaseWnd::CBaseWnd(QWidget *parent) :
    QDialog(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
}

CBaseWnd::~CBaseWnd() {

}

void CBaseWnd::showEvent(QShowEvent *event) {
    this->setAttribute(Qt::WA_Mapped);
	QDialog::showEvent(event);
}

WndManager::WndManager(QObject *parent)
    : QObject(parent)
{
}

WndManager::~WndManager()
{

}

void WndManager::RegisterWndFactory(QString wndId, CreateFunction fun) {
   std::unique_lock<std::mutex> lock(mWndMutex);
   std::map<QString, CreateFunction>::iterator iter = mWndFactory.find(wndId);
   if (iter == mWndFactory.end()) {
      mWndFactory.insert(std::map<QString, CreateFunction>::value_type(wndId, fun));
   }
}

CBaseWnd* WndManager::FindWnd(QString wndId, QString strWndIndex) {
   CBaseWnd* wnd = nullptr;
   std::unique_lock<std::mutex> lock(mWndMutex);
   QString createId = wndId + strWndIndex;
   std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(createId);
   if (iterWndMap != mWndMap.end()) {
      return iterWndMap->second;
   }
   std::map<QString, CreateFunction>::iterator iter = mWndFactory.find(wndId);
   if (iter != mWndFactory.end()) {
      wnd = (CBaseWnd*)(iter->second)();
   }
   else {
      return nullptr;
   }
   std::map<QString, CBaseWnd*>::iterator iterWndMapInsert = mWndMap.find(createId);
   if (iterWndMapInsert == mWndMap.end()) {
      mWndMap.insert(std::map<QString, CBaseWnd*>::value_type(createId, wnd));
   }
   return wnd;
}

bool WndManager::IsWndExist(QString wndId) {
    CBaseWnd* wnd = nullptr;
    std::unique_lock<std::mutex> lock(mWndMutex);
    QString createId = wndId;
    std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(createId);
    while (iterWndMap != mWndMap.end()) {
        if (iterWndMap->first.contains(createId)) {
            return true;
        }
    }
    return false;
}

bool WndManager::HideWnd(QString wndId) {
    std::unique_lock<std::mutex> lock(mWndMutex);
    std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
    if (iterWndMap != mWndMap.end()) {
        iterWndMap->second->hide();
        return true;
    }
    return false;
}

bool WndManager::ShowWnd(QString wndId) {
    std::unique_lock<std::mutex> lock(mWndMutex);
    std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
    if (iterWndMap != mWndMap.end()) {
        iterWndMap->second->show();
        return true;
    }
    return false;
}

bool WndManager::DestoryWnd(QString wndId) {
   std::unique_lock<std::mutex> lock(mWndMutex);
   std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.find(wndId);
   if (iterWndMap != mWndMap.end()) {
      delete iterWndMap->second;
      mWndMap.erase(iterWndMap);
   }
   return false;
}

void WndManager::DestoryAllWnd() {
   std::map<QString, CBaseWnd*>::iterator iterWndMap = mWndMap.begin();
   while (iterWndMap != mWndMap.end()) {
      delete iterWndMap->second;
      iterWndMap = mWndMap.erase(iterWndMap);
   }
}
