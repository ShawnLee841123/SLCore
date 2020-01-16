
#ifndef __INTERFACE_SYSTEM_CORE_H__
#define __INTERFACE_SYSTEM_CORE_H__

class IModule;

class ISystemCore
{
public:
	ISystemCore(){}
	virtual ~ISystemCore(){}

	virtual IModule* GetModule(const char* strName) = 0;
};


#endif	//	__INTERFACE_SYSTEM_CORE_H__
