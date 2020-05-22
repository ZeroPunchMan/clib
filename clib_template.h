#pragma once

#include "cl_common.h"


#ifdef __cplusplus
extern "C" {
#endif

//-------------event type------------------
typedef enum
{
    CL_Event_SgpRecvMsg = 0,
    CL_Event_TcpStaChanged,
    CL_Event_TcpDataFromServer,
    CL_Event_OneSecComplete,
    CL_Event_HasMan,
    CL_Event_SacPressure,
    CL_Event_OnNetworkCfg,
    CL_EventMax,
} CL_Event_t;


#ifdef __cplusplus
}
#endif