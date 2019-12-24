#pragma once

#include "cl_common.h"

typedef enum
{
    UserSta_Leave = 0,
    UserSta_OnBed = 2,
    UserSta_LightSleep = 3,
    UserSta_DeepSleep = 4,
} UserStatus_t;

#define US_RESTLESS_THRE    (15)

extern UserStatus_t userStatus;
static inline UserStatus_t GetUserStatus(void)
{
    return userStatus;
}

int UserStatus_AddSecStatus(uint8_t heart, uint8_t breath, uint8_t move);

