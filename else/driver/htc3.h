#pragma once

#include "cl_common.h"

typedef void (*HTC3_AsyncMeasureDone)(float* temp, float* humi);

CL_Result_t HTC3_Init(void);
CL_Result_t HTC3_SyncMeasure(uint16_t* temp, uint16_t* humi);
void HTC3_Exit(void);

static inline float HTC3_ValToTemp(uint16_t adc)
{
    float temp;
    temp = adc * 175.0f / 65536.0f - 45.0f;
    return temp;
}

static inline float HTC3_ValToHumi(uint16_t adc)
{
    float humi;
    humi = adc * 100.0f / 65536.0f;
    return humi;
}



