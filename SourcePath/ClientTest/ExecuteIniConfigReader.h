
#ifndef __EXECUTE_CONFIGAURE_READER_H__
#define __EXECUTE_CONFIGAURE_READER_H__

#include <map>
#include <vector>
#include <string.h>

class IniConfigFile;

//	singlton
class ExecuteIniConfigReader
{
#pragma region singlton
public:
	static ExecuteIniConfigReader* Instance();
	virtual ~ExecuteIniConfigReader();

private:
	ExecuteIniConfigReader();
	static ExecuteIniConfigReader* _ins;

#pragma endregion

public:
	bool ReadConfig(const char* strFileName);
	bool ParseFileLine(const char* strLine);

	bool AddConfigData(const char* strConfigName, IniConfigFile* pConfig);
#pragma region Get Config Data
	const char* GetConfigStringValue(const char* strConfigName, const char* strSection, const char* strKey);
	bool GetConfigBoolValue(const char* strConfigName, const char* strSection, const char* strKey);
	int GetConfigIntValue(const char* strConfigName, const char* strSection, const char* strKey);
	float GetConfigFloatValue(const char* strConfigName, const char* strSection, const char* strKey);
	bool GetConfigItemList(const char* strConfigName, const char* strSection, std::vector<std::string>& vList);
#pragma endregion
	

protected:

	IniConfigFile* GetConfigFile(const char* strConfigName);

	std::map<const char*, IniConfigFile*>			m_dicAllConfig;
};


#endif
