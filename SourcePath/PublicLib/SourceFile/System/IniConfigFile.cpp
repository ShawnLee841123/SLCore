
#include "../../Include/System/IniConfigFile.h"
#include "../../Include/Common/tools.h"
#include "../../Include/System/FileSystem.h"
#include "../../Include/Common/Util.h"
#ifndef _WIN_
#include <string.h>
#endif
IniConfigFile::IniConfigFile(): m_bLoaded(false), m_strFileName("")
{
	m_dicData.clear();
}

IniConfigFile::IniConfigFile(const char* strFileName): m_bLoaded(false), m_strFileName(strFileName)
{
	m_dicData.clear();
}

IniConfigFile::~IniConfigFile()
{
	ClearSection();
}

bool IniConfigFile::SetFileName(const char* strFileName)
{
	m_strFileName = strFileName;
	return true;
}

bool IniConfigFile::LoadFromFile()
{
	m_bLoaded = !ClearSection();
	std::string strTempFileName = m_strFileName;
	FILE* pFile = System_OpenFile(strTempFileName.c_str(), "rb");

	if (nullptr == pFile)
		return false;

	//	Get File Size
	fseek(pFile, 0, SEEK_END);
	UI32 uSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	SLDynamicBuffer Buf(uSize + 2);

	char* fileContent = Buf.GetBuffer();
	if (uSize != fread(fileContent, sizeof(char), uSize, pFile))
	{
		fclose(pFile);
		return false;
	}

	bool bUTF8 = false;
	//	UTF-8 File Head
	if ((char)0xEF == fileContent[0])
	{
		if ((char)0xBB == fileContent[1])
		{
			if ((char)0xBF == fileContent[2])
			{
				fileContent += 3;
				uSize		-= 3;
				bUTF8 = true;
			}
		}
	}

	if (!bUTF8)
	{
		printf("File[%s] must be UTF-8 WITH BOM file", m_strFileName.c_str());
		return false;
	}

	fileContent[uSize] = '\r';
	fileContent[uSize + 1] = '\n';

	fclose(pFile);

#pragma region split file content to lines
	std::vector<const char*> lines;
	lines.reserve(256);

	//	char to lines
	UI32 uCount = 0;
	const UI32 FILESIZE = uSize + 2;
	for (UI32 i = 0; i < FILESIZE; i++)
	{
		if ('\r' == fileContent[i] || '\n' == fileContent[i])
		{
			fileContent[i] = 0;
			uCount = 0;
		}
		else
		{
			if (0 == uCount)
			{
				lines.push_back(&fileContent[i]);
			}
			uCount++;
		}
	}
#pragma endregion

#pragma region Parse file lines
	std::vector<char> s;
	std::vector<char> t;

	s.reserve(256);
	t.reserve(256);

	SLIniConfigSection* pSLSec = nullptr;
	UI32 uLineCount = (UI32)lines.size();
	for (UI32 i = 0; i < uLineCount; i++)
	{
		UI32 uStrCount = strlen(lines[i]);
		s.resize(uStrCount + 1);

		uStrCount = TrimStringLen(lines[i], uStrCount, &s[0], s.size());

		if (uStrCount < 2)
			continue;

#pragma region Parse section

		const char* strCurLine = &s[0];
		if ((uStrCount > 2) && ('[' == s[0]) && (']' == s[uStrCount - 1]))
		{
			std::string strSecName(strCurLine + 1, uStrCount - 2);

			pSLSec = GetSLIniConfigSection(strSecName.c_str());
			if (nullptr == pSLSec)
			{
				pSLSec = new SLIniConfigSection();
				pSLSec->strName = strSecName;
				m_dicData.insert(std::pair<std::string, SLIniConfigSection *>(strSecName, pSLSec));
			}

			continue;
		}
#pragma endregion

		if ('/' == s[0] && '/' == s[1])
			continue;

		if (';' == s[0])
			continue;

		if ('#' == s[0])
			continue;

		if (nullptr == pSLSec)
			continue;

		const char* strEqual = strchr(strCurLine, '=');
		if (nullptr == strEqual || strEqual == strCurLine)
			continue;

#pragma region Parse section item
		t.resize(uStrCount + 1);
		TrimStringLen(&s[0], (strEqual - strCurLine), &t[0], t.size());
		std::string strKey = &t[0];
		std::string strValue = "";
		if ((strCurLine + uStrCount) > (strEqual + 1))
		{
			TrimString(strEqual + 1, &t[0], t.size());
			strValue = &t[0];
		}
		else
			strKey = "";

		if (0 != strcmp("", strKey.c_str()))
			pSLSec->dicItem.insert(std::pair<std::string, std::string>(strKey, strValue));

#pragma endregion
	}
#pragma endregion

	m_bLoaded = true;
	return m_bLoaded;
}

