
#ifndef __INTERFACE_SYSTEM_CORE_H__
#define __INTERFACE_SYSTEM_CORE_H__

class IModule;

class ISystemHelper;

class ISystemCore
{
public:
	ISystemCore(){}
	virtual ~ISystemCore(){}

	virtual IModule* GetModule(const char* strName) = 0;

	virtual ISystemHelper* GetSystemHelper() = 0;
};


#endif	//	__INTERFACE_SYSTEM_CORE_H__
