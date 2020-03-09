

#include "Module.h"
//#include "SLC_SystemCore.h"
//#include "SLC_SystemHelper.h"
#include "SLC_LogCore.h"
#include "../../CoreInterface/ISystemCore.h"
#include "../../CoreInterface/ISystemHelper.h"
#include "../../PublicLib/Include/System/SystemMacros.h"

INTERFACE_MODULE(SLCLOGCore);


SLC_LogCore g_LogCore;
bool FULLNAME_MODULE(SLCLOGCore)::OnModuleInitialize(ISystemCore* pSysCore)
{
	bool bRet = true;
	bRet &= IModuleInterface::OnModuleInitialize(pSysCore);
	bRet &= g_LogCore.Initialize(this);
	ISystemHelper* pSysHelper = m_pSystemCore->GetSystemHelper();
	if (nullptr == pSysHelper)
		return false;

	bRet &= pSysHelper->RegisterModule("LogCore", this);
	bRet &= m_pSystemCore->ReginserModuleCoreInterface("SLCLOGCore", &g_LogCore);

	return bRet;
}

bool FULLNAME_MODULE(SLCLOGCore)::OnThreadInitialize()
{
	return true;
}

bool FULLNAME_MODULE(SLCLOGCore)::OnStartup()
{
	//return g_SystemCore.OnStart();
	return true;
}

bool FULLNAME_MODULE(SLCLOGCore)::OnDestroy()
{
	//g_SystemCore.OnDestroy();

	//if (nullptr != m_pSystemCore)
	//	m_pSystemCore = nullptr;

	return true;
}