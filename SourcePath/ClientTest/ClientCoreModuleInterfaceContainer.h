
#ifndef __CLIENT_CORE_MODULE_INTERFACE_CONTAINER_H__
#define __CLIENT_CORE_MODULE_INTERFACE_CONTAINER_H__

#include "../CoreInterface/IModuleInterfaceContainer.h"
#include <map>
#include <string>

class IModuleCoreInterface;

class ClientCoreModuleInterfaceContainer: public IModuleInterfaceContainer
{
public:
	ClientCoreModuleInterfaceContainer();
	virtual ~ClientCoreModuleInterfaceContainer();

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


#endif	//	__CLIENT_CORE_MODULE_INTERFACE_CONTAINER_H__
