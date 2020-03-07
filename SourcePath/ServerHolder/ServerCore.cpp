
#include "ServerCore.h"
#include "../LibTools/ExecuteIniConfigReader.h"
#include "../PublicLib/Include/Common/tools.h"
#include "../PublicLib/Include/Common/TypeDefines.h"
#include "../PublicLib/Include/System/FileSystem.h"
#include "../CoreInterface/IModuleInterface.h"
#include "../CoreInterface/ISystemCore.h"
#include "../CoreInterface/ISystemHelper.h"
#include "../CoreInterface/INetWorkCore.h"
#include "../CoreInterface/IModuleCoreInterface.h"
#include "ServerCoreModuleInterfaceContainer.h"


#ifdef _WIN_
#include <windows.h>
//	如果不想显示控制台，把下面一行注释打开
//#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif

typedef IModule* (*_Module_GetModule)();
_Module_GetModule Dll_GetModule = nullptr;
typedef int(*_Module_GetVersion)();
_Module_GetVersion Dll_GetVersion = nullptr;

ServerHolderCore::ServerHolderCore(): m_pSystemModule(nullptr), m_pSystemCore(nullptr), m_pSysModuleHandle(nullptr), m_pModuleContainer(nullptr), m_pConsoleHandle(nullptr)
{
	m_dicRunDllHandleMap.clear();
}

ServerHolderCore::~ServerHolderCore()
{
	if (!OnRelease())
		printf("Release Module Error");
}

bool ServerHolderCore::Initialize()
{
	bool bRet = true;

	m_pConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	//TODO:
	//	Read ini file
	bRet &= ExecuteIniConfigReader::Instance()->ReadConfig("ExecuteAppConfig");
	if (!bRet)
		printf("Server Can not Read Ini File[ExecuteAppConfig]");

#ifdef _WIN_
	if (ExecuteIniConfigReader::Instance()->GetConfigBoolValue("ExecuteAppConfig", "Start Op", "PauseOn"))
		MessageBoxA(NULL, "Client Test Pause", "Press button Continue", MB_OK);
#endif

	//	Initial Module Container
	m_pModuleContainer = new ServerCoreModuleInterfaceContainer();

	bRet &= ExecuteIniConfigReader::Instance()->ReadConfig("ModuleList");
	if (!bRet)
		printf("Server Can not Read Ini File[ModuleList]");
	
	//	load system core and dynamic file
	bRet &= LoadDynamicLibraryList();

	//	Initialize reset module
	bRet &= InitializeModule(m_dicBaseDllHandleMap);

	return bRet;
}

bool ServerHolderCore::Start()
{
	bool bRet = true;

	//TODO:
	//	Create Listen Socket
	//if (nullptr == m_pSystemCore)
	//	return false;

	//INetWorkCore* pNetWork = dynamic_cast<INetWorkCore*>(m_pSystemCore->GetModuleCoreInterface("SLCNetWorkCore"));
	//if (nullptr == pNetWork)
	//	return false;

	//bRet &= pNetWork->CreateListenSocket("192.168.1.103", 11111);

	//bRet &= StartModule();
	if (nullptr != m_pSystemModule)
		bRet &= m_pSystemModule->OnStartup();
	
	bRet &= StartModule(m_dicBaseDllHandleMap);

	return bRet;
}

bool ServerHolderCore::MainLoop()
{
	bool bRet = true;

	//TODO:
	//INetWorkCore* pNetWork = m_pSystemCore->ge
	//while (true)
	//{
	//	printf("main loop");
	//}

	return bRet;
}

bool ServerHolderCore::Destroy()
{
	bool bRet = true;

	//TODO:

	return bRet;
}

#pragma region Share object operate
SYSTEM_HANDLE ServerHolderCore::LoadDynamicLibaray(const char* strFileName)
{
	if (!CheckStringValid(strFileName))
	{
		printf("ServerHolderCore::LoadDynamicLibaray: Error Param[strFileName]");
		return nullptr;
	}

	SYSTEM_HANDLE pHandle = nullptr;
	char strErrorCode[512] = { 0 };
	pHandle = LoadDynamicFile(strFileName, strErrorCode);
	if (nullptr == pHandle)
	{
		printf("Can not load Dynamic File[%s], Error Code[%s]", strFileName, strErrorCode);
	}

	return pHandle;
}

