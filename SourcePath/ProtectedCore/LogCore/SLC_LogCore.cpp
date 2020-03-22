#include "SLC_LogCore.h"
#include "../../PublicLib/Include/Common/LibThreadBase.h"
#include "../../PublicLib/Include/Common/LogThreadBase.h"
#include "../../PublicLib/Include/Common/UnLockQueue.h"
#include "../../PublicLib/Include/Common/StandardUnLockElement.h"
#include "../../PublicLib/Include/Common/UnLockElementTypeDefine.h"
#include "../../PublicLib/Include/Common/tools.h"
#include "../../PublicLib/Include/Common/TypeDefines.h"
#include "../../PublicLib/Include/System/FileSystem.h"
#include "../../PublicLib/Include/System/TimeSystem.h"
#include <iostream>
#include <sstream>
#ifdef _WIN_
#include <windows.h>
#endif

SLC_LogCore::SLC_LogCore(): m_bDefaultLog(false), m_pConsoleHandle(nullptr), m_pGlobalLog(nullptr)
{
	m_dicLogs.clear();
	m_dicRegisterQueue.clear();
}

SLC_LogCore::~SLC_LogCore()
{}

#pragma region Parent interface
#pragma region IModuleCoreInterface
bool SLC_LogCore::Initialize(IModule* pModule)
{
	if (!CreatePath(SLC_LOG_PATH))
		return false;

	if (!RegisterThread(nullptr, "", ""))
		return false;

	return true;
}

bool SLC_LogCore::Startup()
{
	//	Log core just need Initial can be work
	return true;
}

bool SLC_LogCore::Destroy()
{
	bool bRet = true;
	bRet &= StopAllLog();
	return bRet;
}

#pragma endregion

#pragma region Thread about
bool SLC_LogCore::RegisterThread(ThreadBase* pThread, const char* strSource, const char* strLogKey)
{
	std::string strLogName = "";
	bool bNeedDefault = false;
	if (m_dicLogs.size() <= 0)
		bNeedDefault = true;

	if (!CheckStringValid(strLogKey))
	{
		if (bNeedDefault && !m_bDefaultLog)
			strLogName = "Default";
	}
	else
	{
		strLogName = strLogKey;
	}

	if (!AddNewLog(strLogName.c_str()))
		return false;

	LogThreadBase* pLogThread = GetLogThread(strLogName.c_str());
	if (nullptr == pLogThread)
		return false;

	std::stringstream strQueueName;
	strQueueName << "RegisterQueue:" << pLogThread->GetThreadID();

	UnLockQueueBase* pRegisterQueue = GetThreadRegisterQueue(strQueueName.str().c_str());
	if (nullptr == pRegisterQueue)
		return false;

	UnLockQueueBase* pLogQueue = new UnLockQueueBase();

	RegisterLogQueueData* pRegisterData = new RegisterLogQueueData();
	pRegisterData->pThreadLogQueue = pRegisterQueue;
	pRegisterData->bRegister = true;

	if (0 == strcmp(strLogName.c_str(), "Default"))
	{
		m_pGlobalLog = pLogQueue;
		pRegisterData->nThreadID = 0;
	}
	else
	{
		pRegisterData->nThreadID = pThread->GetThreadID();
		pThread->RegisterQueue(pLogQueue, "LogQueue", ESQT_LOG_QUEUE);
	}

	pRegisterQueue->PushQueueElement(pRegisterData, sizeof(RegisterLogQueueData));
	return true;
}
#pragma endregion

#pragma region Log about
bool SLC_LogCore::CreateLog(const char* strLogKey)
{
	return true;
}

bool SLC_LogCore::OutputLog(const char* strLogKey, int nLogLevel, const char* strLog, ...)
{
	std::string strKey = strLogKey;
	if (0 == strcmp(strKey.c_str(), ""))
		strKey = "Default";

	UnLockQueueBase* pLogQueue = GetThreadRegisterQueue(strKey.c_str());
	if (nullptr == pLogQueue)
		return false;

	if (nullptr == strLog)
		return false;

	if (strcmp(strLog, "") == 0)
		return false;

	if (nLogLevel > ELLT_ERROR)
		return false;

	if (nLogLevel < ELLT_ECHO)
		return false;

	LogQueueElementData oData;
	va_list ap;
	va_start(ap, strLog);
	vsnprintf(oData.strLog, LOG_CHARACTER_MAX, strLog, ap);
	va_end(ap);

	oData.nLogLevel = nLogLevel;
	oData.nThreadID = 0;
	pLogQueue->PushQueueElement(&oData, sizeof(oData));
	return true;
}

