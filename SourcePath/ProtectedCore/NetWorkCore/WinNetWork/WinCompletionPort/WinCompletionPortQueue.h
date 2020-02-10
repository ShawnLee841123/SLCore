
#ifndef __WINDOWS_COMPLETION_PORT_QUEUE_H__
#define __WINDOWS_COMPLETION_PORT_QUEUE_H__

#include "../../CommonDefine/SocketOperateDefine.h"
#include "../../../../PublicLib/Include/Common/UnLockQueue.h"
#include "../../../../PublicLib/Include/Common/TypeDefines.h"

#define MSG_BUFFER_COUNT	2048

class SocketMessageData : public UnLockQueueElementDataBase
{
public:
	SocketMessageData();
	virtual ~SocketMessageData();

	SocketMessageData& operator=(const UnLockQueueElementDataBase rhv) override;
	SocketMessageData& operator=(const SocketMessageData& rhv);

	CORE_SOCKET			pLink;
	char				strMsgData[MSG_BUFFER_COUNT];
	UI32				nBufferSize;
	SI64				llStoreID;
};

class SocketRegisterData : public UnLockQueueElementDataBase
{
public:
	SocketRegisterData();
	virtual ~SocketRegisterData();

	SocketRegisterData& operator=(const UnLockQueueElementDataBase rhv) override;
	SocketRegisterData& operator=(const SocketRegisterData& rhv);

	LPOPERATE_SOCKET_CONTEXT				pSockCon;
	PortCompletionSocketRegisterType		eRegType;
	SI32									nThreadID;
};

#endif
