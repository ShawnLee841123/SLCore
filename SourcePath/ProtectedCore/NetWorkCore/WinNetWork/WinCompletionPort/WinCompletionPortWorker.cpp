
#include "WinCompletionPortWorker.h"

WinCompletionPortWorker::WinCompletionPortWorker(): m_pFnGetAcceptExSockAddrs(nullptr), m_pFnAcceptEx(nullptr), m_pCompletionPortHandle(nullptr), m_uThreadFunc(ECPOT_NONE),
m_pConnectCon(nullptr), m_pFnConnectEx(nullptr), m_pFnGetConnectExSockAddres(nullptr), m_pListenCon(nullptr)
{
	dicSocket.clear();

}
WinCompletionPortWorker::~WinCompletionPortWorker()
{
	m_pFnAcceptEx = nullptr;
	m_pFnGetAcceptExSockAddrs = nullptr;
	m_pCompletionPortHandle = nullptr;
	m_pFnConnectEx = nullptr;
	m_pFnGetConnectExSockAddres = nullptr;
	m_uThreadFunc = ECPOT_NONE;
}

#pragma region Parent interface
bool WinCompletionPortWorker::OnThreadInitialize(int nTickTime)
{
	if (nullptr == m_pCompletionPortHandle)
		return false;

	bool bRet = true;
	if (!CheckFunctionEnable(EPCTFT_CONNECT) && !CheckFunctionEnable(EPCTFT_LISTEN))
		bRet = false;

	//	监听功能
	if (CheckFunctionEnable(EPCTFT_LISTEN))
	{ }

	//	连接功能
	if (CheckFunctionEnable(EPCTFT_CONNECT))
	{ }

	return true;
}

bool WinCompletionPortWorker::OnThreadRunning()
{
	return true;
}

bool WinCompletionPortWorker::OnThreadDestroy()
{
	m_pFnAcceptEx = nullptr;
	m_pFnGetAcceptExSockAddrs = nullptr;
	m_pCompletionPortHandle = nullptr;
	m_uThreadFunc = ECPOT_NONE;

	return true;
}
#pragma endregion

#pragma region public func set and check
bool WinCompletionPortWorker::SetPortHandle(void* pHandle)
{
	if (nullptr == pHandle)
		return false;

	if (nullptr != m_pCompletionPortHandle)
		return false;

	m_pCompletionPortHandle = pHandle;

	return (nullptr != m_pCompletionPortHandle);
}

bool WinCompletionPortWorker::SetWorkerParam(WinICOPParams* pParam)
{
	if (nullptr == pParam)
		return false;

	if (pParam->bConnectSock)
	{
		m_pConnectCon = pParam->pFuncSockCon;
		m_pFnConnectEx = pParam->pFuncExHandle;
		m_pFnGetConnectExSockAddres = pParam->pGetFuncExSockAddrHandle;
		m_pCompletionPortHandle = pParam->pICOPHandle;

		WorkFunctionEnable(EPCTFT_CONNECT, true);
	}
	else if (pParam->bListenSock)
	{
		m_pListenCon = pParam->pFuncSockCon;
		m_pFnAcceptEx = pParam->pFuncExHandle;
		m_pFnGetAcceptExSockAddrs = pParam->pGetFuncExSockAddrHandle;
		m_pCompletionPortHandle = pParam->pICOPHandle;

		WorkFunctionEnable(EPCTFT_LISTEN, true);
	}
	else
		m_pCompletionPortHandle = nullptr;
	
	return (nullptr != m_pCompletionPortHandle);
}

bool WinCompletionPortWorker::WorkFunctionEnable(PortCompletionThreadFunctionMask eMask, bool bEnable)
{
	UI32 uFlag = bEnable ? 1 : 0;
	m_uThreadFunc |= (uFlag << eMask);

	return true;
}

bool WinCompletionPortWorker::CheckFunctionEnable(PortCompletionThreadFunctionMask eMask)
{
	UI32 uFlag = 1;
	uFlag &= (m_uThreadFunc >> eMask);

	return uFlag > 0;
}
#pragma endregion

#pragma region ICOP needed function
bool WinCompletionPortWorker::InitializeListenFunc(OPERATE_SOCKET_CONTEXT* pListenCon, void* pICOPHandle)
{
	return true;
}
bool WinCompletionPortWorker::InitializeConnectFunc(OPERATE_SOCKET_CONTEXT* pConnectCon, void* pICOPHandle)
{
	return true;
}

bool WinCompletionPortWorker::DoAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::PostAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::DoRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::PostRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::DoSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::PostSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}
#pragma endregion

