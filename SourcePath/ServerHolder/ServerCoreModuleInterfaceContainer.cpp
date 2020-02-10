#include "ServerCoreModuleInterfaceContainer.h"
#include "../CoreInterface/IModuleCoreInterface.h"
#include "../PublicLib/Include/Common/tools.h"


ServerCoreModuleInterfaceContainer::ServerCoreModuleInterfaceContainer()
{
	m_dicCoreInterface.clear();
}

ServerCoreModuleInterfaceContainer::~ServerCoreModuleInterfaceContainer()
{
	m_dicCoreInterface.clear();
}

#pragma region Parent Interface
bool ServerCoreModuleInterfaceContainer::RegisterCoreInterface(const char* strName, IModuleCoreInterface* pInterface)
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

bool ServerCoreModuleInterfaceContainer::RemoveCoreInterface(const char* strName)
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

IModuleCoreInterface* ServerCoreModuleInterfaceContainer::GetCoreInterface(const char* strName)
{
	if (!CheckStringValid(strName))
		return nullptr;

	if (m_dicCoreInterface.size() <= 0)
		return nullptr;

	std::map<std::string, IModuleCoreInterface*>::iterator iter = m_dicCoreInterface.find(strName);
	if (iter != m_dicCoreInterface.end())
		return iter->second;

	return nullptr;
}
#pragma endregion
