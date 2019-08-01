#pragma once

#include "cl_common.h"

#define CL_EVENT_MAX_NUM    20

typedef CL_BOOL (*CL_EventCallBack)(void* eventArg);

void CL_EventSysInit(void);
CL_RESULT CL_EventSysAddListener(CL_EventCallBack cb, int event, int session);
CL_RESULT CL_EventSysRemoveListener(CL_EventCallBack cb, int event, int session);
CL_RESULT CL_EventSysRaise(int event, int session, void *eventArg);
