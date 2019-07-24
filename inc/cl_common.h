#ifndef _CL_COMMON_H_
#define _CL_COMMON_H_


#include "stdint.h"
#include "string.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CL_CORTEX_M3    (3)
#define CL_CORTEX_M4    (4)
#define CL_X64          (64)

#define CL_CPU_TYPE     CL_CORTEX_M3

typedef enum
{
    CL_FALSE = 0,
    CL_TRUE = 1,
} CL_BOOL;

typedef enum
{
    CL_SUCCESS = 0,

    CL_FAILED = -1,
    CL_INVALID_PARAM = -2,

} CL_RESULT;


#define CL_INLINE           __inline
#define CL_STATIC_INLINE    static CL_INLINE
#define CL_NULL             (0)

#define CL_ARRAY_LENGTH(array)      (sizeof(array) / sizeof(array[0]))

//#define _USE_LOG 

#ifdef _USE_LOG
#define Log(format, ...)    printf(format, ##__VA_ARGS__)
#else
#define Log(format, ...)     
#endif

#ifdef __cplusplus
}
#endif


#endif

