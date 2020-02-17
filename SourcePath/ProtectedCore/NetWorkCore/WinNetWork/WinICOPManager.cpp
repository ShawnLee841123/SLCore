#include "WinICOPManager.h"
#include "../../../PublicLib/Include/Common/tools.h"
#include <iostream>
#include <sstream>
#include <MSWSock.h>
#pragma warning(disable:4996)

#pragma region ICOP store Element


ICOPElement::ICOPElement() : strAddress(""), pSockCon(nullptr), pFuncExHandle(nullptr), pICOPHandle(nullptr), pGetAddrFuncExHandle(nullptr)
{
	dicWorker.clear();
}
ICOPElement::~ICOPElement() 
{
	if (dicWorker.size() > 0)
	{
		std::map<SI32, WinCompletionPortWorker*>::iterator iter = dicWorker.begin();
		for (; iter != dicWorker.end(); ++iter)
		{
			WinCompletionPortWorker* pWorker = iter->second;
			if (nullptr != pWorker)
				pWorker->OnThreadDestroy();

			delete pWorker;
			pWorker = nullptr;
		}
	}

	dicWorker.clear();

	if (nullptr != pSockCon)
		delete pSockCon;

	pSockCon = nullptr;
	pFuncExHandle = nullptr;
	pGetAddrFuncExHandle = nullptr;

	if (nullptr != pICOPHandle)
	{
		//	完成端口销毁流程
		CloseHandle(pICOPHandle);
	}
	pICOPHandle = nullptr;

}

bool ICOPElement::AddWinWorker(SI32 nThreadID, WinCompletionPortWorker* pWorker)
{
	if (nThreadID <= 0)
		return false;

	if (nullptr == pWorker)
		return false;

	std::map<SI32, WinCompletionPortWorker*>::iterator iter = dicWorker.find(nThreadID);
	if (iter != dicWorker.end())
		return false;

	dicWorker.insert(std::pair<SI32, WinCompletionPortWorker*>(nThreadID, pWorker));
	return true;
}

bool ICOPElement::GetThreadParam(WinICOPParams& Param)
{
	Param.pICOPHandle = pICOPHandle;
	Param.pFuncExHandle = pFuncExHandle;
	Param.pGetFuncExSockAddrHandle = pGetAddrFuncExHandle;
	Param.pFuncSockCon = pSockCon;

	return ((nullptr != pICOPHandle) && (nullptr != pFuncExHandle) && (nullptr != pGetAddrFuncExHandle) && (nullptr != pSockCon));
}
#pragma endregion

#pragma region ICOP Manager

WinICOPManager::WinICOPManager(): m_nThreadCount(0)
{
	m_dicICOPEle.clear();
}

WinICOPManager::~WinICOPManager() 
{}

