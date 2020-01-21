

#include "ClientDemoCore.h"
#include "../LibTools/ExecuteIniConfigReader.h"
#include "../PublicLib/Include/Common/tools.h"
#include "../PublicLib/Include/Common/TypeDefines.h"
#include "../PublicLib/Include/System/FileSystem.h"
//#include "../PublicLib/Include/System/SystemMacros.h"
#include "../CoreInterface/IModuleInterface.h"
#include "../CoreInterface/ISystemCore.h"
#include "../CoreInterface/ISystemHelper.h"

#ifdef _WIN_
#include <windows.h>
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif

typedef IModule* (*_Module_GetModule)();
_Module_GetModule Dll_GetModule = nullptr;
typedef int(*_Module_GetVersion)();
_Module_GetVersion Dll_GetVersion = nullptr;

ClientDemoCore::ClientDemoCore(): m_bInitial(false), m_pSystemCore(nullptr), m_pSystemModule(nullptr), m_pSysModuleHandle(nullptr)/*, m_Sock(0)*/ 
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
	if (!ExecuteIniConfigReader::Instance()->ReadConfig("ExecuteAppConfig"))
		return m_bInitial;

	if (ExecuteIniConfigReader::Instance()->GetConfigBoolValue("ExecuteAppConfig", "Start Op", "PauseOn"))
		MessageBox(NULL, "Client Test Pause", "Press button Continue", MB_OK);

	if (!ExecuteIniConfigReader::Instance()->ReadConfig("ModuleList"))
		return m_bInitial;

	m_bInitial = LoadDynamicLibraryList();

	m_bInitial = InitializeAllModule();

	if (!m_bInitial)
		return m_bInitial;

	//	module initialize

	return m_bInitial;
}

