#pragma once

#include "cl_common.h"
#include "sgp_type.h"

//data max length
#define MAX_MSG_DATA_LEN    256
typedef struct
{
    uint8_t msgType;
    uint8_t length;
    uint8_t data[256];
} SGP_RecvMsg_t;  //receive massage

//timeout of one frame, ms
#define SGP_FRAME_TIMEOUT       (500)

typedef CL_Result_t (*SGP_SendFunc)(const uint8_t* buff, uint16_t count);

//init, need send function
void SGP_Init(SGP_SendFunc sendFunc);
void SGP_Exit(void);

void SGP_Process(void);

//send sgp massage
CL_Result_t SGP_SendMsg(DevMsgType_t msgType, const uint8_t* data, uint8_t length);



