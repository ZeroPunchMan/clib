#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CL_ResSuccess = 0,

    CL_ResFailed = -1,
    CL_ResInvalidParam = -2,
    CL_ResBusy = -3,
    CL_ResNotInit = -4,
} CL_Result_t;

#define CL_NULL             ((void*)(0))

#define CL_ARRAY_LENGTH(array)      (sizeof(array) / sizeof(array[0]))

#define CL_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define CL_CONTAINER_OF(member_ptr, type, member) ((type *)((char*)(member_ptr) - CL_OFFSET_OF(type, member)))

#ifdef __cplusplus
}
#endif


