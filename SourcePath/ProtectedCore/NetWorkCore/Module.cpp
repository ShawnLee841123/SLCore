
#include "Module.h"
#include "../../CoreInterface/ISystemCore.h"
#include "../../CoreInterface/ISystemHelper.h"
//#include "SLC_SystemCore.h"
//#include "SLC_SystemHelper.h"

INTERFACE_MODULE(SLCNetWorkCore);


//SLC_SystemCore g_SystemCore;
bool FULLNAME_MODULE(SLCNetWorkCore)::OnModuleInitialize(ISystemCore* pSysCore)
{
	bool bRet = true;
	bRet &= IModuleInterface::OnModuleInitialize(pSysCore);

	//TODO: Module Initialize


	bRet &= m_pSystemCore->GetSystemHelper()->RegisterModule(Name(), this);

	return bRet;
}

bool FULLNAME_MODULE(SLCNetWorkCore)::OnThreadInitialize()
{
	return true;
}

bool FULLNAME_MODULE(SLCNetWorkCore)::OnStartup()
{
	return true;
}

bool FULLNAME_MODULE(SLCNetWorkCore)::OnDestroy()
{
	//if (nullptr != m_pSystemCore)
	//	m_pSystemCore = nullptr;

	return true;
}