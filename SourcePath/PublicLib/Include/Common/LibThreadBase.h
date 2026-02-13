
#ifndef __LIB_THREAD_BASE_H__
#define __LIB_THREAD_BASE_H__

#include "TypeDefines.h"
#include <thread>
#include <map>
#include <string>
#include <memory>

class UnLockQueueBase;
class UnLockQueueElementBase;
class ILogCore;

//	线程无锁队列总数量
#define THREAD_QUEUE_MAX 64
class ThreadBase
{
public:
	ThreadBase();
	virtual ~ThreadBase();

	//	读取队列：本线程为唯一读者，持有 shared_ptr 拥有队列
	virtual bool RegisterQueue(std::shared_ptr<UnLockQueueBase> pQueue, const char* strQueueName, EStandQueueType eType);
	//	写入队列/日志队列：本线程为写者，持有 weak_ptr，读端销毁后 lock() 失效可避免 use-after-free
	virtual bool RegisterQueue(std::weak_ptr<UnLockQueueBase> pQueue, const char* strQueueName, EStandQueueType eType);

	virtual bool OnThreadInitialize(SI32 nTickTime);
	virtual bool OnThreadStart(SI32 nThreadID);
	virtual bool OnThreadRunning();
	virtual bool OnThreadDestroy();
	virtual bool OnThreadClose();

	EServerThreadStatusType GetThreadStatus();
	virtual SI32 GetThreadID();
	virtual void SetThreadID(SI32 nThreadID, EThreadFunctionMaskType eMask = ETFMT_NORMAL);
	void SetLogCore(ILogCore* pLogCore) { m_pLogCore = pLogCore; }

protected:
	virtual void ThreadTick();
	virtual bool ReadQueueProcess(SI32 nElapse);
	virtual bool OnQueueElement(UnLockQueueElementBase* pElement);
	virtual bool AddQueueElement(UnLockQueueElementBase* pElement, const char* strQueueName);

	//	获取队列ID
	virtual SI32 GetQueueID(const char* strQueueName);
	virtual SI32 GetQueueIndex(SI32 nQueueID);
	virtual SI32 GetQueueIndex(const char* strQueueName);
	virtual bool IsReadQueueType(SI32 nQueueID);
	virtual bool IsWriteQueueType(SI32 nQueueID);
	virtual bool IsLogQueue(SI32 nQueueID);
	virtual bool Output(SI32 nLevel, const char* strLog, ...);

#pragma region variable

	EServerThreadStatusType							m_eCurStatus;
	EThreadFunctionMaskType							m_eThreadMask;			//	线程掩码
	std::thread										m_Thread;				//	线程实例
	SI32											m_nThreadID;			//	线程ID
	SI32											m_nLogQueueID;			//	日志队列ID
	SI32											m_nTickTime;			//	每帧时间
	UI32											m_uLastTimeStamp;		//	上帧时间戳
	std::map<std::string, SI32>						m_dicQueueKey;			//	队列名称和ID匹配表
	SI32											m_nReadQueueCount;		//	已注册读队列数
	SI32											m_nWriteQueueCount;		//	已注册写/日志队列数
	std::shared_ptr<UnLockQueueBase>				m_arrReadQueue[THREAD_QUEUE_MAX];		//	读队列（本线程拥有）
	std::weak_ptr<UnLockQueueBase>					m_arrWriteQueue[THREAD_QUEUE_MAX];	//	写/日志队列（不拥有，lock 后使用）
	ILogCore*										m_pLogCore;							//	日志核心（供日志线程领取待注册队列）

#pragma endregion
};

#define THREAD_ECHO(a, ...) Output(1, a, ##__VA_ARGS__)
#define THREAD_DEBUG(a, ...) Output(2, a, ##__VA_ARGS__)
#define THREAD_MSG(a, ...) Output(3, a, ##__VA_ARGS__)
#define THREAD_WARNNING(a, ...) Output(4, a, ##__VA_ARGS__)
#define THREAD_ERROR(a, ...) Output(5, a, ##__VA_ARGS__)

#endif	//	__LIB_THREAD_BASE_H__

