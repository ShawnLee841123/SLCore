
#ifndef __SERVER_CORE_MODULE_INTERFACE_CONTAINER_H__
#define __SERVER_CORE_MODULE_INTERFACE_CONTAINER_H__

#include "../CoreInterface/IModuleInterfaceContainer.h"
#include <map>

class IModuleCoreInterface;

class ServerCoreModuleInterfaceContainer : public IModuleInterfaceContainer
{
public:
	ServerCoreModuleInterfaceContainer();
	virtual ~ServerCoreModuleInterfaceContainer();

#pragma region Parent Interface
	virtual bool RegisterCoreInterface(const char* strName, IModuleCoreInterface* pInterface) override;
	virtual bool RemoveCoreInterface(const char* strName)  override;
	virtual IModuleCoreInterface* GetCoreInterface(const char* strName)  override;

#pragma endregion

protected:

#pragma region Variable
	std::map<std::string, IModuleCoreInterface*>		m_dicCoreInterface;
#pragma endregion
};


#endif	//	__SERVER_CORE_MODULE_INTERFACE_CONTAINER_H__
