
#ifndef __SLC_SYSTEM_HELPER_H__
#define __SLC_SYSTEM_HELPER_H__

#include "../../CoreInterface/ISystemHelper.h"

class SLC_SystemCore;

class SLCSystemHelper : public ISystemHelper
{
public:
	SLCSystemHelper();
	virtual ~SLCSystemHelper();

	virtual bool RegisterModule(const char* strName, IModule* pModule) override;
	virtual bool SetCurrentModule(IModule* pModule) override;
	virtual IModule* GetCurrentModule() override;
	virtual bool SetSystemCore(SLC_SystemCore* pSysCore);

	virtual bool RegisterModuleInterfaceContainer(IModuleInterfaceContainer* pContainer) override;

protected:

	SLC_SystemCore*				m_pSystemCore;
	IModule*					m_pCurModule;
};


#endif	//	__SLC_SYSTEM_HELPER_H__