bool ServerHolderCore::CheckDynamicLibraryVersion(SYSTEM_HANDLE pHandle, const char* strModuleFileName, const char* strGetVersionFuncName)
{
	if (nullptr == pHandle)
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[pHandle]");
		return false;
	}

	if (!CheckStringValid(strModuleFileName))
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[strModuleFileName]");
		return false;
	}

	if (!CheckStringValid(strGetVersionFuncName))
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[strGetVersionFuncName]");
		return false;
	}

	char strErrorCode[512] = { 0 };

	Dll_GetVersion = (_Module_GetVersion)LoadDynamicFileSymbol(pHandle, strGetVersionFuncName, strErrorCode);
	if (nullptr == Dll_GetVersion)
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Module[%s] Can not get Function[%s] Error Code[%s]", strModuleFileName, strGetVersionFuncName, strErrorCode);
		return false;
	}

	SI32 nModuleVersion = Dll_GetVersion();
	if (CORE_MODULE_VERSION != nModuleVersion)
	{
		printf("Module[%s] Version[%x] is not match, Version NO. Should be[%x]", strModuleFileName, nModuleVersion, CORE_MODULE_VERSION);
		return false;
	}

	return true;
}

IModule* ServerHolderCore::GetDynamicLibraryModule(SYSTEM_HANDLE pHandle, const char* strModuleName, const char* strGetModuleFuncName)
{
	if (nullptr == pHandle)
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[pHandle]");
		return nullptr;
	}

	if (!CheckStringValid(strModuleName))
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[strModuleName]");
		return nullptr;
	}

	if (!CheckStringValid(strGetModuleFuncName))
	{
		printf("ServerHolderCore::CheckDynamicLibraryVersion: Error Param[strGetModuleFuncName]");
		return nullptr;
	}

	char strErrorCode[512] = { 0 };
	Dll_GetModule = (_Module_GetModule)LoadDynamicFileSymbol(pHandle, strGetModuleFuncName, strErrorCode);
	if (nullptr == Dll_GetModule)
	{
		printf("ServerHolderCore::GetDynamicLibraryModule: Module[%s] Can not get Function[%s] Error Code[%s]", strModuleName, strGetModuleFuncName, strErrorCode);
		return nullptr;
	}

	IModule* pModule = Dll_GetModule();
	return pModule;
}

SYSTEM_HANDLE ServerHolderCore::GetCoreModuleHandle(const char* strModuleName)
{
	if (!CheckStringValid(strModuleName))
		return nullptr;

	if (m_dicRunDllHandleMap.size() <= 0)
		return nullptr;

	std::map<std::string, SYSTEM_HANDLE>::iterator iter = m_dicRunDllHandleMap.find(strModuleName);
	if (iter != m_dicRunDllHandleMap.end())
		return iter->second;

	return nullptr;
}

bool ServerHolderCore::AddModuleIntoContainer(SYSTEM_HANDLE pHandle, const char* strGroupName, const char* strModuleName)
{
	if (nullptr == pHandle)
		return false;

	if (!CheckStringValid(strGroupName))
		return false;

	if (!CheckStringValid(strModuleName))
		return false;

	std::map<std::string, SYSTEM_HANDLE> tempDic;
	if (0 == strcmp("BaseLib", strGroupName))
	{
		std::map<std::string, SYSTEM_HANDLE>::iterator iter = m_dicBaseDllHandleMap.find(strModuleName);
		if (iter != m_dicBaseDllHandleMap.end())
		{
			printf("Module[%s] Handle is already exsit", strModuleName);
			return false;
		}

		m_dicBaseDllHandleMap.insert(std::pair<const char*, SYSTEM_HANDLE>(strModuleName, pHandle));
		return true;
	}
	else if (0 == (strcmp("RunLib", strGroupName)))
	{
		std::map<std::string, SYSTEM_HANDLE>::iterator iter = m_dicRunDllHandleMap.find(strModuleName);
		if (iter != m_dicRunDllHandleMap.end())
		{
			printf("Module[%s] Handle is already exsit", strModuleName);
			return false;
		}

		m_dicRunDllHandleMap.insert(std::pair<const char*, SYSTEM_HANDLE>(strModuleName, pHandle));
		return true;
	}

	return false;
}

#pragma endregion

