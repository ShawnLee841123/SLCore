#include "WinCompletionPortQueue.h"


SocketMessageData::SocketMessageData(): UnLockQueueElementDataBase(), pLink(INVALID_SOCKET), llStoreID(0), nBufferSize(0)
{
	ZeroMemory(strMsgData, sizeof(char) * MSG_BUFFER_COUNT);
}

SocketMessageData::~SocketMessageData()
{
	pLink = INVALID_SOCKET;
	llStoreID = 0;
	nBufferSize = 0;
	ZeroMemory(strMsgData, sizeof(char) * MSG_BUFFER_COUNT);
}

SocketMessageData& SocketMessageData::operator=(const UnLockQueueElementDataBase rhv)
{
	DataID = rhv.GetDataID();
	return *this;
}

SocketMessageData& SocketMessageData::operator=(const SocketMessageData& rhv)
{
	DataID = rhv.DataID;
	pLink = rhv.pLink;
	llStoreID = rhv.llStoreID;
	nBufferSize = rhv.nBufferSize;
	memcpy(strMsgData, rhv.strMsgData, sizeof(char) * MSG_BUFFER_COUNT);

	return *this;
}

SocketRegisterData::SocketRegisterData():UnLockQueueElementDataBase(), pSockCon(nullptr), nThreadID(0), eRegType(EPCSRT_NONE)
{}

SocketRegisterData::~SocketRegisterData()
{
	pSockCon = nullptr;
	nThreadID = 0;
	eRegType = EPCSRT_NONE;
}

SocketRegisterData& SocketRegisterData::operator=(const UnLockQueueElementDataBase rhv)
{
	DataID = rhv.GetDataID();
	return *this;
}

SocketRegisterData& SocketRegisterData::operator=(const SocketRegisterData& rhv)
{
	DataID = rhv.DataID;
	pSockCon = rhv.pSockCon;
	nThreadID = rhv.nThreadID;
	eRegType = rhv.eRegType;

	return *this;
}
