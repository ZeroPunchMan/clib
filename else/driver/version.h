#pragma once

#include "cl_common.h"

//版本号结构体
typedef struct
{
    uint8_t head[4];  //版本号头
    uint8_t major; //主版本号
    uint8_t minor; //次版本号
    uint16_t revision;  //修订版本号
} Version_t;

extern const Version_t version;


