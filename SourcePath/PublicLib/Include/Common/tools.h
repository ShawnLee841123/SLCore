

#ifndef __PUBLIC_TOOLS_H__
#define __PUBLIC_TOOLS_H__

#include <vector>
#include <string>
#include <mutex>

std::vector<std::string> SplitString(std::string strString, std::string strParam);
std::vector<int> SplitStringToInt(std::string strString, std::string strParam);
std::vector<float> SplitStringToFloat(std::string strString, std::string strParam);
bool CheckStringValid(const char* strValue);

// 消除空格
size_t TrimStringLen(const char * str, size_t len, char * buf, size_t maxlen);
// 消除空格
size_t TrimString(const char * str, char * buf, size_t maxlen);
// 消除空格
size_t TrimWideStrLen(const wchar_t * str, size_t len, wchar_t * buf, size_t maxlen);
// 消除空格
size_t TrimWideStr(const wchar_t * str, wchar_t * buf, size_t maxlen);

class AutoLock
{
public:
	AutoLock();
	~AutoLock();

private:
	std::mutex		m_handle;
};

#endif //__PUBLIC_TOOLS_H__

