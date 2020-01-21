

#ifndef __PUBLIC_LIB_SYSTEM_MACROS_H__
#define __PUBLIC_LIB_SYSTEM_MACROS_H__
//#include <windows.h>
#pragma region Share Object

#define SYSTEM_HANDLE void*
//	it`s dll file in windows
#ifdef _WIN_
#define DEF_DLL_EXPORT __declspec(dllexport)
#define DEF_MODULE_FILE_EXTRA_NAME ".dll"
#else
//	it`s so file in linux
#define DEF_DLL_EXPORT __attribute__((visibility("default")))
#define __cdecl
#define DEF_MODULE_FILE_EXTRA_NAME ".so"
#endif // _WIN_
#pragma endregion

#endif	//	__PUBLIC_LIB_SYSTEM_MACROS_H__
