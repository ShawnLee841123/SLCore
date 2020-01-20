
#ifndef __LIB_FILE_SYSTEM_H__
#define __LIB_FILE_SYSTEM_H__

#include "../Common/TypeDefines.h"
#include "../System/SystemMacros.h"
#include <stdio.h>
//	获取当前工作路径
bool GetCurrentDir(char* strOut, UI32 strCount);

//	创建目录
bool CreatePath(const char* strName);

//	删除目录
bool DeletePath(const char* strName);

//	文件是否存在
bool FileExists(const char* strFileName);

//	目录是否存在
bool PathExists(const char* strName);

//	创建文件(未实现)
FILE* System_CreateFile(const char* strFileName);

//	打开文件
FILE* System_OpenFile(const char* strFileName, const char* strMode);

//	删除文件
bool System_DeleteFile(const char* strFileName);

//	检查文件或目录权限(非公开调用)
EFilePermissionCheckResult CheckFileOrPathPermission(const char* strName, EFileCheckSystemType eType);

//	检查文件或目录是否有权限（公开）
//	参数strName问路径时，只能检查路径是否存在
EFilePermissionCheckResult CheckFilePermission(const char* strName, int eType);

//	打印信息到控制台
void PrintLogTextToScreen(const char* strValue, void* pConsole, ELogLevelType eType);

//	加载动态链接库
SYSTEM_HANDLE LoadDynamicFile(const char* strFileName);

//	加载动态链接库中的符号
void* LoadDynamicFileSymbol(SYSTEM_HANDLE pHandle, const char* strSymbolName);

//	卸载动态链接库
bool CloseDynamicFile(SYSTEM_HANDLE pHandle);

#endif		//	__LIB_FILE_SYSTEM_H__
