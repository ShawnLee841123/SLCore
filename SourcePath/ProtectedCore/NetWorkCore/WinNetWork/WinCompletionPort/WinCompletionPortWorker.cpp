
#include "WinCompletionPortWorker.h"
#include "WinCompletionPortQueue.h"
#include "../../../../CoreInterface/ISystemCore.h"
#include "../../../../CoreInterface/ILogCore.h"
#include "../../../../PublicLib/Include/Common/UnLockQueue.h"

WinCompletionPortWorker::WinCompletionPortWorker(ISystemCore* pSysCore): m_pFnGetAcceptExSockAddrs(nullptr), m_pFnAcceptEx(nullptr), m_pCompletionPortHandle(nullptr), m_uThreadFunc(ECPOT_NONE),
m_pConnectCon(nullptr), m_pFnConnectEx(nullptr), m_pFnGetConnectExSockAddres(nullptr), m_pListenCon(nullptr), m_pSystemCore(pSysCore)
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

	bRet &= m_pSystemCore->GetLogCore()->RegisterThread(this, "", "NetWorker");

	//	监听功能
	if (CheckFunctionEnable(EPCTFT_LISTEN))
	{ }

	//	连接功能
	if (CheckFunctionEnable(EPCTFT_CONNECT))
	{ }

	return bRet;
}

bool WinCompletionPortWorker::OnThreadRunning()
{
	bool bRet = true;
	bRet &= ThreadBase::OnThreadRunning();

	return bRet;
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

bool WinCompletionPortWorker::OnQueueElement(UnLockQueueElementBase* pElement)
{
	UnLockQueueDataElementBase* pDataElement = dynamic_cast<UnLockQueueDataElementBase*>(pElement);
	bool bRet = nullptr != pDataElement;
	if (nullptr == pDataElement)
	{
		return bRet;
	}

	UI32 uDataID = pDataElement->GetDataID();
	switch (uDataID)
	{
	case EESDGT_REGISTER:
	{
		SocketRegisterData* pData = (SocketRegisterData*)pDataElement->GetData();
		break;
	}
	default:
		break;
	}

	return bRet;
}

//bool WinCompletionPortWorker::RegisterConnectSocket(OPERATE_SOCKET_CONTEXT* pSockContext)
//{
//	bool bRet = true;
//
//	return bRet;
//}
#pragma endregion

#pragma region ICOP needed function
bool WinCompletionPortWorker::InitializeListenFunc(OPERATE_SOCKET_CONTEXT* pListenCon, void* pICOPHandle)
{
	//	这里需要投递连接同意响应和收到消息的响应
	return true;
}
bool WinCompletionPortWorker::InitializeConnectFunc(OPERATE_SOCKET_CONTEXT* pConnectCon, void* pICOPHandle)
{
	//	连接端只需要投递消息接收响应
	return true;
}

bool WinCompletionPortWorker::DoAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	
	return true;
}

bool WinCompletionPortWorker::PostAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	if (INVALID_SOCKET == pSockContext->link)
	{
		THREAD_ERROR("Listen Socket INVALID!!!");
		return false;
	}

	DWORD nBytes = 0;
	pIoContext->opType = ECPOT_ACCEPT;
	WSABUF* pWBuff = &pIoContext->buffer;
	WSAOVERLAPPED* pOl = &pIoContext->overlap;
	pIoContext->link = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pIoContext->link)
	{
		THREAD_ERROR("Create new socket for listen link faild!!!");
		return false;
	}

	LPFN_ACCEPTEX pFn = (LPFN_ACCEPTEX)m_pFnAcceptEx;
	if (FALSE == pFn(pSockContext->link, pIoContext->link, pWBuff->buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &nBytes, pOl))
	{
		SI32 nErrorCode = WSAGetLastError();
		if (WSA_IO_PENDING != nErrorCode)
		{
			THREAD_ERROR("Post AcceptEx request faild. Error code[%d]", nErrorCode);
			return false;
		}
	}
	return true;
}

bool WinCompletionPortWorker::DoRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::PostRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	if (nullptr == pIoContext)
		pIoContext = pSockContext->GetNewIoOperate();

	DWORD nFlags = 0;
	DWORD nBytes = 0;
	WSABUF* pWBuff = &pIoContext->buffer;
	WSAOVERLAPPED* pOl = &pIoContext->overlap;
	pIoContext->ResetDataBuf();
	pIoContext->ResetOverlapBuf();
	int nByteRecv = WSARecv(pIoContext->link, pWBuff, 1, &nBytes, &nFlags, pOl, nullptr);
	SI32 iErrorCode = WSAGetLastError();
	if ((SOCKET_ERROR == nByteRecv) && (WSA_IO_PENDING != iErrorCode))
	{
		THREAD_ERROR("Post recv request faild, Error code[%d]", iErrorCode);
		return false;
	}
	return true;
}

bool WinCompletionPortWorker::DoSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	return true;
}

bool WinCompletionPortWorker::PostSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext)
{
	if (nullptr == pIoContext)
		pIoContext = pSockContext->GetNewIoOperate();

	DWORD nFlags = 0;
	DWORD nBytes = 0;
	WSABUF* pWBuff = &pIoContext->buffer;
	WSAOVERLAPPED* pOl = &pIoContext->overlap;

	SI32 nByteSend = WSASend(pIoContext->link, pWBuff, 1, &nBytes, nFlags, pOl, nullptr);
	SI32 iErrorCode = WSAGetLastError();

	if ((SOCKET_ERROR == nByteSend) && (WSA_IO_PENDING != iErrorCode))
	{
		THREAD_ERROR("Post send request faild, Error code[%d]", iErrorCode);
		return false;
	}
	return true;
}
#pragma endregion

