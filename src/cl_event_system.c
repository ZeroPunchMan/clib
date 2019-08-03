#include "cl_event_system.h"
#include "cl_list.h"
#include "cl_pool.h"

typedef struct
{
    CL_ListNode_t node;
    CL_EventCallBack_t callBack;
    int session;
} Listener_t;

CL_POOL_DEFINE(lsrPool, CL_EventMax * 10, Listener_t, static);

static CL_List_t lsrListArray[CL_EventMax];
void CL_EventSysInit(void)
{
    CL_PoolInit(&lsrPool);
    for (int i = 0; i < CL_EventMax; i++)
    {
        CL_ListInit(&lsrListArray[i]);
    }
}

CL_RESULT CL_EventSysAddListener(CL_EventCallBack_t cb, CL_Event_t event, int session)
{
    if (event >= CL_EventMax)
    {
        return CL_INVALID_PARAM;
    }

    Listener_t *pLsr = CL_POOL_ALLOC(&lsrPool, Listener_t);
    if (pLsr == CL_NULL)
    {
        return CL_FAILED;
    }

    pLsr->callBack = cb;
    pLsr->session = session;

    return CL_ListAddLast(&lsrListArray[event], &pLsr->node);
}

CL_RESULT CL_EventSysRemoveListener(CL_EventCallBack_t cb, CL_Event_t event, int session)
{
    if (event >= CL_EventMax)
    {
        return CL_INVALID_PARAM;
    }

    CL_List_t* lsrList = &lsrListArray[event];
    Listener_t* pLsr;
    CL_LIST_FOR_EACH_ENTRY_SAFE(lsrList, pLsr, Listener_t, node)
    {
        if(pLsr->callBack == cb && pLsr->session == session)
        {
            CL_RESULT res;
            res = CL_ListRemove(lsrList, &pLsr->node);
            if(res != CL_SUCCESS)
            {
                return res;
            }
            res = CL_POOL_FREE(&lsrPool, pLsr);
            if(res != CL_SUCCESS)
            {
                return res;
            }
            return CL_SUCCESS;
        }
    }

    return CL_FAILED;
}

CL_RESULT CL_EventSysRaise(CL_Event_t event, int session, void *eventArg)
{
    if (event >= CL_EventMax)
    {
        return CL_INVALID_PARAM;
    }

    CL_List_t* lsrList = &lsrListArray[event];
    Listener_t* pLsr;
    CL_LIST_FOR_EACH_ENTRY(lsrList, pLsr, Listener_t, node)
    {
        if(pLsr->session == session)
        {
            pLsr->callBack(eventArg);
        }
    }

    return CL_SUCCESS;
}

