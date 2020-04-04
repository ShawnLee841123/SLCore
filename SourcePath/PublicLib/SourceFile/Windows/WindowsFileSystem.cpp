

#ifdef _WIN_

#include "../../Include/Windows/WindowsFileSystem.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
//	获取当前工作路径
bool Windows_GetCurrentDir(char* strOut, UI32 strCount)
{
	int res = (int)GetCurrentDirectoryA(strCount, strOut);
	if (0 == res)
	{
		strOut[0] = 0;
		return false;
	}

	if (strCount <= res)
	{
		strOut[0] = 0;
		return false;
	}

	return true;
}

//	创建目录
bool Windows_CreatePath(const char* strName)
{
	if (nullptr == strName)
		return false;

	if (0 == strName[0])
		return false;

	if (Windows_PathExists(strName))
		return true;

	_mkdir(strName);
	return true;
}

//	删除目录
bool Windows_DeletePath(const char* strName)
{
	if (nullptr == strName)
		return false;

	if (0 == strName[0])
		return false;

	if (!Windows_PathExists(strName))
		return true;

	EFileDeleteErrorType eRetType = (EFileDeleteErrorType)remove(strName);
	return eRetType == EFDET_SUCCESS;
}

//	文件是否存在
bool Windows_FileExists(const char* strFileName)
{
	if (nullptr == strFileName)
		return false;

	if (0 == strFileName[0])
		return false;

	return Windows_CheckFilePermission(strFileName, EFCST_EXISTS) == EFPCR_SUCCESS;
}

//	目录是否存在
bool Windows_PathExists(const char* strName)
{
	if (nullptr == strName)
		return false;

	if (0 == strName[0])
		return false;

	return Windows_CheckFilePermission(strName, EFCST_EXISTS) == EFPCR_SUCCESS;
}

//	创建文件(未实现)
FILE* Windows_CreateFile(const char* strFileName)
{
	return fopen(strFileName, "w");
}

//	打开文件
FILE* Windows_OpenFile(const char* strFileName, const char* strMode)
{
	return fopen(strFileName, strMode);
}

//	删除文件
bool Windows_DeleteFile(const char* strFileName)
{
	if (nullptr == strFileName)
		return false;

	if (0 == strFileName[0])
		return false;

	if (!Windows_FileExists(strFileName))
		return true;

	EFileDeleteErrorType eRetType = (EFileDeleteErrorType)remove(strFileName);
	return eRetType == EFDET_SUCCESS;
}

//	检查文件或目录权限(非公开调用)
EFilePermissionCheckResult Windows_CheckFileOrPathPermission(const char* strName, EFileCheckSystemType eType)
{
	if (nullptr == strName)
		return EFPCR_NO_PERMISSION;

	if (0 == strName[0])
		return EFPCR_NO_PERMISSION;

	return ((_access(strName, (int)eType) < 0) ? EFPCR_NO_PERMISSION : EFPCR_SUCCESS);
}

//	检查文件或目录是否有权限（公开）
//	参数strName问路径时，只能检查路径是否存在
EFilePermissionCheckResult Windows_CheckFilePermission(const char* strName, int eType)
{
	int eRet = (int)EFPCR_NO_PERMISSION;
	if (nullptr == strName)
		return (EFilePermissionCheckResult)eRet;

	if (0 == strName[0])
		return (EFilePermissionCheckResult)eRet;

	if (EFCT_NONE == (EFileCheckType)eType)
		return (EFilePermissionCheckResult)eRet;

	if ((EFCST_EXISTS & eType) == EFCST_EXISTS)
		eRet &= (int)Windows_CheckFileOrPathPermission(strName, EFCST_EXISTS);

	if (((EFCT_READ & eType) == EFCT_READ) && ((EFCT_WRITE & eType) == EFCT_WRITE))
	{
		eRet &= (int)Windows_CheckFileOrPathPermission(strName, EFCST_READ_WRITE);
		return (EFilePermissionCheckResult)eRet;
	}

	if ((EFCT_READ & eType) == EFCT_READ)
	{
		eRet &= (int)Windows_CheckFileOrPathPermission(strName, EFCST_READ);
		return (EFilePermissionCheckResult)eRet;
	}

	if ((EFCT_WRITE & eType) == EFCT_WRITE)
	{
		eRet &= (int)Windows_CheckFileOrPathPermission(strName, EFCST_WRITE);
		return (EFilePermissionCheckResult)eRet;
	}

	return (EFilePermissionCheckResult)eRet;
}

void Windows_PrintLogTextToScreen(const char* strValue, void* pConsole, ELogLevelType eType)
{
	if (nullptr == strValue)
		return;

	if (0 == strValue[0])
		return;

	if (nullptr == pConsole)
		return;

	if (eType == ELLT_ERROR)
		SetConsoleTextAttribute(pConsole, FOREGROUND_RED);
	else if (eType == ELLT_WARNING)
		SetConsoleTextAttribute(pConsole, FOREGROUND_RED | FOREGROUND_GREEN);
	else
		SetConsoleTextAttribute(pConsole, FOREGROUND_INTENSITY);

	printf("%s", strValue);
}

SYSTEM_HANDLE Windows_LoadDynamicFile(const char* strFileName, char* strErrorCode)
{
	SYSTEM_HANDLE pHandle = (SYSTEM_HANDLE)LoadLibrary(strFileName);
	if (nullptr == pHandle)
	{
		Windows_GetLastError(strErrorCode);
	}

	return pHandle;
}

//	加载动态链接库中的符号
void* Windows_LoadDynamicFileSymbol(SYSTEM_HANDLE pHandle, const char* strSymbolName, char* strErrorCode)
{
	if (nullptr == pHandle)
		return nullptr;

	if (nullptr == strSymbolName)
		return nullptr;

	if (0 == strcmp(strSymbolName, ""))
		return nullptr;

	void* pFunc = GetProcAddress((HINSTANCE)pHandle, strSymbolName);
	if (nullptr == pFunc)
	{
		Windows_GetLastError(strErrorCode);
	}
	return pFunc;
}

//	卸载动态链接库
bool Windows_CloseDynamicFile(SYSTEM_HANDLE pHandle, char* strErrorCode)
{
	bool bRet = FreeLibrary((HINSTANCE)pHandle);
	if (!bRet)
		Windows_GetLastError(strErrorCode);
	return bRet;
}

bool Windows_GetLastError(char* strErrorCode)
{
	int nErrorCode = GetLastError();
	sprintf(strErrorCode, "%d", nErrorCode);
	if (0 == nErrorCode)
		return true;

	return false;
}
#endif


