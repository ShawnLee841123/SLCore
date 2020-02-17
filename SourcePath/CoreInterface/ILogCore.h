
/*
	项目核心接口

	功能：
		1、管理日志线程
		2、日志线程和工作线程的一一对应
*/



#ifndef __INTERFACE_PROJECT_CORE_H__
#define __INTERFACE_PROJECT_CORE_H__

class ThreadBase;

class ILogCore
{
public:
	ILogCore(){}
	virtual ~ILogCore(){}

#pragma region Thread about
	//	注册线程(这里不负责创建，但是负责销毁，模块销毁的时候，会输出这里面应该移除或销毁的线程)
	//	参数列表：线程，来源（模块名称），日志关键字
	virtual bool RegisterThread(ThreadBase* pThread, const char* strSource, const char* strLogKey = "") = 0;
#pragma endregion

#pragma region Log about
	virtual bool CreateLog(const char* strLogKey) = 0;
#pragma endregion
};


#endif	//	__INTERFACE_PROJECT_CORE_H__
