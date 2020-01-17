#include "../../Include/Common/tools.h"

std::vector<std::string> SplitString(std::string strString, std::string strParam)
{
	std::vector<std::string> vResult;
	vResult.clear();
	std::string::size_type pos;

	if (strString.length() == 0)
	{
		return vResult;
	}
	strString += strParam;//扩展字符串以方便操作
	int size = (int)strString.size();

	for (int i = 0; i < size; i++)
	{
		pos = strString.find(strParam, i);
		if (pos < size)
		{
			std::string s = strString.substr(i, pos - i);
			vResult.push_back(s);
			i = (int)(pos + strParam.size() - 1);
		}
	}

	return vResult;
}

std::vector<int> SplitStringToInt(std::string strString, std::string strParam)
{
	auto strs = SplitString(strString, strParam);
	auto ints = std::vector<int>();
	for (auto s : strs)
	{
		ints.push_back(atoi(s.c_str()));
	}
	return ints;
}

std::vector<float> SplitStringToFloat(std::string strString, std::string strParam)
{
	auto strs = SplitString(strString, strParam);
	auto ints = std::vector<float>();
	for (auto s : strs)
	{
		ints.push_back((float)atof(s.c_str()));
	}
	return ints;
}

bool CheckStringValid(const char* strValue)
{
	if (nullptr == strValue)
		return false;

	if (0 == strcmp("", strValue))
		return false;

	return true;
}

// 消除空格
size_t TrimStringLen(const char * str, size_t len, char * buf, size_t maxlen)
{
	const char * beg = str;
	const char * end = str + len - 1;

	for (; beg <= end; beg++)
	{
		if ((*beg != ' ') && (*beg != '\t'))
		{
			break;
		}
	}

	for (; end >= beg; end--)
	{
		if ((*end != ' ') && (*end != '\t'))
		{
			break;
		}
	}

	size_t size = end - beg + 1;

	if (size >= maxlen)
	{
		size = maxlen - 1;
	}

	::memcpy(buf, beg, size);

	buf[size] = 0;

	return size;
}
// 消除空格
size_t TrimString(const char * str, char * buf, size_t maxlen)
{
	return TrimStringLen(str, ::strlen(str), buf, maxlen);
}
// 消除空格
size_t TrimWideStrLen(const wchar_t * str, size_t len, wchar_t * buf, size_t maxlen)
{
	const wchar_t * beg = str;
	const wchar_t * end = str + len - 1;

	for (; beg <= end; beg++)
	{
		if ((*beg != L' ') && (*beg != L'\t'))
		{
			break;
		}
	}

	for (; end >= beg; end--)
	{
		if ((*end != L' ') && (*end != L'\t'))
		{
			break;
		}
	}

	size_t size = end - beg + 1;

	const size_t MAX_LEN = maxlen / sizeof(wchar_t);

	if (size >= MAX_LEN)
	{
		size = MAX_LEN - 1;
	}

	::memcpy(buf, beg, size * sizeof(wchar_t));

	buf[size] = 0;

	return size;
}
// 消除空格
size_t TrimWideStr(const wchar_t * str, wchar_t * buf, size_t maxlen)
{
	return TrimWideStrLen(str, ::wcslen(str), buf, maxlen);
}
