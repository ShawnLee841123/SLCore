

#include "../../Include/Common/LibThreadBase.h"
#include "../../Include/Common/UnLockQueue.h"
#include "../../Include/Common/StandardUnLockElement.h"
#include <stdarg.h>
#include <string.h>


ThreadBase::ThreadBase() : m_eCurStatus(ESTST_NONE), m_nThreadID(-1), m_nLogQueueID(-1), m_nTickTime(0), m_uLastTimeStamp(0), m_nReadQueueCount(0), m_nWriteQueueCount(0), m_pLogCore(nullptr)
{
	m_dicQueueKey.clear();
}

ThreadBase::~ThreadBase()
{}

bool ThreadBase::RegisterQueue(std::shared_ptr<UnLockQueueBase> pQueue, const char* strQueueName, EStandQueueType eType)
{
	if (!pQueue)
		return false;
	if (ESQT_READ_QUEUE != eType)
		return false;
	if (nullptr == strQueueName || 0 == strQueueName[0])
	{
		THREAD_ERROR("RegisterQueue: queue name is null or empty");
		return false;
	}
	if (m_nReadQueueCount >= THREAD_QUEUE_MAX)
	{
		THREAD_ERROR("RegisterQueue: read queue is Full");
		return false;
	}
	SI32 nQueueIndex = (ESQT_READ_QUEUE << 16) + m_nReadQueueCount;
	m_arrReadQueue[m_nReadQueueCount] = pQueue;
	m_dicQueueKey.insert(std::pair<std::string, SI32>(strQueueName, nQueueIndex));
	m_nReadQueueCount++;
	return true;
}

bool ThreadBase::RegisterQueue(std::weak_ptr<UnLockQueueBase> pQueue, const char* strQueueName, EStandQueueType eType)
{
	if (pQueue.expired())
		return false;
	if (ESQT_WRITE_QUEUE != eType && ESQT_LOG_QUEUE != eType)
		return false;
	if (nullptr == strQueueName || 0 == strQueueName[0])
	{
		THREAD_ERROR("RegisterQueue: queue name is null or empty");
		return false;
	}
	if (m_nWriteQueueCount >= THREAD_QUEUE_MAX)
	{
		THREAD_ERROR("RegisterQueue: write queue is Full");
		return false;
	}
	SI32 nQueueIndex = (eType << 16) + m_nWriteQueueCount;
	if (ESQT_LOG_QUEUE == eType)
		m_nLogQueueID = m_nWriteQueueCount;
	m_arrWriteQueue[m_nWriteQueueCount] = pQueue;
	m_dicQueueKey.insert(std::pair<std::string, SI32>(strQueueName, nQueueIndex));
	m_nWriteQueueCount++;
	return true;
}

bool ThreadBase::OnThreadInitialize(SI32 nTickTime)
{
	m_eCurStatus = ESTST_INITIALIZED;
	return true;
}

bool ThreadBase::OnThreadStart(SI32 nThreadID)
{
	m_nThreadID = nThreadID;
	m_eCurStatus = ESTST_START;
	m_Thread = std::thread(&ThreadBase::ThreadTick, this);
	THREAD_DEBUG("Thread Start");
	return true;
}

bool ThreadBase::OnThreadRunning()
{
	m_eCurStatus = ESTST_RUNNING;
	bool bRet = true;
	bRet &= ReadQueueProcess(0);

	return bRet;
}

bool ThreadBase::OnThreadDestroy()
{
	if (m_eCurStatus < ESTST_STOPED)
		return true;

	if (m_Thread.joinable())
		m_Thread.join();

	//	清空队列：读队列由 shared_ptr 拥有，Destroy 后 reset；写队列为 weak_ptr 无需释放
	if (m_dicQueueKey.size() > 0)
	{
		std::map<std::string, SI32>::iterator iter = m_dicQueueKey.begin();
		for (; iter != m_dicQueueKey.end(); ++iter)
		{
			if (!IsReadQueueType(iter->second))
				continue;
			SI32 nCurIndex = ((iter->second) & 0x0000FFFF);
			if (m_arrReadQueue[nCurIndex])
			{
				m_arrReadQueue[nCurIndex]->Destroy();
				m_arrReadQueue[nCurIndex].reset();
			}
		}
		m_dicQueueKey.clear();
		m_nReadQueueCount = 0;
		m_nWriteQueueCount = 0;
	}

	return true;
}

bool ThreadBase::OnThreadClose()
{
	//	最后设置标记
	m_eCurStatus = ESTST_STOPED;

	return true;
}

EServerThreadStatusType ThreadBase::GetThreadStatus()
{
	return m_eCurStatus;
}

SI32 ThreadBase::GetThreadID()
{
	return m_nThreadID;
}

void ThreadBase::SetThreadID(SI32 nThreadID, EThreadFunctionMaskType eMask)
{
	m_eThreadMask = eMask;
	SI32 nMask = eMask << 16;
	m_nThreadID = (nMask & 0xFFFF0000) + nThreadID;
}

void ThreadBase::ThreadTick()
{
	while (m_eCurStatus >= ESTST_START && m_eCurStatus < ESTST_STOPED)
	{
		OnThreadRunning();
		//	线程退出循环
		if (m_eCurStatus > ESTST_SLEPT)
			break;

		m_eCurStatus = ESTST_SLEPT;
	}

	m_eCurStatus = ESTST_DESTROIED;
}