#pragma region support functions
bool ServerHolderCore::LoadDynamicLibraryList()
{
#ifdef _WIN_
	const char* strFilePath = "./../Modules/";
#else
	const char* strFilePath = "./../Modules/lib";
#endif

	const char* strSystemCoreName = "SLSystemCore";
	std::string strLoadFileName = strFilePath;
#pragma region Load System Core First
	strLoadFileName += strSystemCoreName;
	strLoadFileName += DEF_MODULE_FILE_EXTRA_NAME;
	SYSTEM_HANDLE pSystemHandle = LoadDynamicLibaray(strLoadFileName.c_str());
	if (!CheckDynamicLibraryVersion(pSystemHandle, strSystemCoreName, "Module_GetVersion"))
		return false;

	m_pSysModuleHandle = pSystemHandle;
	if (nullptr == m_pSystemCore)
	{
		char strErrorCode[512] = { 0 };
		Dll_GetModule = (_Module_GetModule)LoadDynamicFileSymbol(m_pSysModuleHandle, "Module_GetModule", strErrorCode);
		if (nullptr == Dll_GetModule)
		{
			return false;
		}

		m_pSystemModule = Dll_GetModule();
		if (nullptr == m_pSystemModule)
			return false;

		//	Initial system core
		if (!m_pSystemModule->OnModuleInitialize(m_pSystemCore))
		{
			printf("Module[%s] OnModuleInitialize Failed", strSystemCoreName);
			return false;
		}

		m_pSystemCore = m_pSystemModule->GetSystemCore();
		if (nullptr == m_pSystemCore)
		{
			printf("ServerHolderCore::LoadDynamicLibraryList: System core Error");
			return false;
		}
	}
#pragma endregion

	if (nullptr == m_pSystemCore)
		return false;

	if (nullptr == m_pModuleContainer)
		return false;

	//	need Module Container
	m_pSystemCore->GetSystemHelper()->RegisterModuleInterfaceContainer(m_pModuleContainer);
	bool bLoadRet = true;

#pragma region Load Base Module
	bLoadRet &= LoadLibraryGroup("BaseLib");
	//std::vector<std::string> vLibList;
	//ExecuteIniConfigReader::Instance()->GetConfigItemList("ModuleList", "BaseLib", vLibList);
	//SI32 nLibCount = (SI32)vLibList.size();
	//
	//if (nLibCount > 0)
	//{
	//	std::string strModuleName = "";
	//	for (SI32 i = 0; i < nLibCount; i++)
	//	{
	//		strLoadFileName = strFilePath;
	//		strModuleName = vLibList[i];
	//		strLoadFileName += strModuleName + DEF_MODULE_FILE_EXTRA_NAME;

	//		SYSTEM_HANDLE pHandle = LoadDynamicLibaray(strLoadFileName.c_str());
	//		if (!CheckDynamicLibraryVersion(pHandle, strModuleName.c_str(), "Module_GetVersion"))
	//			return false;

	//		if (!AddModuleIntoContainer(pHandle, "BaseLib", strModuleName.c_str()))
	//			return false;

	//		bLoadRet &= true;
	//	}
	//}
#pragma endregion

#pragma region Load Run Module
	//bLoadRet &= LoadLibraryGroup("RunLib");
	//vLibList.clear();
	//ExecuteIniConfigReader::Instance()->GetConfigItemList("ModuleList", "RunLib", vLibList);
	//nLibCount = (SI32)vLibList.size();
	//if (nLibCount > 0)
	//{
	//	std::string strModuleName = "";
	//	for (SI32 i = 0; i < nLibCount; i++)
	//	{
	//		strLoadFileName = strFilePath;
	//		strModuleName = vLibList[i];
	//		strLoadFileName += strModuleName + DEF_MODULE_FILE_EXTRA_NAME;

	//		SYSTEM_HANDLE pHandle = LoadDynamicLibaray(strLoadFileName.c_str());
	//		if (!CheckDynamicLibraryVersion(pHandle, strModuleName.c_str(), "Module_GetVersion"))
	//			return false;

	//		if (!AddModuleIntoContainer(pHandle, "RunLib", strModuleName.c_str()))
	//			return false;

	//		bLoadRet &= true;
	//	}
	//}
#pragma endregion

	return bLoadRet;
}

bool ServerHolderCore::OnRelease()
{
	bool bRet = true;
	if (nullptr != m_pSystemCore)
	{
		m_pSystemModule->OnDestroy();

		bRet &= ReleaseAllDynamicLibrary();

		delete m_pSystemModule;
		m_pSystemModule = nullptr;

		char strErrorCode[512] = { 0 };
		if (!CloseDynamicFile(m_pSysModuleHandle, strErrorCode))
		{
			printf("Error Release System Core, Reseaon[%s]", strErrorCode);
			return false;
		}

		m_pSysModuleHandle = nullptr;
	}

	return bRet;
}

