
/*
	Business Logic Interface
	Not Core interface

	can not figure out how to do with this interface.
	wait for core complete
*/


#ifndef __INTERFACE_MODULE_LOGIC_INTERFACE_H__
#define __INTERFACE_MODULE_LOGIC_INTERFACE_H__

class IModule;
class ISystemCore;

class IModuleLogicInterface
{
public:
	IModuleLogicInterface(): m_pSysCore(nullptr), m_pModule(nullptr){}
	virtual ~IModuleLogicInterface(){}

	virtual bool Initialize(IModule* pModule) = 0;
	virtual bool Startup() = 0;
	virtual bool Destroy() = 0;

protected:
	IModule*			m_pModule;
	ISystemCore*				m_pSysCore;

public:

	bool IsEnable() { return ((nullptr != m_pModule) && (nullptr != m_pSysCore)); }
};


#endif	//	__INTERFACE_MODULE_LOGIC_INTERFACE_H__
