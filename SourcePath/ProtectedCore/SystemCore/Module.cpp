﻿
#include "Module.h"
#include "SLC_SystemCore.h"
#include "SLC_SystemHelper.h"

INTERFACE_MODULE(SLCSystemCore);


SLC_SystemCore g_SystemCore;
bool FULLNAME_MODULE(SLCSystemCore)::OnModuleInitialize(ISystemCore* pSysCore)
{
	m_pSystemCore = &g_SystemCore;
	SLCSystemHelper* pSysHelper = new SLCSystemHelper();
	pSysHelper->SetSystemCore(&g_SystemCore);

	return nullptr != m_pSystemCore;
}

bool FULLNAME_MODULE(SLCSystemCore)::OnThreadInitialize()
{
	return true;
}

bool FULLNAME_MODULE(SLCSystemCore)::OnStartup()
{
	return true;
}

bool FULLNAME_MODULE(SLCSystemCore)::OnDestroy()
{
	if (nullptr != m_pSystemCore)
		m_pSystemCore = nullptr;

	return true;
}