bool WinICOPManager::CreateListenSocket(const char* strAddress, int nPort)
{
	if (!CheckStringValid(strAddress))
		return false;

	if (0 >= nPort)
		return false;

#pragma region ICOP Initialize
	void* pICOPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (nullptr == pICOPHandle)
	{
		printf("Completion port Create failed");
		return false;
	}

	LPWIN_OPERATE_SOCKET_CONTEXT pListenCon = new WIN_OPERATE_SOCKET_CONTEXT();
	pListenCon->link = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	int nErrorCode = 0;
	if (INVALID_SOCKET == pListenCon->link)
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not Create Listen Socket. [Error Code: %d]", nErrorCode);
		delete pListenCon;
		pListenCon = nullptr;
		return false;
	}

	if (nullptr == CreateIoCompletionPort((HANDLE)pListenCon->link, pICOPHandle, (DWORD)pListenCon, 0))
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not create ICOP for listen socket. [Error Code: %d]", nErrorCode);
		SAFE_RELEASE_SOCKET(pListenCon->link);
		delete pListenCon;
		pListenCon = nullptr;
		return false;
	}

	CORE_SOCKETADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(CORE_SOCKETADDR_IN));
	inet_pton(AF_INET, strAddress, &(sockAddr.sin_addr));
	sockAddr.sin_port = htons(nPort);
	sockAddr.sin_family = AF_INET;			//	nesscery

	//	bind socket
	if (SOCKET_ERROR == bind(pListenCon->link, (CORE_SOCKADDR*)&(sockAddr), sizeof(CORE_SOCKADDR)))
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not bind socket onto [address: %s, %d]. [Error Code: %d]", strAddress, nPort, nErrorCode);
		SAFE_RELEASE_SOCKET(pListenCon->link);
		delete pListenCon;
		pListenCon = nullptr;
		return false;
	}

	//	begin listen
	if (SOCKET_ERROR == listen(pListenCon->link, LISTEN_LINK_COUNT))
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not listen socket onto [address: %s, %d], [Error Code: %d]", strAddress, nPort, nErrorCode);
		SAFE_RELEASE_SOCKET(pListenCon->link);
		delete pListenCon;
		pListenCon = nullptr;
		return false;
	}

	ICOPElement* pElement = new ICOPElement();
	pElement->strAddress = strAddress;
	pElement->pICOPHandle = pICOPHandle;
	pElement->pSockCon = pListenCon;

	GUID guidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	if (SOCKET_ERROR == WSAIoctl(pListenCon->link, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx), &pElement->pFuncExHandle, sizeof(pElement->pFuncExHandle), &dwBytes, NULL, NULL))
	{
		int nError = WSAGetLastError();
		printf("Can not get Function[AcceptEx] pointer. Error code[%d]", nError);
		SAFE_RELEASE_SOCKET(pListenCon->link);
		return false;
	}

	GUID guidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	dwBytes = 0;
	if (SOCKET_ERROR == WSAIoctl(pListenCon->link, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidGetAcceptExSockAddrs, sizeof(guidGetAcceptExSockAddrs),
		&pElement->pGetAddrFuncExHandle, sizeof(pElement->pGetAddrFuncExHandle), &dwBytes, NULL, NULL))
	{
		int nError = WSAGetLastError();
		printf("Can not get Function[GetAcceptExSockAddrs] pointer. Error code[%d]", nError);
		SAFE_RELEASE_SOCKET(pListenCon->link);
		return false;
	}

	//	Windows extra function pointer AcceptEx and GetAcceptExSockAddrs push down to thread get
	WinICOPParams oParams;
	if (!pElement->GetThreadParam(oParams))
		return false;
#pragma endregion
	//	创建多个线程
	for (SI32 i = 0; i < LISTEN_THREAD_COUNT; i++)
	{
		WinCompletionPortWorker* pWorker = new WinCompletionPortWorker();
		
		pWorker->SetWorkerParam(&oParams);


		pElement->AddWinWorker(pWorker->GetThreadID(), pWorker);
	}

	std::stringstream strName;
	strName << "[Listen:ICOPHandle]" << strAddress << ":" << nPort;
	AddNewElement(strName.str().c_str(), pElement);

	return true;
}

