

#include "ClientDemoCore.h"

int main()
{

	ClientDemoCore oDemo;
	oDemo.Initialize();
	oDemo.Start();
	oDemo.MainLoop();
	//oDemo.Destroy();
	//int nValue = atoi("11111");
	//unsigned short uShortValue = nValue;

	//getchar();
	return 0;
}