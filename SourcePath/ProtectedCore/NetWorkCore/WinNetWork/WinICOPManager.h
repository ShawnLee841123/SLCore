
#ifndef __WIN_ICOP_MANAGER_H__
#define __WIN_ICOP_MANAGER_H__

#include "WinCompletionPort/WinCompletionPortWorker.h"

#include <map>
#include <string>

#pragma region ICOP store Element

class ISystemCore;
class WinCompletionPortWorker;
class UnLockQueueBase;

class ICOPElement
{
public:
	ICOPElement(ISystemCore* pSysCore);
	virtual ~ICOPElement();

	virtual bool AddWinWorker(SI32 nThreadID, WinCompletionPortWorker* pWorker);
	virtual bool GetThreadParam(WinICOPParams& Param);
	virtual bool InitializeICOPWorker();
#pragma region Variable
	std::string										strAddress;
	LPWIN_OPERATE_SOCKET_CONTEXT					pSockCon;
	void*											pFuncExHandle;
	void*											pICOPHandle;
	void*											pGetAddrFuncExHandle;
#pragma endregion
protected:
	std::map<SI32, WinCompletionPortWorker*>		dicWorker;
	ISystemCore*									m_pSystemCore;
};
#pragma endregion

#pragma region ICOP Manager

class WinICOPManager
{
public:
	WinICOPManager(ISystemCore* pSysCore);
	virtual ~WinICOPManager();

	virtual bool CreateListenSocket(const char* strAddress, int nPort);

	virtual bool CreateConnectSocket(const char* strAddress, int nPort);

	virtual bool StartWork();

	virtual bool OnDestroy();
protected:

#pragma region Function
	virtual bool AddNewElement(const char* strName, ICOPElement* pEle);
	virtual ICOPElement* GetElement(const char* strName);

#pragma endregion

#pragma region Variable

	std::map<std::string, ICOPElement*>			m_dicICOPEle;
	SI32										m_nThreadCount;
	ISystemCore*								m_pSystemCore;

	std::map<SI32, UnLockQueueBase*>			m_dicReadQueue;
	std::map<SI32, UnLockQueueBase*>			m_dicWriteQueue;
#pragma endregion
};
#pragma endregion

#endif
