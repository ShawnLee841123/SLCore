// ServerCore.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>

//#include "PortCompletion/PortCompleteCore.h"
//#ifdef _WIN_
//#include "../InterNetCore/PortCompletion/PortCompleteCore.h"
//#else
//#endif

#include "ServerCore.h"

#include "../CoreInterface/ISystemCore.h"
#include "../CoreInterface/INetWorkCore.h"
#include "../CoreInterface/IModuleInterface.h"

#ifdef _WIN_
#include <windows.h>
#endif

ServerHolderCore oCore;

bool MainDestroy()
{
	return oCore.Destroy();
}

#ifdef _WIN_
static BOOL WINAPI console_ctrl_handler(DWORD type)
{
	switch (type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT:
		return MainDestroy();
	default:
		return FALSE;
	}
	return TRUE;
}
#endif

int main()
{

//#ifdef _WIN_
//	PortCompleteCore oCore;
//	if (!oCore.Initialize(nullptr))
//		return 0;
//
//	if (!oCore.Start())
//		return 0;
//
//	while (true)
//	{
//		oCore.Tick(0);
//	}
//
//	oCore.Destroy();
//#else
//#endif

#ifdef _WIN_
	SetConsoleCtrlHandler(console_ctrl_handler, true);
#endif
	
	if (!oCore.Initialize())
		return 0;

	if (!oCore.Start())
		return 0;

	printf("Server Holder Start Success!\n");
	oCore.MainLoop();

	MainDestroy();

	getchar();
	
	return 0;
}

