

#ifndef __SERVER_HOLDER_CORE_H__
#define __SERVER_HOLDER_CORE_H__

#include "../PublicLib/Include/System/SystemMacros.h"

#include <map>

class ISystemCore;
class IModule;

class ServerHolderCore
{
public:
	ServerHolderCore();
	~ServerHolderCore();

	bool Initialize();
	bool Start();
	bool MainLoop();
	bool Destroy();

protected:

#pragma region Share object operate
	SYSTEM_HANDLE LoadDynamicLibaray(const char* strFileName);
	bool CheckDynamicLibraryVersion(SYSTEM_HANDLE pHandle, const char* strModuleFileName, const char* strGetVersionFuncName);
	IModule* GetDynamicLibraryModule(SYSTEM_HANDLE pHandle, const char* strModuleName, const char* strGetModuleFuncName);
	SYSTEM_HANDLE GetCoreModuleHandle(const char* strModuleName);
	bool AddModuleIntoContainer(SYSTEM_HANDLE pHandle, const char* strModuleName);
#pragma endregion

#pragma region support functions
	bool LoadDynamicLibraryList();
	bool OnRelease();
	bool ReleaseAllDynamicLibrary();
#pragma endregion

#pragma region Call Module function
	bool InitializeAllModule();
	bool StartAllModule();
#pragma endregion

	ISystemCore*								m_pSystemCore;
	IModule*									m_pSystemModule;
	SYSTEM_HANDLE								m_pSysModuleHandle;	//	SystemCore`s dll handle have to separate management
	std::map<const char*, SYSTEM_HANDLE>		m_dicDllHandleMap;
};

#endif
