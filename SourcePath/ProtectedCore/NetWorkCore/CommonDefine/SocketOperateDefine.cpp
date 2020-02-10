
#include "SocketOperateDefine.h"
#include <string>




#ifdef _WIN_

#pragma region completion port needed

#pragma region io context
_WIN_PER_IO_CONTEXT::_WIN_PER_IO_CONTEXT(): link(INVALID_SOCKET), len(0)
{
	opType = ECPOT_NONE;

	ResetDataBuf();
	ResetOverlapBuf();
}
_WIN_PER_IO_CONTEXT::~_WIN_PER_IO_CONTEXT()
{
	ResetDataBuf();
	ResetOverlapBuf();
	opType = ECPOT_NONE;
	
}

_WIN_PER_IO_CONTEXT& _WIN_PER_IO_CONTEXT::operator=(const _WIN_PER_IO_CONTEXT& rhv)
{
	memcpy(&overlap, &rhv.overlap, sizeof(OVERLAPPED));
	memcpy(&buffer, &rhv.buffer, sizeof(WSABUF));
	memcpy(databuff, rhv.databuff, sizeof(char) * IO_BUFF_SIZE);
	len = rhv.len;
	link = rhv.link;
	opType = rhv.opType;
	return *this;
}

void _WIN_PER_IO_CONTEXT::ResetDataBuf()
{
	ZeroMemory(databuff, sizeof(char) * IO_BUFF_SIZE);
	len = 0;
}

void _WIN_PER_IO_CONTEXT::ResetOverlapBuf()
{
	ZeroMemory(&overlap, sizeof(WSAOVERLAPPED));
}

#pragma endregion



#pragma region socket context

CORE_SOCKET		link;										//	网络连接
CORE_SOCKADDR	Addr;										//	连接地址
SI32			nRevThreadID;								//	接收数据线程ID
SI32			nSendThreadID;								//	发送数据线程ID
std::vector<LPWIN_OPERATE_IO_CONTEXT>	vIoOperate;			//	IO操作队列
SI64			nStoreID;									//	存储线程ID

_WIN_PER_SOCKET_CONTEXT::_WIN_PER_SOCKET_CONTEXT(): link(INVALID_SOCKET), nRevThreadID(0), nSendThreadID(0), nStoreID(0)
{
	vIoOperate.clear();

}

_WIN_PER_SOCKET_CONTEXT::~_WIN_PER_SOCKET_CONTEXT()
{
	SAFE_RELEASE_SOCKET(link);
	nRevThreadID = 0;
	nSendThreadID = 0;
	nStoreID = 0;
	ClearOperate();
}

_WIN_PER_SOCKET_CONTEXT& _WIN_PER_SOCKET_CONTEXT::operator=(const _WIN_PER_SOCKET_CONTEXT& rhv)
{
	link = rhv.link;
	nRevThreadID = rhv.nRevThreadID;
	nSendThreadID = rhv.nSendThreadID;
	nStoreID = rhv.nStoreID;
	memcpy(&Addr, &(rhv.Addr), sizeof(CORE_SOCKADDR));
	SI32 nIoCount = (SI32)rhv.vIoOperate.size();
	for (int i = 0; i < nIoCount; i++)
	{
		LPOPERATE_IO_CONTEXT pIo = rhv.vIoOperate[i];
		if (nullptr != pIo)
		{
			LPOPERATE_IO_CONTEXT pIoNew = GetNewIoOperate();
			*pIoNew = *pIo;
		}
	}

	return *this;
}

void _WIN_PER_SOCKET_CONTEXT::ClearOperate()
{
	SI32 nOpCount = (SI32)vIoOperate.size();
	if (nOpCount > 0)
	{
		for (SI32 i = 0; i < nOpCount; i++)
		{
			LPOPERATE_IO_CONTEXT pIoContext = vIoOperate[i];
			if (nullptr == pIoContext)
				continue;

			delete pIoContext;
			pIoContext = nullptr;
		}

		vIoOperate.clear();
	}
}

LPWIN_OPERATE_IO_CONTEXT _WIN_PER_SOCKET_CONTEXT::GetNewIoOperate()
{
	LPOPERATE_IO_CONTEXT pNewIoOp = new WIN_OPERATE_IO_CONTEXT();
	vIoOperate.push_back(pNewIoOp);

	return pNewIoOp;
}

void _WIN_PER_SOCKET_CONTEXT::RemoveIoOperate(LPWIN_OPERATE_IO_CONTEXT pIoContext)
{
	if (nullptr != pIoContext)
	{
		SI32 nIoCount = (SI32)vIoOperate.size();
		if (nIoCount <= 0)
			return;

		std::vector<LPWIN_OPERATE_IO_CONTEXT>::iterator iter = vIoOperate.begin();
		for (; iter != vIoOperate.end(); ++iter)
		{
			if ((*iter)->link == pIoContext->link)
			{
				delete pIoContext;
				pIoContext = nullptr;
				vIoOperate.erase(iter);
				break;
			}
		}
	}
}

#pragma endregion

#pragma endregion

#else
#endif