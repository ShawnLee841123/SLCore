
/*
	项目核心接口

	功能：
		1、管理日志线程
		2、日志线程和工作线程的一一对应
*/

#include "IModuleCoreInterface.h"
#include "../PublicLib/Include/Common/TypeDefines.h"
#include <memory>

#ifndef __INTERFACE_PROJECT_CORE_H__
#define __INTERFACE_PROJECT_CORE_H__

class ThreadBase;
class UnLockQueueBase;

class ILogCore: public IModuleCoreInterface
{
public:
	ILogCore(){}
	virtual ~ILogCore(){}

#pragma region Thread about
	virtual bool RegisterThread(ThreadBase* pThread, const char* strSource, const char* strLogKey = "") = 0;
	virtual std::shared_ptr<UnLockQueueBase> TakePendingLogQueue(SI32 nRegisterId) = 0;
#pragma endregion

#pragma region Log about
	virtual bool CreateLog(const char* strLogKey) = 0;
	virtual bool OutputLog(const char* strLogKey, SI32 nLogLevel, const char* strLog, ...) = 0;
#pragma endregion
};

#define LOG_DEFAULT "Default"

#define LOG_CORE_ECHO(a, ...) if (nullptr != m_pSystemCore){ if (nullptr != m_pSystemCore->GetLogCore()){m_pSystemCore->GetLogCore()->OutputLog(LOG_DEFAULT, 1, a, ##__VA_ARGS__);}}
#define LOG_CORE_DEBUG(a, ...) if (nullptr != m_pSystemCore){ if (nullptr != m_pSystemCore->GetLogCore()){m_pSystemCore->GetLogCore()->OutputLog(LOG_DEFAULT, 2, a, ##__VA_ARGS__);}}
#define LOG_CORE_MSG(a, ...) if (nullptr != m_pSystemCore){ if (nullptr != m_pSystemCore->GetLogCore()){m_pSystemCore->GetLogCore()->OutputLog(LOG_DEFAULT, 3, a, ##__VA_ARGS__);}}
#define LOG_CORE_WARNNING(a, ...) if (nullptr != m_pSystemCore){ if (nullptr != m_pSystemCore->GetLogCore()){m_pSystemCore->GetLogCore()->OutputLog(LOG_DEFAULT, 4, a, ##__VA_ARGS__);}}
#define LOG_CORE_ERROR(a, ...) if (nullptr != m_pSystemCore){ if (nullptr != m_pSystemCore->GetLogCore()){m_pSystemCore->GetLogCore()->OutputLog(LOG_DEFAULT, 5, a, ##__VA_ARGS__);}}

//#define LOG_CORE_ECHO(a, ...) ((ILogCore*)(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 1, a, ##__VA_ARGS__)
//#define LOG_CORE_DEBUG(a, ...) ((ILogCore*)(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 2, a, ##__VA_ARGS__)
//#define LOG_CORE_MSG(a, ...) ((ILogCore*)(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 3, a, ##__VA_ARGS__)
//#define LOG_CORE_WARNNING(a, ...) ((ILogCore*)(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 4, a, ##__VA_ARGS__)
//#define LOG_CORE_ERROR(a, ...) ((ILogCore*)(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 5, a, ##__VA_ARGS__)

//#define LOG_CORE_ECHO(a, ...) (dynamic_cast<ILogCore*>(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 1, a, ##__VA_ARGS__)
//#define LOG_CORE_DEBUG(a, ...) (dynamic_cast<ILogCore*>(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 2, a, ##__VA_ARGS__)
//#define LOG_CORE_MSG(a, ...) (dynamic_cast<ILogCore*>(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 3, a, ##__VA_ARGS__)
//#define LOG_CORE_WARNNING(a, ...) (dynamic_cast<ILogCore*>(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 4, a, ##__VA_ARGS__)
//#define LOG_CORE_ERROR(a, ...) (dynamic_cast<ILogCore*>(m_pSystemCore->GetLogCore()))->OutputLog(LOG_DEFAULT, 5, a, ##__VA_ARGS__)

#endif	//	__INTERFACE_PROJECT_CORE_H__
