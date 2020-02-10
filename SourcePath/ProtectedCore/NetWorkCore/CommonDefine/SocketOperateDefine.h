
#ifndef __SOCKET_OPERATE_DEFINE_H__
#define __SOCKET_OPERATE_DEFINE_H__
#include "../../../PublicLib/Include/Common/TypeDefines.h"
#include "NetWorkCoreEnumDefine.h"

#define IO_BUFF_SIZE 2048
#define LISTEN_LINK_COUNT 4096
//	监听端线程数量
#define LISTEN_THREAD_COUNT 2
//	连接端线程数量
#define CONNECT_THREAD_COUNT 1
#define SAFE_RELEASE_SOCKET(a) {if (INVALID_SOCKET != a){closesocket(a); a = INVALID_SOCKET;}}

#ifdef _WIN_
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>

#pragma region completion port needed

typedef struct _WIN_PER_IO_CONTEXT
{
	OVERLAPPED		overlap;						//	重叠方式
	WSABUF			buffer;							//	重叠缓冲
	char			databuff[IO_BUFF_SIZE];			//	数据缓冲
	SI32			len;							//	数据缓冲长度
	ECompletionPortOperateType opType;				//	操作方式
	CORE_SOCKET		link;							//	网络连接

	_WIN_PER_IO_CONTEXT();
	virtual ~_WIN_PER_IO_CONTEXT();
	_WIN_PER_IO_CONTEXT& operator=(const _WIN_PER_IO_CONTEXT& rhv);

	void ResetDataBuf();
	void ResetOverlapBuf();

}WIN_OPERATE_IO_CONTEXT, *LPWIN_OPERATE_IO_CONTEXT;

typedef struct _WIN_PER_SOCKET_CONTEXT
{
	CORE_SOCKET		link;										//	网络连接
	CORE_SOCKADDR	Addr;										//	连接地址
	SI32			nRevThreadID;								//	接收数据线程ID
	SI32			nSendThreadID;								//	发送数据线程ID
	std::vector<LPWIN_OPERATE_IO_CONTEXT>	vIoOperate;			//	IO操作队列
	SI64			nStoreID;									//	存储线程ID

	_WIN_PER_SOCKET_CONTEXT();
	virtual ~_WIN_PER_SOCKET_CONTEXT();
	_WIN_PER_SOCKET_CONTEXT& operator=(const _WIN_PER_SOCKET_CONTEXT& rhv);
	void ClearOperate();
	LPWIN_OPERATE_IO_CONTEXT GetNewIoOperate();
	void RemoveIoOperate(LPWIN_OPERATE_IO_CONTEXT pIoContext);

}WIN_OPERATE_SOCKET_CONTEXT, *LPWIN_OPERATE_SOCKET_CONTEXT;

#pragma endregion

#else
#endif

#ifdef _WIN_
#define OPERATE_IO_CONTEXT _WIN_PER_IO_CONTEXT
#define OPERATE_SOCKET_CONTEXT _WIN_PER_SOCKET_CONTEXT
#define LPOPERATE_IO_CONTEXT LPWIN_OPERATE_IO_CONTEXT
#define LPOPERATE_SOCKET_CONTEXT LPWIN_OPERATE_SOCKET_CONTEXT
#endif

#endif