bool WinICOPManager::CreateConnectSocket(const char* strAddress, int nPort)
{
	if (!CheckStringValid(strAddress))
		return false;

	if (0 >= nPort)
		return false;

#pragma region ICOP Initialize
	void* pICOPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (nullptr == pICOPHandle)
	{
		printf("Completion port Create failed");
		return false;
	}

	//std::stringstream strName;
	//strName << "[Listen:ICOPHandle]" << strAddress << ":" << nPort;

	//m_dicICOPHandle.insert(std::pair<const char*, void*>(strName.str().c_str(), pICOPHandle));

	LPWIN_OPERATE_SOCKET_CONTEXT pConnectCon = new WIN_OPERATE_SOCKET_CONTEXT();
	pConnectCon->link = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	int nErrorCode = 0;
	if (INVALID_SOCKET == pConnectCon->link)
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not Create Listen Socket. [Error Code: %d]", nErrorCode);
		delete pConnectCon;
		pConnectCon = nullptr;
		return false;
	}

	if (nullptr == CreateIoCompletionPort((HANDLE)pConnectCon->link, pICOPHandle, (DWORD)pConnectCon, 0))
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not create ICOP for listen socket. [Error Code: %d]", nErrorCode);
		SAFE_RELEASE_SOCKET(pConnectCon->link);
		delete pConnectCon;
		pConnectCon = nullptr;
		return false;
	}

	CORE_SOCKETADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(CORE_SOCKETADDR_IN));
	inet_pton(AF_INET, strAddress, &(sockAddr.sin_addr));
	sockAddr.sin_port = htons(nPort);
	sockAddr.sin_family = AF_INET;			//	nesscery

	if (INVALID_SOCKET == connect(pConnectCon->link, (CORE_SOCKADDR*)&sockAddr, sizeof(CORE_SOCKETADDR_IN)))
	{
		nErrorCode = WSAGetLastError();
		printf("[Error] Can not connect [Address: %s, %d]. [Error Code: %d]", strAddress, nPort, nErrorCode);
		SAFE_RELEASE_SOCKET(pConnectCon->link);
		delete pConnectCon;
		pConnectCon = nullptr;
		return false;
	}

	ICOPElement* pElement = new ICOPElement();
	pElement->pICOPHandle = pICOPHandle;
	pElement->pSockCon = pConnectCon;
	pElement->strAddress = strAddress;

	GUID guidConnectEx = WSAID_CONNECTEX;
	DWORD dwBytes = 0;
	if (SOCKET_ERROR == WSAIoctl(pConnectCon->link, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidConnectEx, sizeof(guidConnectEx), &pElement->pFuncExHandle, sizeof(pElement->pFuncExHandle), &dwBytes, NULL, NULL))
	{
		int nError = WSAGetLastError();
		printf("Can not get Function[ConnectEx] pointer. Error code[%d]", nError);
		SAFE_RELEASE_SOCKET(pConnectCon->link);
		return false;
	}
#pragma endregion

	//	创建线程
	for (SI32 i = 0; i < CONNECT_THREAD_COUNT; i++)
	{
		WinCompletionPortWorker* pWorkder = new WinCompletionPortWorker();

		pElement->AddWinWorker(pWorkder->GetThreadID(), pWorkder);
	}

	std::stringstream strName;
	strName << "[Connect:ICOPHandle]" << strAddress << ":" << nPort;
	AddNewElement(strName.str().c_str(), pElement);

	return true;
}

bool WinICOPManager::OnDestroy()
{
	if (m_dicICOPEle.size() > 0)
	{
		std::map<const char*, ICOPElement*>::iterator iter = m_dicICOPEle.begin();
		for (; iter != m_dicICOPEle.end(); ++iter)
		{
			ICOPElement* pEle = iter->second;
			if (nullptr != pEle)
				delete pEle;
			
			pEle = nullptr;
		}
	}

	m_dicICOPEle.clear();
	WSACleanup();

	return true;
}

#pragma region Function
bool WinICOPManager::AddNewElement(const char* strName, ICOPElement* pEle)
{
	if (!CheckStringValid(strName))
		return false;

	if (nullptr == pEle)
		return false;

	std::map<const char*, ICOPElement*>::iterator iter = m_dicICOPEle.find(strName);
	if (iter != m_dicICOPEle.end())
	{
		printf("[Error] ICOP Element[%s] already exist", strName);
		return false;
	}

	m_dicICOPEle.insert(std::pair<const char*, ICOPElement*>(strName, pEle));
	return true;
}

ICOPElement* WinICOPManager::GetElement(const char* strName)
{
	if (!CheckStringValid(strName))
		return nullptr;

	if (m_dicICOPEle.size() <= 0)
		return nullptr;

	std::map<const char*, ICOPElement*>::iterator iter = m_dicICOPEle.find(strName);
	if (iter != m_dicICOPEle.end())
		return iter->second;

	return nullptr;
}

#pragma endregion