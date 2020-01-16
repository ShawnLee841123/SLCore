

#include "ClientDemoCore.h"
#include "../PublicLib/Include/Common/tools.h"
#include "../CoreInterface/IModuleInterface.h"
#include "../CoreInterface/ISystemCore.h"
#include "../CoreInterface/ISystemHelper.h"


typedef IModule* (*_Module_GetModule)();
_Module_GetModule Dll_GetModule = nullptr;
typedef int(*_Module_GetVersion)();
_Module_GetVersion Dll_GetVersion = nullptr;

ClientDemoCore::ClientDemoCore(): m_Sock(0), m_bInitial(false), m_pSystemCore(nullptr)
{
	m_dicDllHandleMap.clear();
}

ClientDemoCore::~ClientDemoCore()
{
	if (!OnRelease())
		printf("Release Module Error");
	//SAFE_RELEASE_SOCKET(m_Sock);
	//WSACleanup();
}

bool ClientDemoCore::Initialize()
{
	//m_bInitial = CreateServerLink("10.53.3.212:11111");
	m_bInitial = LoadDynamicLibraryList();
	return m_bInitial;
}

bool ClientDemoCore::Start()
{
	return true;
}

bool ClientDemoCore::MainLoop()
{
	if (!m_bInitial)
		return false;
	//while (true)
	//{
	//	SendMsg();
	//	ReciveMsg();
	//}
	return true;
}

bool ClientDemoCore::CreateServerLink(const char* strServerAddr)
{
	//	初始化Sockect
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (!CheckStringValid(strServerAddr))
	{
		printf("Error Server Addr");
		return false;
	}

	std::vector<std::string> vServerAddr = SplitString(strServerAddr, ":");
	if (vServerAddr.size() != 2)
	{
		printf("Invalid Server Address[%s]", strServerAddr);
		return false;
	}

	SOCKADDR_IN serverAddrIn;
	ZeroMemory(&serverAddrIn, sizeof(SOCKADDR_IN));
	//	字符串转Socket地址(地址不能带有端口号，不然报错)
	inet_pton(AF_INET, vServerAddr[0].c_str(), &(serverAddrIn.sin_addr));
	int nValue = atoi(vServerAddr[1].c_str());
	unsigned short uPort = nValue;
	serverAddrIn.sin_port = htons(nValue);
	//	family一定要设置，不然就10047了
	serverAddrIn.sin_family = AF_INET;

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Sock)
	{
		printf("Create Socket Error");
		return false;
	}

	connect(m_Sock, (SOCKADDR*)&serverAddrIn, sizeof(SOCKADDR_IN));

	return INVALID_SOCKET != m_Sock;
}

bool ClientDemoCore::ReciveMsg()
{
	char recvBuff[1024] = { 0 };
	if (INVALID_SOCKET != m_Sock)
	{
		recv(m_Sock, recvBuff, 1024, 0);
	}

	if (CheckStringValid(recvBuff))
	{
		printf("Recive Server Msg[%s]", recvBuff);
	}

	return false;
}

bool ClientDemoCore::SendMsg()
{
	char sendBuff[1024] = { 0 };
	scanf("%s", &sendBuff);
	send(m_Sock, sendBuff, strlen(sendBuff) + 1, 0);
	return false;
}

#pragma region Dynamic Liberary operate
void* ClientDemoCore::LoadDynamicLibrary(const char* strFileName)
{
	if (!CheckStringValid(strFileName))
	{
		printf("ClientDemoCore::LoadDynamicLibrary: Error Param[strFileName]");
		return nullptr;
	}

	//const char* strDllName = "SLSystemCore.dll";
	void* pHandle = nullptr;
	//	Load Dll File
	pHandle = LoadLibrary(strFileName);
	if (nullptr == pHandle)
	{
		int nError = GetLastError();
		printf("Load Dll File[%s] Failed and ErrorCode[%d]", strFileName, nError);
		return pHandle;
	}

	return pHandle;
}

bool ClientDemoCore::LoadCheckFileVersion(void* pModuleHandle, const char* strModuleFileName, const char* strGetVersionFuncName)
{
	if (nullptr == pModuleHandle)
	{
		printf("ClientDemoCore::LoadCheckFileVersion: Param Check Error[Module File Handle]");
		return false;
	}

	if (!CheckStringValid(strModuleFileName))
	{
		printf("ClientDemoCore::LoadCheckFileVersion: Param Check Error[Module Name]");
		return false;
	}

	if (!CheckStringValid(strGetVersionFuncName))
	{
		printf("ClientDemoCore::LoadCheckFileVersion: Param Check Error[Get Module Version Function Name]");
		return false;
	}

	//	Get file export get version function
	Dll_GetVersion = (_Module_GetVersion)GetProcAddress((HINSTANCE)pModuleHandle, strGetVersionFuncName);
	if (nullptr == Dll_GetVersion)
	{
		int nError = GetLastError();
		printf("Load Dll[%s] Function Module_GetVersion Failed, and ErrorCode[%d]", strModuleFileName, nError);
		return false;
	}

	//	Check Module Version No.
	int nModuleVersion = Dll_GetVersion();
	if (CORE_MODULE_VERSION != nModuleVersion)
	{
		printf("Dll File[%s]`s Version[%d] Check Can not Pass. Need Version[%d]", strModuleFileName, nModuleVersion, CORE_MODULE_VERSION);
		return false;
	}

	return true;
}

