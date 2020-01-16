

#include "SLC_SystemHelper.h"
#include "SLC_SystemCore.h"
#include "../../CoreInterface/IModuleInterface.h"
#include "../../PublicLib/Include/Common/tools.h"

SLCSystemHelper::SLCSystemHelper(): m_pSystemCore(nullptr), m_pCurModule(nullptr)
{}

SLCSystemHelper::~SLCSystemHelper()
{
	if (nullptr != m_pSystemCore)
		m_pSystemCore = nullptr;

	if (nullptr != m_pCurModule)
		m_pCurModule = nullptr;
}

bool SLCSystemHelper::RegisterModule(const char* strName, IModule* pModule)
{
	if (nullptr == pModule)
		return false;

	if (!CheckStringValid(strName))
		return false;

	if (nullptr == m_pSystemCore)
		return false;

	std::map<const char*, IModule*>::iterator iter = m_pSystemCore->m_dicModules.find(strName);
	if (m_pSystemCore->m_dicModules.end() != iter)
		return false;

	m_pSystemCore->m_dicModules.insert(std::pair<const char*, IModule*>(strName, pModule));
	return true;
}

bool SLCSystemHelper::SetCurrentModule(IModule* pModule)
{
	if (nullptr == pModule)
		return false;

	m_pCurModule = pModule;
	return nullptr != m_pCurModule;
}

IModule* SLCSystemHelper::GetCurrentModule()
{
	return m_pCurModule;
}

bool SLCSystemHelper::SetSystemCore(SLC_SystemCore* pSysCore)
{
	if (nullptr == pSysCore)
		return false;

	m_pSystemCore = pSysCore;
	m_pSystemCore->m_pSystemHelper = this;
	return nullptr != m_pSystemCore;
}
