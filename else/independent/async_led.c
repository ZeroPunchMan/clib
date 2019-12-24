#include "async_led.h"
#include "gpio.h"
#include "sys_time.h"

//----------struct define----------------------
typedef struct
{
    uint8_t portGroup;
    uint8_t portNum;
} AyncLed_Port_t; //modify when porting


typedef struct
{
    const uint32_t* stepDur;
    uint8_t stepNum;
} AsyncLed_Effect_t;


typedef struct
{
    const AyncLed_Port_t* port;
    const AsyncLed_Effect_t* effects;
    uint8_t effNum;
    uint8_t curEffIdx;
    uint8_t stepIdx;
    uint64_t stepStartTime;
} AsyncLed_Status_t;
//***************end of struct define***************************

//------------effects define-----------------
//battery
static const uint32_t batEff_Normal[] = {0, 5000};  //on off ...
static const uint32_t batEff_Low[] = {500, 3000};

static const AsyncLed_Effect_t batEffects[] =
{
    {batEff_Normal, CL_ARRAY_LENGTH(batEff_Normal)},
    {batEff_Low, CL_ARRAY_LENGTH(batEff_Low)},
};

//ble
static const uint32_t bleEff_Idle[] = {400, 1000};
static const uint32_t bleEff_Conn[] = {1000, 4000, 1000, 4000, 1000, 4000, 1000, 4000};

static const AsyncLed_Effect_t bleEffects[] =
{
    {bleEff_Idle, CL_ARRAY_LENGTH(bleEff_Idle)},
    {bleEff_Conn, CL_ARRAY_LENGTH(bleEff_Conn)},
};

//*****************end of effects*******************************


//------------------driver define------------------------------
//modify when porting
static const AyncLed_Port_t asyncLedPorts[] =
{
    {0, 3},     //battery
    {2, 3},     //ble
};

static inline void LedPortInit(const AyncLed_Port_t* ledPort)
{
    //modify when porting
    Gpio_InitIOExt(ledPort->portGroup, ledPort->portNum, GpioDirOut, FALSE, FALSE, FALSE, FALSE);
}

static inline void LedPortExit(const AyncLed_Port_t* ledPort)
{
    //modify when porting
    Gpio_InitIOExt(ledPort->portGroup, ledPort->portNum, GpioDirIn, FALSE, FALSE, FALSE, FALSE);
}

static inline void LedWork(const AyncLed_Port_t* ledPort)
{
    //modify when porting
    Gpio_SetIO(ledPort->portGroup, ledPort->portNum, 0);
}

static inline void LedStop(const AyncLed_Port_t* ledPort)
{
    //modify when porting
    Gpio_SetIO(ledPort->portGroup, ledPort->portNum, 1);
}
//****************end fo driver*********************

static AsyncLed_Status_t ledStatus[Led_Max] =
{
    {
        .port = &asyncLedPorts[Led_Battery],
        .effects = batEffects,
        .effNum = CL_ARRAY_LENGTH(batEffects),
        .curEffIdx = 0,
        .stepIdx = 0,
        .stepStartTime = 0,
    },
    {
        .port = &asyncLedPorts[Led_Ble],
        .effects = bleEffects,
        .effNum = CL_ARRAY_LENGTH(bleEffects),
        .curEffIdx = 0,
        .stepIdx = 0,
        .stepStartTime = 0,

    },
};

//---------------------do not modify code below----------------------------------------------
void AsyncLed_Init(void)
{
    for(int i = 0; i < Led_Max; i++)
    {
        LedPortInit(ledStatus[i].port);
        LedStop(ledStatus[i].port);
    }
//    AsyncLed_SetEffect(Led_Battery, 1);
//    AsyncLed_SetEffect(Led_Ble, 1);
}

void AsyncLed_Exit(void)
{
    for(int i = 0; i < Led_Max; i++)
    {
        LedPortExit(ledStatus[i].port);
    }
}

void AsyncLed_SetEffect(LedId_t ledId, uint8_t effId)
{
    if(ledId < Led_Max)
    {
        AsyncLed_Status_t* ledSta = &ledStatus[ledId];
        if(effId < ledSta->effNum)
        {
            if(effId == ledSta->curEffIdx)
            {
                return;
            }
            else
            {
                ledSta->stepIdx = 0;
                SetToCurTime(&ledSta->stepStartTime);
                ledSta->curEffIdx = effId;
            }
        }
    }
}

void AsyncLed_Process(void)
{
    for(int8_t i = 0; i < Led_Max; i++)
    {
        AsyncLed_Status_t* ledSta = &ledStatus[i];
        const AsyncLed_Effect_t* curEffect = ledSta->effects + ledSta->curEffIdx;
        if(TimeElapsed(ledSta->stepStartTime) >= curEffect->stepDur[ledSta->stepIdx])
        {
            SetToCurTime(&ledSta->stepStartTime);
            ledSta->stepIdx++;
            if(ledSta->stepIdx >= curEffect->stepNum)
            {
                ledSta->stepIdx = 0;
            }
            if(curEffect->stepDur[ledSta->stepIdx] == 0)
            {
                ledSta->stepIdx++;
                if(ledSta->stepIdx >= curEffect->stepNum)
                {
                    ledSta->stepIdx = 0;
                }
            }
//            Log("led: %d step to %d\r\n", i, ledSta->stepIdx);
        }

        if(ledSta->stepIdx & 0x01)
        {
            LedStop(ledSta->port);
        }
        else
        {
            LedWork(ledSta->port);
        }
    }
}


