

#ifndef __SERVER_HOLDER_CORE_H__
#define __SERVER_HOLDER_CORE_H__

#include "../PublicLib/Include/System/SystemMacros.h"

#include <map>

class ISystemCore;
class IModule;
class IModuleInterfaceContainer;

/*
	Server Work Process
	1、Initialize()
		a、Initial SystemCore
		b、Load Base Lib、Initial Base Lib
	2、Start()
		a、Start SystemCore
		b、Initial Base Lib
	3、MainLoop()
		a、Start Base Lib
		b、Load Run Lib
		c、Initial Run Lib
		d、Start Run Lib
		e、Loop Start
*/


class ServerHolderCore
{
public:
	ServerHolderCore();
	~ServerHolderCore();

	bool Initialize();
	bool Start();
	bool MainLoop();
	bool Destroy();
	bool Release();

protected:

#pragma region Share object operate
	SYSTEM_HANDLE LoadDynamicLibaray(const char* strFileName);
	bool CheckDynamicLibraryVersion(SYSTEM_HANDLE pHandle, const char* strModuleFileName, const char* strGetVersionFuncName);
	IModule* GetDynamicLibraryModule(SYSTEM_HANDLE pHandle, const char* strModuleName, const char* strGetModuleFuncName);
	SYSTEM_HANDLE GetCoreModuleHandle(const char* strModuleName);
	bool AddModuleIntoContainer(SYSTEM_HANDLE pHandle, const char* strGroupName, const char* strModuleName);
#pragma endregion

#pragma region support functions
	bool LoadDynamicLibraryList();
	bool OnRelease();
	bool ReleaseAllDynamicLibrary();
	bool LoadLibraryGroup(const char* strGroupName);
	bool ReleaseAllBaseLibrary();
#pragma endregion

#pragma region Call Module function
	bool InitializeModule(std::map<std::string, SYSTEM_HANDLE>& dicMap);
	bool StartModule(std::map<std::string, SYSTEM_HANDLE>& dicMap);
#pragma endregion

#pragma region Main Loop Function
	bool OnMainLoopInitialize();
	bool OnMainLoopStartup();
	bool OnMainLoopTick();
	bool OnMainLoopDestroy();
#pragma endregion

	ISystemCore*								m_pSystemCore;
	IModule*									m_pSystemModule;
	SYSTEM_HANDLE								m_pSysModuleHandle;	//	SystemCore`s dll handle have to separate management
	std::map<std::string, SYSTEM_HANDLE>		m_dicRunDllHandleMap;
	std::map<std::string, SYSTEM_HANDLE>		m_dicBaseDllHandleMap;
	IModuleInterfaceContainer*					m_pModuleContainer;
	void*										m_pConsoleHandle;
	bool										m_bLoopEnable;
};

#endif
