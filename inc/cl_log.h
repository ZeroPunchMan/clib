#pragma once

#include "cl_common.h"
#include "clib_config.h"

#if defined(CL_LOG_LEVEL_INFO)
#define CL_LOG_INFO(format, ...)          \
    {                                     \
        CL_PRINTF("info: ")               \
        CL_PRINTF(format, ##__VA_ARGS__); \
        CL_PRINTF("\r\n")                 \
    }
#else
#define CL_LOG_INFO(format, ...)
#endif

#if defined(CL_LOG_LEVEL_WARN)
#define CL_LOG_WARN(format, ...)          \
    {                                     \
        CL_PRINTF("warn: ")               \
        CL_PRINTF(format, ##__VA_ARGS__); \
        CL_PRINTF("\r\n")                 \
    }
#else
#define CL_LOG_WARN(format, ...)
#endif

#if defined(CL_LOG_LEVEL_ERROR)
#define CL_LOG_ERROR(format, ...)         \
    {                                     \
        CL_PRINTF("error: ")              \
        CL_PRINTF(format, ##__VA_ARGS__); \
        CL_PRINTF("\r\n")                 \
    }
#else
#define CL_LOG_ERROR(format, ...)
#endif

#define CL_ASSERT(x)                                                          \
    if (!(x))                                                                 \
    {                                                                         \
        CL_LOG_ERROR(ASSERT, "assert failed at %s : %d", __FILE__, __LINE__); \
        while (1)                                                             \
            ;                                                                 \
    }
