
#include "../NetWorkCore.h"
#include "WinICOPManager.h"
#include "../CommonDefine/SocketOperateDefine.h"
#include "../../../CoreInterface/IModuleInterface.h"
#include "../../../PublicLib/Include/Common/tools.h"
#include "../../../PublicLib/Include/Common/TypeDefines.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <MSWSock.h>
#pragma warning(disable:4996)

SL_NetWorkCore::SL_NetWorkCore(): m_pICOPMgr(nullptr)
{}
SL_NetWorkCore::~SL_NetWorkCore()
{}

#pragma region Interface
//	初始化函数
bool SL_NetWorkCore::Initialize(IModule* pModule)
{
#pragma region Module Core Necessary
	if (nullptr == pModule)
		return false;

	m_pModule = pModule;

	ISystemCore* pSystemCore = pModule->GetSystemCore();
	if (nullptr == pSystemCore)
		return false;

	m_pSysCore = pSystemCore;
#pragma endregion

	bool bRet = IsEnable();

	//	Windows路径下的东东，因此这里需要初始化Socket和完成端口
	//	初始化Sockect
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_pICOPMgr = new WinICOPManager();

	return bRet;
}

bool SL_NetWorkCore::Startup()
{
	bool bRet = true;

	return bRet;
}

bool SL_NetWorkCore::Destroy()
{
	return true;
}

#pragma region socket about

#pragma region listen socket
//	create listen socket
bool SL_NetWorkCore::CreateListenSocket(const char* strAddress, const char* strPort)
{
	if (!CheckStringValid(strAddress))
		return false;

	if (!CheckStringValid(strPort))
		return false;

	SI32 nPort = atoi(strPort);
	if (0 >= nPort)
		return false;

	return CreateListenSocket(strAddress, nPort);
}

//	create listen socket
bool SL_NetWorkCore::CreateListenSocket(const char* strAddress, int nPort)
{
	if (!CheckStringValid(strAddress))
		return false;

	if (nullptr == m_pICOPMgr)
		return false;

	return m_pICOPMgr->CreateListenSocket(strAddress, nPort);
}

//	create listen socket("xx.xx.xx.xx:xxxx")
bool SL_NetWorkCore::CreateListenSocket(const char* strAddress)
{
	if (!CheckStringValid(strAddress))
		return false;

	std::vector<std::string> vAddr = SplitString(strAddress, ":");
	if (vAddr.size() != 2)
		return false;

	return CreateListenSocket(vAddr[0].c_str(), vAddr[1].c_str());
}
#pragma endregion 

#pragma region connect socket
//	create connect socket
bool SL_NetWorkCore::CreateConnectSocket(const char* strAddress, const char* strPort)
{
	if (!CheckStringValid(strAddress))
		return false;

	if (!CheckStringValid(strPort))
		return false;

	SI32 nPort = atoi(strPort);
	if (0 >= nPort)
		return false;

	return CreateConnectSocket(strAddress, nPort);
}

//	create connect socket
bool SL_NetWorkCore::CreateConnectSocket(const char* strAddress, int nPort)
{

	return true;
}

//	create connect socket
bool SL_NetWorkCore::CreateConnectSocket(const char* strAddress)
{
	if (!CheckStringValid(strAddress))
		return false;

	std::vector<std::string> vAddr = SplitString(strAddress, ":");
	if (vAddr.size() != 2)
		return false;

	return CreateConnectSocket(vAddr[0].c_str(), vAddr[1].c_str());
}
#pragma endregion
#pragma endregion

#pragma endregion


