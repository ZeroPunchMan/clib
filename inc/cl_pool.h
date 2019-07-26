#pragma once

#include "cl_common.h"

typedef struct
{
    uint16_t cellSize;
    uint16_t capacity;
    uint32_t poolAddr;
} CL_Pool_t;

#define CL_POOL_DEFINE(name, capacity, cell_type, modifier) \
static cell_type name##_poolBuf[capacity]; \
modifier CL_Pool_t name = { \
    .                     sizeof(cell_type), \
    capacity,   \
    (void*)name##_poolBuf   \
};

#define CL_POOL_DECL(name) \
extern CL_Pool_t name;

void* CL_PoolAlloc(CL_Pool_t* pool);
CL_RESULT CL_PoolFree(CL_Pool_t* pool, void* pCell);


