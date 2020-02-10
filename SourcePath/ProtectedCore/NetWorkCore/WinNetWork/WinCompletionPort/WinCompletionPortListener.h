
#ifndef __WINDOWS_COMPLETION_PORT_LISTENER_H__
#define __WINDOWS_COMPLETION_PORT_LISTENER_H__

#include "WinCompletionPortWorker.h"

class WinCompletionPortListener : public WinCompletionPortWorker
{
public:
	WinCompletionPortListener();
	virtual ~WinCompletionPortListener();
};

#endif