bool ThreadBase::ReadQueueProcess(SI32 nElapse)
{
	if (m_dicQueueKey.size() <= 0)
		return true;

	std::map<std::string, SI32>::iterator iter = m_dicQueueKey.begin();
	for (; iter != m_dicQueueKey.end(); ++iter)
	{
		if (!IsReadQueueType(iter->second))
			continue;

		SI32 nQueueIndex = ((iter->second) & 0x0000FFFF);
		std::shared_ptr<UnLockQueueBase> pQueue = m_arrReadQueue[nQueueIndex];
		if (!pQueue)
		{
			THREAD_ERROR("Func[ReadQueueProcess] Queue[%s] is null", iter->first.c_str());
			continue;
		}

		EQueueOperateResultType eRet = EQORT_SUCCESS;
		do 
		{
			UnLockQueueElementBase* pElement = pQueue.get()->PopQueueElement(eRet);

			//	接到结束信息，就啥也别干了
			ThreadCloseElement* pClose = dynamic_cast<ThreadCloseElement*>(pElement);
			if (nullptr != pClose)
				return OnThreadClose();

			if (!OnQueueElement(pElement))
			{
				if (nullptr != pElement)
				{
					pElement->ClearElement();
					delete pElement;
				}
				eRet = EQORT_POP_INVALID_ELEMENT;
				THREAD_WARNNING("Func[ReadQueueProcess] ReadQueue[%s] have invalid element", iter->first.c_str());
				break;
			}

		} while (eRet == EQORT_SUCCESS);
	}

	return true;
}

bool ThreadBase::OnQueueElement(UnLockQueueElementBase* pElement)
{
	if (nullptr == pElement)
	{
		THREAD_ERROR("Func[OnQueueElement] Param[pElement] is null");
		return false;
	}

	return true;
}

bool ThreadBase::AddQueueElement(UnLockQueueElementBase* pElement, const char* strQueueName)
{
	if (nullptr == strQueueName)
	{
		THREAD_ERROR("Func[AddQueueElement] Param[strQueueName] is null");
		return false;
	}

	if (0 == strQueueName[0])
	{
		THREAD_ERROR("Func[AddQueueElement] Param[strQueueName] is Empty string");
		return false;
	}

	if (nullptr == pElement)
	{
		THREAD_ERROR("Func[AddQueueElement] Param[pElement] is null");
		return false;
	}
		
	std::map<std::string, SI32>::iterator iter = m_dicQueueKey.find(strQueueName);
	if (iter == m_dicQueueKey.end())
	{
		THREAD_ERROR("Func[AddQueueElement] Can not find write queue[%s]", strQueueName);
		return false;
	}

	if (!IsWriteQueueType(iter->second) && !IsLogQueue(iter->second))
	{
		THREAD_ERROR("Func[AddQueueElement] Queue[%s] is not Write/Log queue", strQueueName);
		return false;
	}

	SI32 nQueueIndex = ((iter->second) & 0x0000FFFF);
	std::shared_ptr<UnLockQueueBase> pQueue = m_arrWriteQueue[nQueueIndex].lock();
	if (!pQueue)
	{
		THREAD_ERROR("Func[AddQueueElement] Queue[%s] expired (reader destroyed)", strQueueName);
		return false;
	}

	EQueueOperateResultType eRet = pQueue->PushQueueElement(pElement);
	return EQORT_SUCCESS == eRet;
}

SI32 ThreadBase::GetQueueID(const char* strQueueName)
{
	if (nullptr == strQueueName)
		return -1;

	if (strQueueName[0] == 0)
		return -1;

	std::map<std::string, SI32>::iterator iter = m_dicQueueKey.find(strQueueName);
	if (iter != m_dicQueueKey.end())
		return iter->second;

	return -1;
}

SI32 ThreadBase::GetQueueIndex(SI32 nQueueID)
{
	SI32 nIndex = (nQueueID & 0x0000FFFF);
	return nIndex;
}

SI32 ThreadBase::GetQueueIndex(const char* strQueueName)
{
	SI32 nQueueID = GetQueueID(strQueueName);

	return GetQueueIndex(nQueueID);
}

bool ThreadBase::IsReadQueueType(SI32 nQueueID)
{
	return ((nQueueID >> 16) == ESQT_READ_QUEUE);
}

bool ThreadBase::IsWriteQueueType(SI32 nQueueID)
{
	return ((nQueueID >> 16) == ESQT_WRITE_QUEUE);
}

bool ThreadBase::IsLogQueue(SI32 nQueueID)
{
	return ((nQueueID >> 16) == ESQT_LOG_QUEUE);
}

bool ThreadBase::Output(SI32 nLevel, const char* strLog, ...)
{
	if (nullptr == strLog)
		return false;

	if (strcmp(strLog, "") == 0)
		return false;

	if (m_nLogQueueID < 0)
		return false;

	if (nLevel > ELLT_ERROR)
		return false;

	if (nLevel < ELLT_ECHO)
		return false;

	LogQueueElementData oData;
	va_list ap;
	va_start(ap, strLog);
	vsnprintf(oData.strLog, LOG_CHARACTER_MAX, strLog, ap);
	va_end(ap);

	oData.nLogLevel = nLevel;
	oData.nThreadID = m_nThreadID;
	std::shared_ptr<UnLockQueueBase> pLogQueue = m_arrWriteQueue[m_nLogQueueID].lock();
	if (!pLogQueue)
		return false;
	EQueueOperateResultType eRet = pLogQueue->PushQueueElement(&oData, sizeof(oData));
	return EQORT_SUCCESS == eRet;
}

