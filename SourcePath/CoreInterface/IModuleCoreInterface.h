
#ifndef __INTERFACE_MODULE_CORE_INTERFACE_H__
#define __INTERFACE_MODULE_CORE_INTERFACE_H__

class IModule;
class ISystemCore;

class IModuleCoreInterface
{
public:
	IModuleCoreInterface(): m_pModule(nullptr), m_pSysCore(nullptr)
	{}
	virtual ~IModuleCoreInterface() {}

	virtual bool Initialize(IModule* pModule) = 0;
	virtual bool Startup() = 0;
	virtual bool Destroy() = 0;
	virtual bool Release() = 0;

protected:
	IModule*					m_pModule;
	ISystemCore*				m_pSysCore;

public:

	bool IsEnable() { return ((nullptr != m_pModule) && (nullptr != m_pSysCore)); }
};



#endif
