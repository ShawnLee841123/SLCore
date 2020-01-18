
#include "Module.h"
#include "NetWorkCore.h"
#include "../../CoreInterface/ISystemCore.h"
#include "../../CoreInterface/ISystemHelper.h"


INTERFACE_MODULE(SLCNetWorkCore);


SL_NetWorkCore g_Core;
bool FULLNAME_MODULE(SLCNetWorkCore)::OnModuleInitialize(ISystemCore* pSysCore)
{
	bool bRet = true;
	bRet &= IModuleInterface::OnModuleInitialize(pSysCore);

	//TODO: Module Initialize
	bRet &= g_Core.Initialize(pSysCore);

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
	g_Core.Destroy();
	return true;
}