

#include "Module.h"
//#include "SLC_SystemCore.h"
//#include "SLC_SystemHelper.h"
#include "../../CoreInterface/ISystemCore.h"
#include "../../PublicLib/Include/System/SystemMacros.h"

INTERFACE_MODULE(SLCLOGCore);


//SLC_SystemCore g_SystemCore;
bool FULLNAME_MODULE(SLCLOGCore)::OnModuleInitialize(ISystemCore* pSysCore)
{
	//m_pSystemCore = &g_SystemCore;
	//SLCSystemHelper* pSysHelper = new SLCSystemHelper();
	//pSysHelper->SetSystemCore(&g_SystemCore);

	//return nullptr != m_pSystemCore;
	return true;
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