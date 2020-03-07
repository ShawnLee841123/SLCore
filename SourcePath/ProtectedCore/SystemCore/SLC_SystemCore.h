
#ifndef __SLC_SYSTEM_CORE_H__
#define __SLC_SYSTEM_CORE_H__

#include "../../CoreInterface/ISystemCore.h"
#include <map>


class SLCSystemHelper;
class IModuleInterfaceContainer;
class IModuleCoreInterface;

class SLC_SystemCore: public ISystemCore
{
public:

	friend class SLCSystemHelper;

	SLC_SystemCore();
	~SLC_SystemCore();

#pragma region Parent Interface override
	virtual IModule* GetModule(const char* strName) override;
	virtual ISystemHelper* GetSystemHelper() override;
	virtual IModuleInterfaceContainer* GetInterfaceContainer() override;
	virtual IModuleCoreInterface* GetModuleCoreInterface(const char* strName) override;

	virtual bool ReginserModuleCoreInterface(const char* strName, IModuleCoreInterface* pInterface) override;
#pragma endregion

	virtual bool OnStart();
	virtual bool OnInitialize();
	virtual bool OnDestroy();

protected:
	
#pragma region Variable
	std::map<std::string, IModule*>			m_dicModules;
	SLCSystemHelper*						m_pSystemHelper;
	IModuleInterfaceContainer*				m_pContainer;
#pragma endregion
};

#endif	//	__SLC_SYSTEM_CORE_H__
