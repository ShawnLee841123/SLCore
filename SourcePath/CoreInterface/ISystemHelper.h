
#ifndef __INTERFACE_SYSTEM_HELPER_H__
#define __INTERFACE_SYSTEM_HELPER_H__

class IModule;
class IModuleInterfaceContainer;

class ISystemHelper
{
public:
	ISystemHelper(){}
	virtual ~ISystemHelper(){}

	virtual bool RegisterModule(const char* strName, IModule* pModule) = 0;
	virtual bool SetCurrentModule(IModule* pModule) = 0;
	virtual IModule* GetCurrentModule() = 0;
	virtual bool RegisterModuleInterfaceContainer(IModuleInterfaceContainer* pContainer) = 0;

};

#endif
