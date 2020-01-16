
#ifndef __CLIENT_DEMO_CORE_H__
#define __CLIENT_DEMO_CORE_H__

#include <WS2tcpip.h>
#include <winsock.h>
#include <map>

#define SAFE_RELEASE_SOCKET(a) {if (INVALID_SOCKET != a){closesocket(a); a = INVALID_SOCKET;}}

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

	bool CreateServerLink(const char* strServerAddr);
	bool ReciveMsg();
	bool SendMsg();

protected:

#pragma region Dynamic Liberary operate
	bool LoadDynamicLibraryList();
	void* LoadDynamicLibrary(const char* strFileName);
	bool LoadCheckFileVersion(void* pModuleHandle, const char* strModuleFileName, const char* strGetVersionFuncName);
	IModule* LoadModuleFromDynamicLibrary(const char* strModuleName, const char* strGetModuleFuncName);
	bool AddModuleInContainer(void* pModuleHandle, const char* strModuleName);
	void* GetModuleHandle(const char* strModuleName);
	
	bool ReleaseDynamicLibray();
	bool OnRelease();
#pragma endregion

	bool			m_bInitial;

	SOCKET			m_Sock;

	ISystemCore*	m_pSystemCore;
	void*			m_pSysModuleHandle;	//	SystemCore`s dll handle have to separate management
	std::map<const char*, void*>		m_dicDllHandleMap;
};

#endif	//	__CLIENT_DEMO_CORE_H__
