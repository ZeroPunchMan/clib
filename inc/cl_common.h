#pragma once

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CL_CORTEX_M3    (3)
#define CL_CORTEX_M4    (4)
#define CL_X64          (64)

#define CL_CPU_TYPE     CL_CORTEX_M3

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

#define OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define CONTAINER_OF(member_ptr, type, member) ((type *)((char*)(member_ptr) - OFFSET_OF(type, member)))

#define CL_ASSERT(x) \
if(!(x)) \
{Log("assert failed at %s : %d", __FILE__, __LINE__); while(1);}


#ifdef __cplusplus
}
#endif


