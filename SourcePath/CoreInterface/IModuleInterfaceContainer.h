

/*

	IModule Interface Container

	Module Work Interface

	Impletement in Project. SystemCore only have a pointer for this

	creater:	Shawn Lee
	Date:		2020-2-10
*/


#ifndef __IMODULE_INTERFACE_CONTAINER_H__
#define __IMODULE_INTERFACE_CONTAINER_H__


class IModuleCoreInterface;

class IModuleInterfaceContainer
{
public:
	IModuleInterfaceContainer(){}
	virtual ~IModuleInterfaceContainer() {}

	virtual bool RegisterCoreInterface(const char* strName, IModuleCoreInterface* pInterface) = 0;
	virtual bool RemoveCoreInterface(const char* strName) = 0;
	virtual IModuleCoreInterface* GetCoreInterface(const char* strName) = 0;
};

#endif	//	__IMODULE_INTERFACE_CONTAINER_H__
