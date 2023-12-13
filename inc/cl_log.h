#pragma once

#include "cl_common.h"
#include "clib_config.h"

#define CL_LOG_MODULE_CHECK(module) (CL_LOG_##module##_ENABLED)

#if defined(CL_LOG_LEVEL_INFO)
#define CL_LOG_INFO(module, format, ...)      \
    {                                         \
        if (CL_LOG_MODULE_CHECK(module))      \
        {                                     \
            CL_PRINTF(format, ##__VA_ARGS__); \
        }                                     \
    }
#else
#define CL_LOG_INFO(module, format, ...)
#endif

#if defined(CL_LOG_LEVEL_WARN)
#define CL_LOG_WARN(module, format, ...)      \
    {                                         \
        if (CL_LOG_MODULE_CHECK(module))      \
        {                                     \
            CL_PRINTF(format, ##__VA_ARGS__); \
        }                                     \
    }
#else
#define CL_LOG_WARN(module, format, ...)
#endif

#if defined(CL_LOG_LEVEL_ERROR)
#define CL_LOG_ERROR(module, format, ...)     \
    {                                         \
        if (CL_LOG_MODULE_CHECK(module))      \
        {                                     \
            CL_PRINTF(format, ##__VA_ARGS__); \
        }                                     \
    }
#else
#define CL_LOG_ERROR(module, format, ...)
#endif

#define CL_ASSERT(x)                                                          \
    if (!(x))                                                                 \
    {                                                                         \
        CL_LOG_ERROR(ASSERT, "assert failed at %s : %d", __FILE__, __LINE__); \
        while (1)                                                             \
            ;                                                                 \
    }
