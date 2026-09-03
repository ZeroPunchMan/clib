#pragma once

#include "cl_common.h"
#include "clib_config.h"

#if defined(CL_LOG_LEVEL_INFO)
#define CL_LOG_INFO(format, ...)          \
    {                                     \
        CL_PRINTF(format "\n", ##__VA_ARGS__); \
    }
#else
#define CL_LOG_INFO(format, ...)
#endif

#if defined(CL_LOG_LEVEL_WARN)
#define CL_LOG_WARN(format, ...)          \
    {                                     \
        CL_PRINTF(format "\n", ##__VA_ARGS__); \
    }
#else
#define CL_LOG_WARN(format, ...)
#endif

#if defined(CL_LOG_LEVEL_ERROR)
#define CL_LOG_ERROR(format, ...)         \
    {                                     \
        CL_PRINTF(format "\n", ##__VA_ARGS__); \
    }
#else
#define CL_LOG_ERROR(format, ...)
#endif

#define CL_ASSERT(x)                                                          \
    if (!(x))                                                                 \
    {                                                                         \
        CL_LOG_ERROR("assert failed at %s : %d", __FILE__, __LINE__); \
        while (1)                                                             \
            ;                                                                 \
    }
