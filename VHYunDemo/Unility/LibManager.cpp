#include "LibManager.h"
#include <windows.h>

LibManager::LibManager(ToolManager* toolManager, QObject *parent)
    : QObject(parent),
   mToolManager(toolManager)
{
}

LibManager::~LibManager()
{
}

bool LibManager::InitDll() {
   if (!RegisterDll(L"VhallWebRtcLive.dll")) {
       return false;
   }
   return true;
}

bool LibManager::RegisterDll(const wchar_t* dll) {
   HINSTANCE dllInterface = LoadLibrary(dll);
   if (dllInterface == NULL)
   {
       int err = GetLastError();
      return false;
   }

   void *Register = GetProcAddress(dllInterface, "RegisterGlobalManager");
   if (Register != NULL)
   {
      bool ret = ((bool(*)(void*))Register)(mToolManager);
   }
   else {
      FreeLibrary(dllInterface);
      dllInterface = NULL;
      return false;
   }
   return true;
}
