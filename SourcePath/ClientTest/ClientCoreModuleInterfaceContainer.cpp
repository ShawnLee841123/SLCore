
#include "ClientCoreModuleInterfaceContainer.h"
#include "../CoreInterface/IModuleCoreInterface.h"
#include "../PublicLib/Include/Common/tools.h"

ClientCoreModuleInterfaceContainer::ClientCoreModuleInterfaceContainer()
{
	m_dicCoreInterface.clear();
}

ClientCoreModuleInterfaceContainer::~ClientCoreModuleInterfaceContainer()
{
	m_dicCoreInterface.clear();
}

#pragma region Parent Interface
bool ClientCoreModuleInterfaceContainer::RegisterCoreInterface(const char* strName, IModuleCoreInterface* pInterface)
{
	if (!CheckStringValid(strName))
		return false;

	if (nullptr == pInterface)
		return false;

	std::map<std::string, IModuleCoreInterface*>::iterator iter = m_dicCoreInterface.find(strName);
	if (iter != m_dicCoreInterface.end())
		return false;

	m_dicCoreInterface.insert(std::pair<std::string, IModuleCoreInterface*>(strName, pInterface));
	return true;
}

bool ClientCoreModuleInterfaceContainer::RemoveCoreInterface(const char* strName)
{
	if (!CheckStringValid(strName))
		return false;

	std::map<std::string, IModuleCoreInterface*>::iterator iter = m_dicCoreInterface.find(strName);
	if (iter != m_dicCoreInterface.end())
	{
		m_dicCoreInterface.erase(iter);
		return true;
	}

	return false;
}

IModuleCoreInterface* ClientCoreModuleInterfaceContainer::GetCoreInterface(const char* strName)
{
	if (!CheckStringValid(strName))
		return false;

	std::map<std::string, IModuleCoreInterface*>::iterator iter = m_dicCoreInterface.find(strName);
	if (iter != m_dicCoreInterface.end())
		return iter->second;

	return nullptr;
}
#pragma endregion


