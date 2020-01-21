
#ifndef __CLIENT_DEMO_CORE_H__
#define __CLIENT_DEMO_CORE_H__

#include "../PublicLib/Include/System/SystemMacros.h"

//#include <WS2tcpip.h>
//#include <winsock.h>
#include <map>

//#define SAFE_RELEASE_SOCKET(a) {if (INVALID_SOCKET != a){closesocket(a); a = INVALID_SOCKET;}}

class ISystemCore;
class IModule;

class ClientDemoCore
{
public:
	ClientDemoCore();
	~ClientDemoCore();

	bool Initialize();
	bool Start();
	bool MainLoop();
	bool Destroy();

	//bool CreateServerLink(const char* strServerAddr);
	//bool ReciveMsg();
	//bool SendMsg();

protected:

#pragma region Dynamic Liberary operate
	bool LoadDynamicLibraryList();
	void* LoadDynamicLibrary(const char* strFileName);
	bool LoadCheckFileVersion(void* pModuleHandle, const char* strModuleFileName, const char* strGetVersionFuncName);
	IModule* LoadModuleFromDynamicLibrary(const char* strModuleName, const char* strGetModuleFuncName);
	bool AddModuleInContainer(void* pModuleHandle, const char* strModuleName);
	SYSTEM_HANDLE GetCoreModuleHandle(const char* strModuleName);

	bool ReleaseAllDynamicLibray();
	bool OnRelease();

#pragma region Call Modules Function
	bool InitializeAllModule();
	bool StartAllModule();
#pragma endregion

#pragma endregion

	bool			m_bInitial;

	//SOCKET			m_Sock;

	ISystemCore*	m_pSystemCore;
	IModule*		m_pSystemModule;
	SYSTEM_HANDLE	m_pSysModuleHandle;	//	SystemCore`s dll handle have to separate management
	std::map<const char*, SYSTEM_HANDLE>		m_dicDllHandleMap;
};

#endif	//	__CLIENT_DEMO_CORE_H__
