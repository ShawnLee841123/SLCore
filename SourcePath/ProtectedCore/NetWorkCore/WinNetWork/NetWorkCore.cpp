﻿
#include "../NetWorkCore.h"
#include "../../../PublicLib/Include/Common/tools.h"
#include "../../../PublicLib/Include/Common/TypeDefines.h"
#include <vector>
#include <string>

SL_NetWorkCore::SL_NetWorkCore()
{}
SL_NetWorkCore::~SL_NetWorkCore()
{}

#pragma region Interface
//	初始化函数
bool SL_NetWorkCore::Initialize(ISystemCore* pSystemCore)
{
	return true;
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

	if (0 >= nPort)
		return false;

	return true;
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
	if (!CheckStringValid(strAddress))
		return false;

	if (0 >= nPort)
		return false;

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


