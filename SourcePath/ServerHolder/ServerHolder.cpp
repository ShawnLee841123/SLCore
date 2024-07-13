// ServerCore.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>

#include "ServerCore.h"

#include "../CoreInterface/ISystemCore.h"
#include "../CoreInterface/INetWorkCore.h"
#include "../CoreInterface/IModuleInterface.h"
#include "../PublicLib/Include/Common/tools.h"

#ifdef _WIN_
#include <windows.h>
#else
#include <cerrno>
#include <iostream>
#include <csignal>
#include <unistd.h>
#endif

//ServerHolderCore oCore;
ServerHolderCore* g_pCore = nullptr;

bool MainDestroy()
{
	//return oCore.Destroy();
	//return g_pCore->Destroy();
	g_pCore->Destroy();
	return true;
}

#ifdef _WIN_
static BOOL WINAPI console_ctrl_handler(DWORD type)
{
	AutoLock oLock;
	switch (type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT:
		{
			return MainDestroy();
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
#else
void signal_handler(int signal)
{
	switch (signal)
	{
	case SIGINT:
		{
			MainDestroy();
		}
		break;
	case SIGCONT:
	{
		if (nullptr != g_pCore)
			g_pCore->SetPause(false);
	}
	break;
	default:
		break;
	}
}
#endif

bool ReleaseGlobalPtr()
{
	if (nullptr != g_pCore)
		delete g_pCore;

	g_pCore = nullptr;

	return nullptr == g_pCore;
}

bool ExeMain()
{
#ifdef _WIN_
	SetConsoleCtrlHandler(console_ctrl_handler, true);
#else
	struct sigaction sa;
	sa.sa_handler = &signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, nullptr);
	sigaction(SIGCONT, &sa, nullptr);
#endif
	g_pCore = new ServerHolderCore();
	if (!g_pCore->Initialize())
	{
		ReleaseGlobalPtr();
		return false;
	}

	if (!g_pCore->Start())
	{
		ReleaseGlobalPtr();
		return false;
	}

	printf("Server Holder Start Success!\n");
	g_pCore->MainLoop();

	MainDestroy();
	g_pCore->Release();

	ReleaseGlobalPtr();

	return true;
}

int main()
{
	bool bRet = false;
	try
	{
		bRet = ExeMain();
	}
	catch (...)
	{
#ifdef _WIN_
	int nError = GetLastError();
	if (nError != 0)
	{
		printf("Server Holder Get ErrorCode[%d]", nError);
	}
#else
	if (errno != 0)
	{
		printf("Server Holder Get ErrorCode[%d]", errno);
	}
#endif
	}	

	getchar();
	return 1;
}

