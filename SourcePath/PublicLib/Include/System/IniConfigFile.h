
#ifndef __PUBLIC_LIB_INI_CONFIG_FILE_H__
#define __PUBLIC_LIB_INI_CONFIG_FILE_H__

#include <string>
#include <vector>
#include <map>
#include "../../Include/Common/TypeDefines.h"

class IniConfigFile
{
public:
	IniConfigFile();
	IniConfigFile(const char* strFileName);
	~IniConfigFile();

	bool SetFileName(const char* strFileName);
	bool LoadFromFile();

#pragma region Define Protected struct
protected:

	struct SLIniConfigSection
	{
		std::string						strName;
		std::map<std::string, std::string>	dicItem;
	};
#pragma endregion

	IniConfigFile::SLIniConfigSection* GetSLIniConfigSection(const char* strName);
public:
#pragma region Value Get
	float GetFloatValue(const char* strSection, const char* strItem, float fDefaultValue = 0.0f);
	SI32 GetIntValue(const char* strSection, const char* strItem, SI32 nDefaultValue = 0);
	bool GetBoolValue(const char* strSection, const char* strItem, bool bDefaultValue = false);
	const char* GetStringValue(const char* strSection, const char* strItem, const char* strDefaultValue = "");
	bool GetSectionItemEnableList(const char* strSection, std::vector<std::string>& vList);
#pragma endregion

protected:

	bool ClearSection();

	std::map<std::string, SLIniConfigSection*>	m_dicData;

	std::string			m_strFileName;
	bool				m_bLoaded;
};



#endif	//	__PUBLIC_LIB_INI_CONFIG_FILE_H__
