/********************************************
 实现单个LED的显示效果
 ***************************************
 */
#pragma once

#include "cl_common.h"


typedef enum
{
    Led_Battery = 0,
    Led_Ble = 1,
    Led_Max,
} LedId_t;

//led effect
#define BAT_LED_OFF     0
#define BAT_LED_WARN    1

#define BLE_LED_IDLE    0
#define BLE_LED_CONN    1


//LED 初始化
void AsyncLed_Init(void);

//LED 退出
void AsyncLed_Exit(void);

//LED 设置效果
void AsyncLed_SetEffect(LedId_t ledId, uint8_t effId);

//LED 处理函数
void AsyncLed_Process(void);

