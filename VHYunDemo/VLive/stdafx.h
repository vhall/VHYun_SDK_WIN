#pragma once
// Windows Header Files:
#include <WinSock2.h> 
#include <windows.h>
#include <tchar.h>
#include <QtWidgets/QApplication>
#include <string>

#include "VH_ConstDeff.h"



//#ifdef _DEBUG
//#pragma comment(lib, "MessageDispatcherD.lib")
//#pragma comment(lib, "PluginManagerD.lib")
//#else
//#pragma comment(lib, "MessageDispatcherR.lib")
//#pragma comment(lib, "PluginManagerR.lib")
//#endif
#pragma comment (lib, "Version.lib")
#pragma comment (lib, "ws2_32.lib")

extern std::wstring gCurrentVersion;
extern std::string gCurStreamID;
extern std::string gToken;


