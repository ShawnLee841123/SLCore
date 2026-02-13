
#ifndef __SLC_PROJECT_CORE_H__
#define __SLC_PROJECT_CORE_H__

#include "../../CoreInterface/IModuleCoreInterface.h"
#include "../../CoreInterface/ILogCore.h"
#include "../../PublicLib/Include/Common/UnLockQueue.h"

#include <map>
#include <string>
#include <memory>

class ThreadBase;
class LogThreadBase;

#define SLC_LOG_PATH "../Log/"
#define SLC_TIME_STRING_SIZE 512

//	Thread and Log	Management
class SLC_LogCore: public ILogCore//, public IModuleCoreInterface
{
public:
	SLC_LogCore();
	~SLC_LogCore();

#pragma region Parent interface
#pragma region IModuleCoreInterface
	virtual bool Initialize(IModule* pModule) override;
	virtual bool Startup() override;
	virtual bool Destroy() override;
	virtual bool Release() override;
#pragma endregion

#pragma region Thread about
	//	注册线程(这里不负责创建，但是负责销毁，模块销毁的时候，会输出这里面应该移除或销毁的线程)
	//	参数列表：线程，来源（模块名称），日志关键字
	virtual bool RegisterThread(ThreadBase* pThread, const char* strSource, const char* strLogKey = "") override;
#pragma endregion

#pragma region Log about
	virtual bool CreateLog(const char* strLogKey) override;
	virtual bool OutputLog(const char* strLogKey, SI32 nLogLevel, const char* strLog, ...) override;
#pragma endregion
#pragma endregion

protected:

#pragma region Protected Log function
	virtual LogThreadBase* GetLogThread(const char* strLogKey);
	virtual std::shared_ptr<UnLockQueueBase> TakePendingLogQueue(SI32 nRegisterId) override;
	virtual UnLockQueueBase* GetThreadRegisterQueue(const char* strKey);
	std::shared_ptr<UnLockQueueBase> GetThreadRegisterQueuePtr(const char* strKey);
	virtual bool AddNewLog(const char* strLogKey);
	virtual bool RemoveThread(SI32 nThreadID);
	virtual SI32 CalculateLogThreadID();
	virtual bool CheckLogID(SI32 nThreadID);
#pragma endregion
#pragma region Destroy About
	virtual bool StopAllLog();
	virtual bool ReleaseAllLog();
#pragma endregion

	std::map<std::string, LogThreadBase*>		m_dicLogs;				//	日志线程列表
	std::map<std::string, std::shared_ptr<UnLockQueueBase>>	m_dicRegisterQueue;	//	注册日志队列
	std::map<SI32, std::shared_ptr<UnLockQueueBase>>			m_mapPendingLogQueues;	//	待领取队列，key 为本次注册唯一 id
	SI32														m_nNextRegisterId;		//	下次注册分配的唯一 id

	std::shared_ptr<UnLockQueueBase>			m_pGlobalLog;
	void*										m_pConsoleHandle;		//	控制台窗口
	bool										m_bDefaultLog;			//	默认日志
};



#endif	//	__SLC_PROJECT_CORE_H__