bool ClientDemoCore::Start()
{
	return StartAllModule();
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

bool ClientDemoCore::Destroy()
{
	return OnRelease();
	//return true;
}

//bool ClientDemoCore::CreateServerLink(const char* strServerAddr)
//{
//	//	初始化Sockect
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//	if (!CheckStringValid(strServerAddr))
//	{
//		printf("Error Server Addr");
//		return false;
//	}
//
//	std::vector<std::string> vServerAddr = SplitString(strServerAddr, ":");
//	if (vServerAddr.size() != 2)
//	{
//		printf("Invalid Server Address[%s]", strServerAddr);
//		return false;
//	}
//
//	SOCKADDR_IN serverAddrIn;
//	ZeroMemory(&serverAddrIn, sizeof(SOCKADDR_IN));
//	//	字符串转Socket地址(地址不能带有端口号，不然报错)
//	inet_pton(AF_INET, vServerAddr[0].c_str(), &(serverAddrIn.sin_addr));
//	int nValue = atoi(vServerAddr[1].c_str());
//	unsigned short uPort = nValue;
//	serverAddrIn.sin_port = htons(nValue);
//	//	family一定要设置，不然就10047了
//	serverAddrIn.sin_family = AF_INET;
//
//	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (INVALID_SOCKET == m_Sock)
//	{
//		printf("Create Socket Error");
//		return false;
//	}
//
//	connect(m_Sock, (SOCKADDR*)&serverAddrIn, sizeof(SOCKADDR_IN));
//
//	return INVALID_SOCKET != m_Sock;
//}
//
//bool ClientDemoCore::ReciveMsg()
//{
//	char recvBuff[1024] = { 0 };
//	if (INVALID_SOCKET != m_Sock)
//	{
//		recv(m_Sock, recvBuff, 1024, 0);
//	}
//
//	if (CheckStringValid(recvBuff))
//	{
//		printf("Recive Server Msg[%s]", recvBuff);
//	}
//
//	return false;
//}
//
//bool ClientDemoCore::SendMsg()
//{
//	char sendBuff[1024] = { 0 };
//	scanf("%s", &sendBuff);
//	send(m_Sock, sendBuff, strlen(sendBuff) + 1, 0);
//	return false;
//}

#pragma region Dynamic Liberary operate
void* ClientDemoCore::LoadDynamicLibrary(const char* strFileName)
{
	if (!CheckStringValid(strFileName))
	{
		printf("ClientDemoCore::LoadDynamicLibrary: Error Param[strFileName]");
		return nullptr;
	}

	void* pHandle = nullptr;
	//	Load Dll File
	pHandle = LoadDynamicFile(strFileName);
	if (nullptr == pHandle)
	{
		char strErrorCode[512] = { 0 };
		GetDllLastError(strErrorCode);
		printf("Load Dll File[%s] Failed and ErrorCode[%s]", strFileName, strErrorCode);
		//int nError = GetLastError();
		//printf("Load Dll File[%s] Failed and ErrorCode[%d]", strFileName, nError);
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
	Dll_GetVersion = (_Module_GetVersion)LoadDynamicFileSymbol(pModuleHandle, strGetVersionFuncName);
	if (nullptr == Dll_GetVersion)
	{
		char strErrorCode[512] = { 0 };
		GetDllLastError(strErrorCode);
		printf("Load Dll[%s] Function Module_GetVersion Failed, and ErrorCode[%s]", strModuleFileName, strErrorCode);
		//int nError = GetLastError();
		//printf("Load Dll[%s] Function Module_GetVersion Failed, and ErrorCode[%d]", strModuleFileName, nError);
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

	m_dicDllHandleMap.insert(std::pair<const char*, SYSTEM_HANDLE>(strModuleName, pModuleHandle));
	return true;
}

SYSTEM_HANDLE ClientDemoCore::GetCoreModuleHandle(const char* strModuleName)
{
	if (!CheckStringValid(strModuleName))
	{
		printf("ClientDemoCore::GetCoreModuleHandle: Param Check Error[Module Name]");
		return nullptr;
	}

	std::map<const char *, SYSTEM_HANDLE>::iterator iter = m_dicDllHandleMap.find(strModuleName);
	if (m_dicDllHandleMap.end() == iter)
	{
		printf("ClientDemoCore::GetCoreModuleHandle: Can not get Module Handle[%s]", strModuleName);
		return nullptr;
	}

	return iter->second;
}

bool ClientDemoCore::LoadDynamicLibraryList()
{
	const char* strDllName = "../Modules/SLSystemCore.dll";
	const char* strModuleName = "SLSystemCore";
	//	Load Dll File
	void* pHandle = LoadDynamicLibrary(strDllName);
	
	if (!LoadCheckFileVersion(pHandle, strModuleName, "Module_GetVersion"))
		return false;

	if (0 == strcmp("SLSystemCore", strModuleName))
		m_pSysModuleHandle = pHandle;

	bool bSystem = false;

	if (nullptr == m_pSystemCore)
	{
		m_pSystemModule = LoadModuleFromDynamicLibrary(strModuleName, "Module_GetModule");
		if (!m_pSystemModule->OnModuleInitialize(m_pSystemCore))
		{
			printf("ClientDemoCore::LoadDynamicLibraryList: Load SystemCore Module Error");
			return false;
		}

		m_pSystemCore = m_pSystemModule->GetSystemCore();
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
	std::vector<std::string> vLibList;
	ExecuteIniConfigReader::Instance()->GetConfigItemList("ModuleList", "RunLib", vLibList);
	SI32 nLibCount = (SI32)vLibList.size();
	bool bLoadRet = true;
	if (nLibCount > 0)
	{
		std::string strFileName = "";
		std::string strLibModuleName = "";
		for (SI32 i = 0; i < nLibCount; i++)
		{
			strFileName = "../Modules/";
			strLibModuleName = vLibList[i];
			strFileName += strLibModuleName + ".dll";

			pHandle = LoadDynamicLibrary(strFileName.c_str());

			if (!LoadCheckFileVersion(pHandle, strLibModuleName.c_str(), "Module_GetVersion"))
				return false;

			if (!AddModuleInContainer(pHandle, strLibModuleName.c_str()))
				return false;

			bLoadRet &= true;
		}

	}
	return bLoadRet;
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
		pModuleHandle = GetCoreModuleHandle(strModuleName);

	if (nullptr == pModuleHandle)
	{
		printf("ClientDemoCore::LoadModuleFromDynamicLibrary: Module[%s]`s Handle Error", strModuleName);
		return nullptr;
	}

	Dll_GetModule = (_Module_GetModule)LoadDynamicFileSymbol(pModuleHandle, strGetModuleFuncName);
	if (nullptr == Dll_GetModule)
	{
		char strErrorCode[512] = { 0 };
		GetDllLastError(strErrorCode);
		printf("Load Dll[%s] Function[%s] Failed, and ErrorCode[%s]", strModuleName, strGetModuleFuncName, strErrorCode);
		//int nError = GetLastError();
		//printf("Load Dll[%s] Function[%s] Failed, and ErrorCode[%d]", strModuleName, strGetModuleFuncName, nError);
		return false;
	}

	IModule* pModule = Dll_GetModule();
	return pModule;
}

bool ClientDemoCore::ReleaseAllDynamicLibray()
{
	std::map<const char*, SYSTEM_HANDLE>::iterator iter = m_dicDllHandleMap.begin();

	for (; iter != m_dicDllHandleMap.end(); ++iter)
	{
		if (!CloseDynamicFile(iter->second))
		{
			//char strErrorCode[512] = { 0 };
			//GetDllLastError(strErrorCode);
			printf("Release Dll[%s] Error", iter->first);
			//int nError = GetLastError();
			//printf("Release Dll[%s] Error", iter->first);
			return false;
		}
	}

	m_dicDllHandleMap.clear();

	return true;
}

bool ClientDemoCore::OnRelease()
{
	if (nullptr != m_pSystemCore)
	{
		//delete m_pSystemCore;
		//m_pSystemCore = nullptr;

		m_pSystemModule->OnDestroy();

		ReleaseAllDynamicLibray();

		delete m_pSystemModule;
		m_pSystemModule = nullptr;

		if (!CloseDynamicFile(m_pSysModuleHandle))
		{
			//int nError = GetLastError();
			printf("Release Main Dll Error");
			return false;
		}

		m_pSysModuleHandle = nullptr;
	}
	return true;
}

#pragma region Call Modules Function
bool ClientDemoCore::InitializeAllModule()
{
	SI32 nModuleCount = (SI32)m_dicDllHandleMap.size();
	if (nModuleCount <= 0)
		return false;

	bool bIniRet = true;
	std::map<const char*, SYSTEM_HANDLE>::iterator iter = m_dicDllHandleMap.begin();
	const char* strGetModuleFunc = "Module_GetModule";
	for (; iter != m_dicDllHandleMap.end(); ++iter)
	{
		void* pHandle = iter->second;
		if (nullptr == pHandle)
		{
			printf("Error: Module[%s] handle is invalid", iter->first);
			return false;
		}

		Dll_GetModule = (_Module_GetModule)LoadDynamicFileSymbol(pHandle, strGetModuleFunc);
		if (nullptr == Dll_GetModule)
		{
			char strErrorCode[512] = { 0 };
			GetDllLastError(strErrorCode);
			printf("Load Dll[%s] Function Module_GetModule Failed, and ErrorCode[%s]", iter->first, strErrorCode);
			//int nError = GetLastError();
			//printf("Load Dll[%s] Function Module_GetModule Failed, and ErrorCode[%d]", iter->first, nError);
			return false;
		}

		IModule* pModule = Dll_GetModule();
		if (nullptr == pModule)
		{
			printf("Error Module[%s] Can not get", iter->first);
			return false;
		}

		bIniRet &= pModule->OnModuleInitialize(m_pSystemCore);
		if (!bIniRet)
		{
			printf("Error: Module[%s] Initalize Failed", iter->first);
		}
	}

	return bIniRet;
}

bool ClientDemoCore::StartAllModule()
{
	//	获取System的模块接口，直接调用模块的StartUp。
	if (nullptr != m_pSystemModule)
		return m_pSystemModule->OnStartup();

	return false;
}
#pragma endregion

#pragma endregion