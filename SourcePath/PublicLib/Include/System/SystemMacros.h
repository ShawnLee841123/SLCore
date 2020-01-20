

#ifndef __PUBLIC_LIB_SYSTEM_MACROS_H__
#define __PUBLIC_LIB_SYSTEM_MACROS_H__
//#include <windows.h>
#pragma region Share Object

#define SYSTEM_HANDLE void*
//	it`s dll file in windows
#ifdef _WIN_
#define DEF_DLL_EXPORT __declspec(dllexport)
#else
//	it`s so file in linux
#define DEF_DLL_EXPORT __attribute__((visibility("default")))
#define __cdecl
#endif // _WIN_
#pragma endregion

#endif	//	__PUBLIC_LIB_SYSTEM_MACROS_H__
