
#ifndef __INTERFACE_NET_WORK_CORE_H__
#define __INTERFACE_NET_WORK_CORE_H__

class ISystemCore;

class INetWorkCore
{
public:
	INetWorkCore() {}
	virtual ~INetWorkCore() {}

#pragma region Interface
#pragma region socket about

#pragma region listen socket
	//	create listen socket
	virtual bool CreateListenSocket(const char* strAddress, const char* strPort) = 0;

	//	create listen socket
	virtual bool CreateListenSocket(const char* strAddress, int nPort) = 0;

	//	create listen socket("xx.xx.xx.xx:xxxx")
	virtual bool CreateListenSocket(const char* strAddress) = 0;
#pragma endregion 

#pragma region connect socket
	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress, const char* strPort) = 0;

	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress, int nPort) = 0;

	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress) = 0;
#pragma endregion
#pragma endregion

#pragma endregion
};

#endif	//	__INTERFACE_NET_WORK_CORE_H__

