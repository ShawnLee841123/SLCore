
#ifndef __NET_WORK_CORE_H__
#define __NET_WORK_CORE_H__

#include "../../CoreInterface/INetWorkCore.h"
#include "../../CoreInterface/IModuleCoreInterface.h"

#include <map>

class IModule;
#ifdef _WIN_
class WinICOPManager;
#else
#endif

class SL_NetWorkCore: public INetWorkCore, public IModuleCoreInterface
{
public:
	SL_NetWorkCore();
	virtual ~SL_NetWorkCore();

#pragma region Interface
	//	初始化函数
	virtual bool Initialize(IModule* pModule) override;
	virtual bool Startup() override;
	virtual bool Destroy() override;
	virtual bool Release() override;
#pragma region socket about

#pragma region listen socket
	//	create listen socket
	virtual bool CreateListenSocket(const char* strAddress, const char* strPort) override;

	//	create listen socket
	virtual bool CreateListenSocket(const char* strAddress, int nPort) override;

	//	create listen socket("xx.xx.xx.xx:xxxx")
	virtual bool CreateListenSocket(const char* strAddress) override;
#pragma endregion 

#pragma region connect socket
	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress, const char* strPort) override;

	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress, int nPort) override;

	//	create connect socket
	virtual bool CreateConnectSocket(const char* strAddress) override;
#pragma endregion
#pragma endregion

#pragma endregion

protected:

#pragma region Variable

#pragma region Handle core
#ifdef _WIN_
	WinICOPManager*						m_pICOPMgr;
#else
	std::map<std::string, SI32>			m_dicEpollFD;
#endif

	
#pragma endregion

#pragma endregion
};


#endif	//	__NET_WORK_CORE_H__
