
#ifndef __INTERFACE_MODULE_INTERFACE_H__
#define __INTERFACE_MODULE_INTERFACE_H__

#include <string.h>
#include <cstdint>
class ISystemCore;

class IModule
{
public:
	virtual ~IModule() {}

	//	模块初始化
	virtual bool OnModuleInitialize(ISystemCore* pSysCore) = 0;
	//	加入到某个线程后的初始化
	virtual bool OnThreadInitialize() = 0;
	//	开始工作
	virtual bool OnStartup() = 0;
	//	模块销毁(用于模块中停止多线程及释放线程中资源)
	virtual bool OnDestroy() = 0;
	//	模块释放资源（释放模块主线程中的资源）
	virtual bool OnRelease() = 0;
	//	获取名字
	virtual const char* Name() = 0;
	//	获取系统核心
	virtual ISystemCore* GetSystemCore() = 0;
};

#define CORE_MODULE_VERSION 0x20200115

#define CREATE_MODULE(a) \
class IModuleInterface : public IModule \
{ \
protected: \
	ISystemCore* m_pSystemCore; \
public: \
	IModuleInterface() :m_pSystemCore(nullptr){} \
	virtual ~IModuleInterface(){} \
	virtual bool OnModuleInitialize(ISystemCore* pSysCore) { m_pSystemCore = pSysCore; return nullptr != m_pSystemCore; } \
	virtual bool OnThreadInitialize() { return true; } \
	virtual bool OnDestroy() { return true; } \
	virtual bool OnRelease(){ delete this; return true; } \
	virtual const char* Name() { return "IModuleInterface"; } \
	virtual ISystemCore* GetSystemCore() { return m_pSystemCore; } \
}; \
class Module##a : public IModuleInterface \
{ \
public: \
	Module##a(){} \
	virtual ~Module##a(){} \
	virtual bool OnModuleInitialize(ISystemCore* pSysCore); \
	virtual bool OnThreadInitialize(); \
	virtual bool OnStartup(); \
	virtual bool OnDestroy(); \
	virtual bool OnRelease(); \
	virtual const char* Name() { return #a; } \
};

#define INTERFACE_MODULE(a) \
extern "C" DEF_DLL_EXPORT	\
int __cdecl Module_GetVersion() \
{ \
	return CORE_MODULE_VERSION;\
} \
IModule* g_pModule = nullptr; \
extern "C" DEF_DLL_EXPORT	\
IModule* __cdecl Module_GetModule() \
{ \
	if (nullptr == g_pModule) {g_pModule = new Module##a;} return g_pModule; \
}

#define FULLNAME_MODULE(a) Module##a

#endif	//	__INTERFACE_MODULE_INTERFACE_H__
