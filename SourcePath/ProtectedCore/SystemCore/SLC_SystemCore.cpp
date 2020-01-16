
#include "SLC_SystemCore.h"
#include "../../CoreInterface/IModuleInterface.h"
#include "../../PublicLib/Include/Common/tools.h"

SLC_SystemCore::SLC_SystemCore()
{}

SLC_SystemCore::~SLC_SystemCore()
{
	if (m_dicModules.size() <= 0)
		return;

	if (nullptr != m_pSystemHelper)
		delete m_pSystemHelper;

	m_pSystemHelper = nullptr;

	std::map<const char *, IModule *>::iterator iter = m_dicModules.begin();
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

	std::map<const char *, IModule *>::iterator iter = m_dicModules.find(strName);
	if (m_dicModules.end() != iter)
		return iter->second;

	return nullptr;
}