bool ServerHolderCore::ReleaseAllDynamicLibrary()
{
	std::map<std::string, SYSTEM_HANDLE>::iterator iter = m_dicRunDllHandleMap.begin();
	for (; iter != m_dicRunDllHandleMap.end(); ++iter)
	{
		char strErrorCode[512] = { 0 };
		if (!CloseDynamicFile(iter->second, strErrorCode))
		{
			printf("Error Release Module[%s] for Reseaon[%s]", iter->first.c_str(), strErrorCode);
			return false;
		}
	}

	m_dicRunDllHandleMap.clear();
	return true;
}

bool ServerHolderCore::LoadLibraryGroup(const char* strGroupName)
{
#ifdef _WIN_
	const char* strFilePath = "./../Modules/";
#else
	const char* strFilePath = "./../Modules/lib";
#endif

	std::string strLoadFileName = strFilePath;

	bool bLoadRet = true;
	std::vector<std::string> vLibList;
	bLoadRet &= ExecuteIniConfigReader::Instance()->GetConfigItemList("ModuleList", strGroupName, vLibList);
	SI32 nLibCount = (SI32)vLibList.size();
	if (nLibCount > 0)
	{
		std::string strModuleName = "";
		for (SI32 i = 0; i < nLibCount; i++)
		{
			strLoadFileName = strFilePath;
			strModuleName = vLibList[i];
			strLoadFileName += strModuleName + DEF_MODULE_FILE_EXTRA_NAME;

			SYSTEM_HANDLE pHandle = LoadDynamicLibaray(strLoadFileName.c_str());
			if (!CheckDynamicLibraryVersion(pHandle, strModuleName.c_str(), "Module_GetVersion"))
				return false;

			if (!AddModuleIntoContainer(pHandle, strGroupName, strModuleName.c_str()))
				return false;

			bLoadRet &= true;
		}
	}

	return bLoadRet;
}
#pragma endregion

#pragma region Call Module function
bool ServerHolderCore::InitializeModule(std::map<std::string, SYSTEM_HANDLE>& dicMap)
{
	SI32 nLibCount = (SI32)dicMap.size();
	if (nLibCount <= 0)
		return true;

	std::map<std::string, SYSTEM_HANDLE>::iterator iter = dicMap.begin();
	bool bRet = true;
	for (; iter != dicMap.end(); ++iter)
	{
		SYSTEM_HANDLE pHandle = iter->second;
		if (nullptr == pHandle)
		{
			printf("Error Module[%s] in ServerHolderCore Handle Map", iter->first.c_str());
			bRet = false;
			continue;
		}

		IModule* pModule = GetDynamicLibraryModule(pHandle, iter->first.c_str(), "Module_GetModule");
		if (nullptr == pModule)
		{
			printf("ServerHolderCore::InitializeModule: Error Module[%s] can not Get Module", iter->first.c_str());
			bRet = false;
		}

		bRet &= pModule->OnModuleInitialize(m_pSystemCore);
		if (!bRet)
		{
			printf("Error: Module[%s] Initialize Failed", iter->first.c_str());
		}
	}

	return bRet;
}

bool ServerHolderCore::StartModule(std::map<std::string, SYSTEM_HANDLE>& dicMap)
{
	SI32 nLibCount = (SI32)dicMap.size();
	if (nLibCount <= 0)
		return true;

	std::map<std::string, SYSTEM_HANDLE>::iterator iter = dicMap.begin();
	bool bRet = true;
	for (; iter != dicMap.end(); ++iter)
	{
		SYSTEM_HANDLE pHandle = iter->second;
		if (nullptr == pHandle)
		{
			printf("Error Module[%s] in ServerHolderCore Handle Map", iter->first.c_str());
			bRet = false;
			continue;
		}

		IModule* pModule = GetDynamicLibraryModule(pHandle, iter->first.c_str(), "Module_GetModule");
		if (nullptr == pModule)
		{
			printf("ServerHolderCore::InitializeModule: Error Module[%s] can not Get Module", iter->first.c_str());
			bRet = false;
		}

		bRet &= pModule->OnStartup();
		if (!bRet)
		{
			printf("Error: Module[%s] Startup Failed", iter->first.c_str());
		}
	}

	return bRet;
}
#pragma endregion
