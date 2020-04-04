
#include "ExecuteIniConfigReader.h"
#include "../PublicLib/Include/Common/tools.h"
#include "../PublicLib/Include/Common/TypeDefines.h"
#include "../PublicLib/Include/System/IniConfigFile.h"

#pragma region singlton
ExecuteIniConfigReader* ExecuteIniConfigReader::_ins = nullptr;
ExecuteIniConfigReader* ExecuteIniConfigReader::Instance()
{
	if (nullptr == _ins)
		_ins = new ExecuteIniConfigReader();

	return _ins;
}

ExecuteIniConfigReader::~ExecuteIniConfigReader() 
{
	SI32 nConfigCount = m_dicAllConfig.size();
	if (nConfigCount <= 0)
		return;

	std::map<std::string, IniConfigFile*>::iterator iter = m_dicAllConfig.begin();
	for (; iter != m_dicAllConfig.end(); ++iter)
	{
		IniConfigFile* pConf = iter->second;
		if (nullptr != pConf)
			delete pConf;

		pConf = nullptr;
	}

	m_dicAllConfig.clear();

	//if (nullptr != ExecuteIniConfigReader::_ins)
	//	delete ExecuteIniConfigReader::_ins;

	//ExecuteIniConfigReader::_ins = nullptr;
}

bool ExecuteIniConfigReader::DestroyInstance()
{
	if (nullptr != _ins)
		delete _ins;
	
	_ins = nullptr;

	return (nullptr == _ins);
}

ExecuteIniConfigReader::ExecuteIniConfigReader()
{
	m_dicAllConfig.clear();
}

#pragma endregion

bool ExecuteIniConfigReader::ReadConfig(const char* strFileName)
{
	if (!CheckStringValid(strFileName))
		return false;

    std::string strPath = "./";
	std::string strReadName = strPath + strFileName;
	strReadName += ".ini";
	IniConfigFile* pConf = new IniConfigFile();
	pConf->SetFileName(strReadName.c_str());
	if (!pConf->LoadFromFile())
	{
		printf("Can not Read File[%s]", strFileName);
		return false;
	}

	AddConfigData(strFileName, pConf);
	return true;
}

bool ExecuteIniConfigReader::ParseFileLine(const char* strLine)
{
	return true;
}

bool ExecuteIniConfigReader::AddConfigData(const char* strConfigName, IniConfigFile* pConfig)
{
	if (!CheckStringValid(strConfigName))
		return false;

	if (nullptr == pConfig)
	{
		printf("Config File[%s] File is Error", strConfigName);
		return false;
	}

	std::map<std::string, IniConfigFile*>::iterator iter = m_dicAllConfig.find(strConfigName);
	if (iter != m_dicAllConfig.end())
	{
		printf("Config File[%s] is Already Existed, Please recheck", strConfigName);
		return false;
	}

	m_dicAllConfig.insert(std::pair<const char*, IniConfigFile*>(strConfigName, pConfig));
		
	return true;
}

#pragma region Get Config Data
const char* ExecuteIniConfigReader::GetConfigStringValue(const char* strConfigName, const char* strSection, const char* strKey)
{
	if (!CheckStringValid(strConfigName))
		return "";

	if (!CheckStringValid(strSection))
		return "";

	if (!CheckStringValid(strKey))
		return "";

	IniConfigFile* pConf = GetConfigFile(strConfigName);
	if (nullptr == pConf)
		return "";

	return pConf->GetStringValue(strSection, strKey);
}

bool ExecuteIniConfigReader::GetConfigBoolValue(const char* strConfigName, const char* strSection, const char* strKey)
{
	if (!CheckStringValid(strConfigName))
		return false;

	if (!CheckStringValid(strSection))
		return false;

	if (!CheckStringValid(strKey))
		return false;

	IniConfigFile* pConf = GetConfigFile(strConfigName);
	if (nullptr == pConf)
		return false;

	return pConf->GetBoolValue(strSection, strKey);
}

int ExecuteIniConfigReader::GetConfigIntValue(const char* strConfigName, const char* strSection, const char* strKey)
{
	if (!CheckStringValid(strConfigName))
		return 0;

	if (!CheckStringValid(strSection))
		return 0;

	if (!CheckStringValid(strKey))
		return 0;

	IniConfigFile* pConf = GetConfigFile(strConfigName);
	if (nullptr == pConf)
		return 0;

	return pConf->GetIntValue(strSection, strKey);
}

float ExecuteIniConfigReader::GetConfigFloatValue(const char* strConfigName, const char* strSection, const char* strKey)
{
	if (!CheckStringValid(strConfigName))
		return 0.0f;

	if (!CheckStringValid(strSection))
		return 0.0f;

	if (!CheckStringValid(strKey))
		return 0.0f;

	IniConfigFile* pConf = GetConfigFile(strConfigName);
	if (nullptr == pConf)
		return 0.0f;

	return pConf->GetFloatValue(strSection, strKey);
}

bool ExecuteIniConfigReader::GetConfigItemList(const char* strConfigName, const char* strSection, std::vector<std::string>& vList)
{
	if (!CheckStringValid(strConfigName))
		return false;

	if (!CheckStringValid(strSection))
		return false;

	IniConfigFile* pConf = GetConfigFile(strConfigName);
	if (nullptr == pConf)
		return false;

	pConf->GetSectionItemEnableList(strSection, vList);

	return vList.size() > 0;
}
#pragma endregion

IniConfigFile* ExecuteIniConfigReader::GetConfigFile(const char* strConfigName)
{
	if (!CheckStringValid(strConfigName))
		return nullptr;

	SI32 nCount = (SI32)m_dicAllConfig.size();
	if (nCount <= 0)
		return nullptr;

	std::map<std::string, IniConfigFile*>::iterator iter = m_dicAllConfig.begin();
	for (; iter != m_dicAllConfig.end(); ++iter)
	{
		if (0 == strcmp(strConfigName, iter->first.c_str()))
			return iter->second;
	}

	printf("Can not Find Config[%s]", strConfigName);
	return nullptr;
}

