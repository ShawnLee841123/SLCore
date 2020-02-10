
#ifndef __WINDOWS_COMPLETION_PORT_WORKER_H__
#define __WINDOWS_COMPLETION_PORT_WORKER_H__

#include "../../../../PublicLib/Include/Common/LibThreadBase.h"
#include "../../CommonDefine/NetWorkCoreEnumDefine.h"
#include "../../CommonDefine/SocketOperateDefine.h"

#include <map>

/*
	写在前面：
		之前写完了一版完成端口的例子，启动1个子线程的时候没有一点问题，但是两个以上线程的时候就发现了问题：虽然是两个线程，但是所有的投递都跑到了监听链接的线程里了
		看了网上的一些说法和对大神的请教之后，发现如下问题。可以使用多个完成端口完成目的功能，或者使用同一个完成端口，在多个线程中同时使用。这个互不妨碍，但是需要处理好连接的存储等等这些事情。
	这部分需要详细的琢磨一下了。目前的把握还没有那么大，如果不是使用无锁队列的话，直接加锁就可以应付，但是为了完成预期的目标。这里必须按照设计目的来进行。当然了，无锁多线程的底限是必须保留的，
	至于其他的东西，必须坚持这一原则，如果有其他的东西，则必须遵守这一异步的原则来完成设计需求。否则，这个项目，或者说框架的意义就没有了。
		
		加油。
*/

struct WinICOPParams
{
	
	void* pICOPHandle;
	void* pFuncExHandle;
	void* pGetFuncExSockAddrHandle;
	LPOPERATE_SOCKET_CONTEXT pFuncSockCon;
	bool bListenSock;
	bool bConnectSock;

	WinICOPParams() : pICOPHandle(nullptr), pFuncExHandle(nullptr), pGetFuncExSockAddrHandle(nullptr), pFuncSockCon(nullptr), bListenSock(false), bConnectSock(false)
	{}

	~WinICOPParams()
	{
		pICOPHandle = nullptr;
		pFuncExHandle = nullptr;
		pGetFuncExSockAddrHandle = nullptr;
		pFuncSockCon = nullptr;
	}

	bool Enable() { return (bListenSock | bConnectSock); }
};

class WinCompletionPortWorker : public ThreadBase
{
public:
	WinCompletionPortWorker();
	virtual ~WinCompletionPortWorker();

#pragma region Parent interface
	virtual bool OnThreadInitialize(int nTickTime) override;
	virtual bool OnThreadRunning() override;
	virtual bool OnThreadDestroy() override;
#pragma endregion

#pragma region public func set and check
	virtual bool SetPortHandle(void* pHandle);
	virtual bool SetWorkerParam(WinICOPParams* pParam);
	virtual bool WorkFunctionEnable(PortCompletionThreadFunctionMask eMask, bool bEnable);
#pragma endregion

protected:

	virtual bool CheckFunctionEnable(PortCompletionThreadFunctionMask eMask);

#pragma region ICOP needed function
	virtual bool InitializeListenFunc(OPERATE_SOCKET_CONTEXT* pListenCon, void* pICOPHandle);
	virtual bool InitializeConnectFunc(OPERATE_SOCKET_CONTEXT* pConnectCon, void* pICOPHandle);

	virtual bool DoAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
	virtual bool PostAccept(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
	virtual bool DoRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
	virtual bool PostRecv(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
	virtual bool DoSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
	virtual bool PostSend(OPERATE_SOCKET_CONTEXT* pSockContext, OPERATE_IO_CONTEXT* pIoContext);
#pragma endregion

#pragma region Variable
	UI32								m_uThreadFunc;					//	线程功能标记

#ifdef _WIN_
	//	完成端口和具体的函数指针
	void*								m_pCompletionPortHandle;		//	完成端口指针
	void*								m_pFnAcceptEx;					//	连接事件函数指针
	void*								m_pFnGetAcceptExSockAddrs;		//	连接地址获取函数指针
	void*								m_pFnConnectEx;					//	连接请求函数指针
	void*								m_pFnGetConnectExSockAddres;	//	连接请求地址获取函数指针
	LPOPERATE_SOCKET_CONTEXT			m_pListenCon;
	LPOPERATE_SOCKET_CONTEXT			m_pConnectCon;
#endif

	std::map<CORE_SOCKET, LPOPERATE_SOCKET_CONTEXT>			dicSocket;

#pragma endregion
};

#endif
