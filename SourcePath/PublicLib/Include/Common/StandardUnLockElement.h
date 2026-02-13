
#ifndef __UNLOCK_QUEUQ_ELEMENT_DEFINE_H__
#define __UNLOCK_QUEUQ_ELEMENT_DEFINE_H__

#pragma region Log data
#include "UnLockQueue.h"
//	日志打印数据
#define LOG_CHARACTER_MAX	512
class LogQueueElementData : public UnLockQueueElementDataBase
{
public:
	LogQueueElementData();
	virtual ~LogQueueElementData();

	LogQueueElementData& operator=(const LogQueueElementData rhv);

	SI32		nLogLevel;		//	日志等级
	SI32		nThreadID;		//	线程ID
	char		strLog[LOG_CHARACTER_MAX];
};

class ScreenLogQueueElementData : public UnLockQueueElementDataBase
{
public:
	ScreenLogQueueElementData();
	virtual ~ScreenLogQueueElementData();

	ScreenLogQueueElementData& operator=(const ScreenLogQueueElementData rhv);

	SI32		nLogLevel;
	SI32		nThreadID;
	char		strLog[LOG_CHARACTER_MAX];
};

class RegisterLogQueueData : public UnLockQueueElementDataBase
{
public:
	RegisterLogQueueData();
	virtual ~RegisterLogQueueData();

	RegisterLogQueueData& operator=(const RegisterLogQueueData rhv);

	SI32					nThreadID;		//	业务线程 ID，用于日志线程内队列命名（Thread%d）
	SI32					nRegisterId;	//	本次注册唯一 id，用于从 LogCore 领取 pending 队列
	bool					bRegister;
	UnLockQueueBase*		pThreadLogQueue;	//	保留兼容，不再使用；领取队列改用 TakePendingLogQueue(nRegisterId)
};

#pragma endregion

#endif	//	__UNLOCK_QUEUQ_ELEMENT_DEFINE_H__
