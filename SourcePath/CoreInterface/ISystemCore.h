
#ifndef __INTERFACE_SYSTEM_CORE_H__
#define __INTERFACE_SYSTEM_CORE_H__

class IModule;

class ISystemHelper;
class IModuleInterfaceContainer;
class IModuleCoreInterface;
class ILogCore;

class ISystemCore
{
public:
	ISystemCore(){}
	virtual ~ISystemCore(){}

	virtual IModule* GetModule(const char* strName) = 0;

	virtual ISystemHelper* GetSystemHelper() = 0;
	virtual IModuleInterfaceContainer* GetInterfaceContainer() = 0;
	virtual IModuleCoreInterface* GetModuleCoreInterface(const char* strName) = 0;
	virtual ILogCore* GetLogCore() = 0;

	virtual bool ReginserModuleCoreInterface(const char* strName, IModuleCoreInterface* pInterface) = 0;
};


#endif	//	__INTERFACE_SYSTEM_CORE_H__
