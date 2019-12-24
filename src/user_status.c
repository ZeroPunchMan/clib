#include "user_status.h"
#include "cl_event_system.h"
#include "string.h"
#include "cl_queue.h"

#define STA_BUFF_LEN 20

typedef enum
{
    MS_NoSignal = 0,
    MS_Restless = 1,
    MS_Calm = 2,
} minStatus_t;
CL_QUEUE_DEF_INIT(status_q, STA_BUFF_LEN, uint8_t, static);
static int8_t minStaCount[3];

UserStatus_t userStatus = UserSta_Leave;

static void ToLeave(void)
{
    memset(minStaCount, 0, sizeof(minStaCount));
    CL_QueueClear(&status_q);   //切换状态,清空队列
    userStatus = UserSta_Leave; //记录状态
}

static void ToOnBed(void)
{
    memset(minStaCount, 0, sizeof(minStaCount));
    CL_QueueClear(&status_q);
    userStatus = UserSta_OnBed;
}

static void ToLightSleep(void)
{
    memset(minStaCount, 0, sizeof(minStaCount));
    CL_QueueClear(&status_q);
    userStatus = UserSta_LightSleep;
}

static void ToDeepSleep(void)
{
    memset(minStaCount, 0, sizeof(minStaCount));
    CL_QueueClear(&status_q);
    userStatus = UserSta_DeepSleep;
}

static bool NoSigProc(minStatus_t minSta)
{
    static uint8_t count = 0;
    if (minSta == MS_NoSignal)
    {
        if (++count > 3)
        {
            count = 0;
            ToLeave();
            return true;
        }
    }
    else
    {
        count = 0;
    }

    return false;
}

static void LeaveProc(minStatus_t minSta)
{
    if (minSta != MS_NoSignal)
    {
        ToOnBed();
    }
}


static void AddMinSta(minStatus_t minSta, int maxLen)
{
    uint8_t sta = minSta;
    uint8_t oldVal;

    if (CL_QueueLength(&status_q) >= maxLen)
    {
        CL_QueuePoll(&status_q, &oldVal);
        minStaCount[oldVal]--;
    }

    CL_QueueAdd(&status_q, &sta);
    minStaCount[sta]++;
}

static void OnBedProc(minStatus_t minSta)
{

    AddMinSta(minSta, 15);
    if (minStaCount[MS_Calm] >= 14)
    {
        ToLightSleep();
    }
}

static void LightSleepProc(minStatus_t minSta)
{
    AddMinSta(minSta, 15);
    if (minStaCount[MS_Calm] >= 15)
    {
        ToDeepSleep();
    }
    else if (minStaCount[MS_Restless] > 10)
    {
        ToOnBed();
    }
}

static void DeepSleepProc(minStatus_t minSta)
{
    if (minSta != MS_Calm)
    {
        ToLightSleep();
    }
}

static void CalcMinuteStatus(minStatus_t minSta)
{
    if (NoSigProc(minSta))
        return;

    switch (userStatus)
    {
    case UserSta_Leave:
        LeaveProc(minSta);
        break;
    case UserSta_OnBed:
        OnBedProc(minSta);
        break;
    case UserSta_LightSleep:
        LightSleepProc(minSta);
        break;
    case UserSta_DeepSleep:
        DeepSleepProc(minSta);
        break;
    }
}

int UserStatus_AddSecStatus(uint8_t heart, uint8_t breath, uint8_t move)
{
    static uint8_t hasSignalCount = 0;
    static uint8_t moveCount = 0;
    static uint8_t secondCount = 0;

    if (heart != 0 || breath != 0)
    {
        hasSignalCount++;
    }

    if (move != 0)
    {
        moveCount++;
    }

    if (++secondCount >= 60)
    {
        secondCount = 0;

        minStatus_t minSta;
        if (hasSignalCount > 0 || moveCount > 0)
        {
            if (moveCount > US_RESTLESS_THRE)
                minSta = MS_Restless;
            else
                minSta = MS_Calm;
        }
        else
        {
            minSta = MS_NoSignal;
        }

        moveCount = 0;
        hasSignalCount = 0;

        CalcMinuteStatus(minSta);
        return minSta;
    }
    return -1;
}
