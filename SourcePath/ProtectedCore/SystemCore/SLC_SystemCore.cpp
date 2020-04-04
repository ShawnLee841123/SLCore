
#include "SLC_SystemCore.h"
#include "../../CoreInterface/IModuleInterface.h"
#include "../../CoreInterface/IModuleCoreInterface.h"
#include "../../CoreInterface/IModuleInterfaceContainer.h"
#include "../../PublicLib/Include/Common/tools.h"

SLC_SystemCore::SLC_SystemCore(): m_pContainer(nullptr), m_pLogCore(nullptr)
{}

SLC_SystemCore::~SLC_SystemCore()
{
	if (m_dicModules.size() <= 0)
		return;

	if (nullptr != m_pSystemHelper)
		delete m_pSystemHelper;

	m_pSystemHelper = nullptr;

	if (nullptr != m_pLogCore)
		m_pLogCore = nullptr;

	if (nullptr != m_pContainer)
		delete m_pContainer;
	m_pContainer = nullptr;

	std::map<std::string, IModule*>::iterator iter = m_dicModules.begin();
	for (; iter != m_dicModules.end(); iter++)
	{
		IModule* pModule = iter->second;
		if (nullptr != pModule)
		{
			//	销毁模块
			pModule->OnDestroy();
			//	释放模块指针
			delete pModule;
			pModule = nullptr;
		}
	}

	//	清理模块指针容器
	m_dicModules.clear();
}

IModule* SLC_SystemCore::GetModule(const char* strName)
{
	if (!CheckStringValid(strName))
		return nullptr;

	if (m_dicModules.size() <= 0)
		return nullptr;

	std::map<std::string, IModule*>::iterator iter = m_dicModules.find(strName);
	if (m_dicModules.end() != iter)
		return iter->second;

	return nullptr;
}

ISystemHelper* SLC_SystemCore::GetSystemHelper()
{
	return (ISystemHelper*)m_pSystemHelper;
}

IModuleInterfaceContainer* SLC_SystemCore::GetInterfaceContainer()
{
	return m_pContainer;
}

IModuleCoreInterface* SLC_SystemCore::GetModuleCoreInterface(const char* strName)
{
	if (nullptr == m_pContainer)
		return nullptr;

	return m_pContainer->GetCoreInterface(strName);
}

ILogCore* SLC_SystemCore::GetLogCore()
{
	return m_pLogCore;
}

bool SLC_SystemCore::ReginserModuleCoreInterface(const char* strName, IModuleCoreInterface* pInterface)
{
	if (nullptr == m_pContainer)
		return false;

	//	Log core
	if (0 == strcmp("SLCLOGCore", strName))
		m_pLogCore = (ILogCore*)pInterface;

	return m_pContainer->RegisterCoreInterface(strName, pInterface);
}

bool SLC_SystemCore::OnStart()
{
	if (m_dicModules.size() <= 0)
		return true;

	std::map<std::string, IModule*>::iterator iter = m_dicModules.begin();
	bool bRet = true;
	for (; iter != m_dicModules.end(); ++iter)
	{
		if (nullptr != iter->second)
			bRet &= iter->second->OnStartup();
	}

	return bRet;
}

bool SLC_SystemCore::OnInitialize()
{
	if (m_dicModules.size() <= 0)
		return true;

	std::map<std::string, IModule*>::iterator iter = m_dicModules.begin();
	bool bRet = true;
	for (; iter != m_dicModules.end(); ++iter)
	{
		if (nullptr != iter->second)
			bRet &= iter->second->OnModuleInitialize(this);
	}

	return bRet;
}

bool SLC_SystemCore::OnDestroy()
{
	if (m_dicModules.size() <= 0)
		return true;

	//	释放模块指针
	std::map<std::string, IModule*>::iterator iter = m_dicModules.begin();
	bool bRet = true;
	for (; iter != m_dicModules.end(); ++iter)
	{
		IModule* pModule = iter->second;
		if (nullptr != pModule)
		{
			bRet &= pModule->OnDestroy();
		}
	}

	return bRet;
}

bool SLC_SystemCore::OnRelease()
{
	//	释放模块指针
	std::map<std::string, IModule*>::iterator iter = m_dicModules.begin();
	bool bRet = true;
	for (; iter != m_dicModules.end(); ++iter)
	{
		IModule* pModule = iter->second;
		if (nullptr != pModule)
		{
			bRet &= pModule->OnRelease();
			delete pModule;
		}

		pModule = nullptr;
	}

	m_dicModules.clear();

	return bRet;
}