bool ClientDemoCore::AddModuleInContainer(void* pModuleHandle, const char* strModuleName)
{
	if (nullptr == pModuleHandle)
	{
		printf("ClientDemoCore::AddModuleInContainer: Param Check Error[Module File Handle]");
		return false;
	}

	if (!CheckStringValid(strModuleName))
	{
		printf("ClientDemoCore::AddModuleInContainer: Param Check Error[Module Name]");
		return false;
	}
	//	Add Dll Handle in container
	if (m_dicDllHandleMap.find(strModuleName) != m_dicDllHandleMap.end())
	{
		printf("Dll already exist");
		return false;
	}

	m_dicDllHandleMap.insert(std::pair<const char*, void*>(strModuleName, pModuleHandle));
	return true;
}

void* ClientDemoCore::GetModuleHandle(const char* strModuleName)
{
	if (!CheckStringValid(strModuleName))
	{
		printf("ClientDemoCore::GetModuleHandle: Param Check Error[Module Name]");
		return nullptr;
	}

	std::map<const char *, void*>::iterator iter = m_dicDllHandleMap.find(strModuleName);
	if (m_dicDllHandleMap.end() == iter)
	{
		printf("ClientDemoCore::GetModuleHandle: Can not get Module Handle[%s]", strModuleName);
		return nullptr;
	}

	return iter->second;
}

bool ClientDemoCore::LoadDynamicLibraryList()
{
	const char* strDllName = "SLSystemCore.dll";
	const char* strModuleName = "SLSystemCore";
	//	Load Dll File
	void* pHandle = LoadDynamicLibrary(strDllName);
	
	if (!LoadCheckFileVersion(pHandle, strModuleName, "Module_GetVersion"))
		return false;

	//	SystemCore do not add to module handle container
	//if (!AddModuleInContainer(pHandle, strModuleName))
	//	return false;
	if (0 == strcmp("SLSystemCore", strModuleName))
		m_pSysModuleHandle = pHandle;

	bool bSystem = false;

	if (nullptr == m_pSystemCore)
	{
		IModule* pSysModule = LoadModuleFromDynamicLibrary(strModuleName, "Module_GetModule");
		if (!pSysModule->OnModuleInitialize(m_pSystemCore))
		{
			printf("ClientDemoCore::LoadDynamicLibraryList: Load SystemCore Module Error");
			return false;
		}

		m_pSystemCore = pSysModule->GetSystemCore();
		if (nullptr == m_pSystemCore)
		{
			printf("ClientDemoCore::LoadDynamicLibraryList: SystemCore Module Error");
			return false;
		}

		bSystem = true;
	}

	if (!bSystem)
		return false;

	//TODO:	Load Other Module
	return true;
}

IModule* ClientDemoCore::LoadModuleFromDynamicLibrary(const char* strModuleName, const char* strGetModuleFuncName)
{
	if (!CheckStringValid(strModuleName))
	{
		printf("ClientDemoCore::LoadModuleFromDynamicLibrary: Param Check Error[Module Name]");
		return nullptr;
	}

	if (!CheckStringValid(strGetModuleFuncName))
	{
		printf("ClientDemoCore::LoadModuleFromDynamicLibrary: Param Check Error[Get Module Function Name]");
		return nullptr;
	}

	void* pModuleHandle = nullptr;
	if (0 == strcmp(strModuleName, "SLSystemCore"))
		pModuleHandle = m_pSysModuleHandle;
	else
		pModuleHandle = GetModuleHandle(strModuleName);

	if (nullptr == pModuleHandle)
	{
		printf("ClientDemoCore::LoadModuleFromDynamicLibrary: Module[%s]`s Handle Error", strModuleName);
		return nullptr;
	}

	Dll_GetModule = (_Module_GetModule)GetProcAddress((HINSTANCE)pModuleHandle, strGetModuleFuncName);
	if (nullptr == Dll_GetModule)
	{
		int nError = GetLastError();
		printf("Load Dll[%s] Function[%s] Failed, and ErrorCode[%d]", strModuleName, strGetModuleFuncName, nError);
		return false;
	}

	IModule* pModule = Dll_GetModule();
	return pModule;
}

bool ClientDemoCore::ReleaseDynamicLibray()
{
	return true;
}

bool ClientDemoCore::OnRelease()
{
	if (nullptr != m_pSystemCore)
	{
		//delete m_pSystemCore;
		//m_pSystemCore = nullptr;

		if (!FreeLibrary((HINSTANCE)m_pSysModuleHandle))
		{
			int nError = GetLastError();
			printf("Release Main Dll Error");
			return false;
		}

		m_pSysModuleHandle = nullptr;
	}
	return true;
}

#pragma endregion