IniConfigFile::SLIniConfigSection* IniConfigFile::GetSLIniConfigSection(const char* strName)
{
	if (!CheckStringValid(strName))
		return nullptr;

	if (m_dicData.size() <= 0)
		return nullptr;

	std::map<std::string, SLIniConfigSection *>::iterator iter = m_dicData.find(strName);
	if (iter == m_dicData.end())
		return nullptr;

	return iter->second;
}

#pragma region Value Get
float IniConfigFile::GetFloatValue(const char* strSection, const char* strItem, float fDefaultValue)
{
	const char* strValue = GetStringValue(strSection, strItem);
	if (!CheckStringValid(strValue))
		return fDefaultValue;

	return (float)atof(strValue);
}

SI32 IniConfigFile::GetIntValue(const char* strSection, const char* strItem, SI32 nDefaultValue)
{
	const char* strValue = GetStringValue(strSection, strItem);
	if (!CheckStringValid(strValue))
		return nDefaultValue;

	return atoi(strValue);
}

bool IniConfigFile::GetBoolValue(const char* strSection, const char* strItem, bool bDefaultValue)
{
	const char* strValue = GetStringValue(strSection, strItem);
	if (!CheckStringValid(strValue))
		return bDefaultValue;

	if (0 == strcmp("true", strValue))
		return true;

	if (0 == strcmp("yes", strValue))
		return true;

	if (0 == strcmp("t", strValue))
		return true;

	if (0 == strcmp("y", strValue))
		return true;

	if (0 == strcmp("1", strValue))
		return true;

	return false;
}

const char* IniConfigFile::GetStringValue(const char* strSection, const char* strItem, const char* strDefaultValue)
{
	if (!CheckStringValid(strSection))
		return strDefaultValue;

	if (!CheckStringValid(strItem))
		return strDefaultValue;

	SLIniConfigSection* pSection = GetSLIniConfigSection(strSection);
	if (nullptr == pSection)
		return strDefaultValue;

	if (pSection->dicItem.size() <= 0)
		return strDefaultValue;

	std::map<std::string, std::string>::iterator iter = pSection->dicItem.find(strItem);
	if (iter == pSection->dicItem.end())
		return strDefaultValue;

	return iter->second.c_str();
}

bool IniConfigFile::GetSectionItemEnableList(const char* strSection, std::vector<std::string>& vList)
{
	if (!CheckStringValid(strSection))
		return false;

	SLIniConfigSection* pSection = GetSLIniConfigSection(strSection);
	if (nullptr == pSection)
		return false;

	if (pSection->dicItem.size() <= 0)
		return true;

	std::vector<std::string> tempList;
	std::map<std::string, std::string>::iterator iter = pSection->dicItem.begin();
	for (; iter != pSection->dicItem.end(); ++iter)
	{
		tempList.push_back(iter->first);
	}

	SI32 nDestCount = tempList.size();
	if (nDestCount > 0)
	{
		for (SI32 i = 0; i < nDestCount; i++)
		{
			if (GetBoolValue(strSection, tempList[i].c_str()))
				vList.push_back(tempList[i]);
		}
	}

	return vList.size() > 0;
}
#pragma endregion

bool IniConfigFile::ClearSection()
{
	SI32 nSectionCount = (SI32)m_dicData.size();
	if (nSectionCount <= 0)
		return true;

	std::map<std::string, SLIniConfigSection*>::iterator iter = m_dicData.begin();
	for (; iter != m_dicData.end(); ++iter)
	{
		SLIniConfigSection* pSection = iter->second;
		if (nullptr != pSection)
		{
			pSection->dicItem.clear();
			delete pSection;
		}

		pSection = nullptr;
	}

	m_dicData.clear();

	return true;
}
