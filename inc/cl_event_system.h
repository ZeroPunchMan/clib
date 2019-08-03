#pragma once

#include "cl_common.h"
#include "cl_event_type.h"

typedef CL_BOOL (*CL_EventCallBack_t)(void* eventArg);

void CL_EventSysInit(void);

CL_RESULT CL_EventSysAddListener(CL_EventCallBack_t cb, CL_Event_t event, int session);

CL_RESULT CL_EventSysRemoveListener(CL_EventCallBack_t cb, CL_Event_t event, int session);

CL_RESULT CL_EventSysRaise(CL_Event_t event, int session, void *eventArg);