#pragma endregion
#pragma endregion


#pragma region Protected Log function
LogThreadBase* SLC_LogCore::GetLogThread(const char* strLogKey)
{
	if (!CheckStringValid(strLogKey))
		return nullptr;

	std::map<std::string, LogThreadBase*>::iterator iter = m_dicLogs.find(strLogKey);
	if (iter != m_dicLogs.end())
		return iter->second;

	return nullptr;
}

UnLockQueueBase* SLC_LogCore::GetThreadRegisterQueue(const char* strKey)
{
	if (!CheckStringValid(strKey))
		return nullptr;

	std::map<std::string, UnLockQueueBase*>::iterator iter = m_dicRegisterQueue.find(strKey);
	if (iter != m_dicRegisterQueue.end())
		return iter->second;

	if (0 == strcmp(strKey, LOG_DEFAULT))
		return m_pGlobalLog;

	return nullptr;
}

bool SLC_LogCore::AddNewLog(const char* strLogKey)
{

	if (!CheckStringValid(strLogKey))
		return false;

	LogThreadBase* pNewLogThread = new LogThreadBase();
	m_dicLogs.insert(std::pair<std::string, LogThreadBase*>(strLogKey, pNewLogThread));
	pNewLogThread->OnThreadInitialize(0);

	//int nThreadID = CalculateLogThreadID();

	//	Create log file
	//	fileName
	std::string strFileFullName = SLC_LOG_PATH;
	strFileFullName += strLogKey;
	char strCurTime[SLC_TIME_STRING_SIZE] = { 0 };
	GetCurTimeString(strCurTime, SLC_TIME_STRING_SIZE, ".");
	strFileFullName += strCurTime;
	strFileFullName += ".log";

	FILE* pFile = System_CreateFile(strFileFullName.c_str());
#ifdef _WIN_
	if (nullptr == m_pConsoleHandle)
	{
		m_pConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (nullptr == m_pConsoleHandle)
			return false;
	}

	//	after will put log level setting in configuration file
	pNewLogThread->BeforeLogStart(ELLT_DEBUG, ELLT_DEBUG, pFile, m_pConsoleHandle);
#else
	pNewLogThread->BeforeLogStart(ELLT_DEBUG, ELLT_DEBUG, pFile);
#endif
	int nThreadID = CalculateLogThreadID();

	//	Add Register queue in log thread
	UnLockQueueBase* pRegisterQueue = new UnLockQueueBase();
	pNewLogThread->RegisterQueue(pRegisterQueue, "RegisterQueue", ESQT_READ_QUEUE);

	std::stringstream strQueueName;
	strQueueName << "RegisterQueue:" << nThreadID;
	m_dicRegisterQueue.insert(std::pair <std::string, UnLockQueueBase*>(strQueueName.str().c_str(), pRegisterQueue));

	//	start thread
	pNewLogThread->OnThreadStart(nThreadID);

	return true;
}

bool SLC_LogCore::RemoveThread(int nThreadID)
{
	return true;
}

int SLC_LogCore::CalculateLogThreadID()
{
	int nLogCount = m_dicLogs.size();
	int nRet = ((1 << 30) | nLogCount);
	return nRet;
}

bool SLC_LogCore::CheckLogID(int nThreadID)
{
	return ((nThreadID & (1 << 30)) == (1 << 30));
}
#pragma endregion

#pragma region Destroy About
bool SLC_LogCore::StopAllLog()
{
	int nLogCount = (int)m_dicLogs.size();
	bool bRet = true;
	if (nLogCount > 0)
	{
		std::map<std::string, LogThreadBase*>::iterator iter = m_dicLogs.begin();
		for (; iter != m_dicLogs.end(); ++iter)
		{
			if (nullptr != iter->second)
			{
				bRet &= iter->second->OnThreadDestroy();
				delete iter->second;
			}
			
		}

		m_dicLogs.clear();
	}

	bRet &= (m_dicLogs.size() == 0);
	return bRet;
}
#pragma endregion

