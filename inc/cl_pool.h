#pragma once

#include "cl_common.h"

typedef struct
{
    uint16_t cellSize;
    uint16_t capacity;
    void* memBuff;
    uint16_t* idxBuff;
    uint16_t freeCount;
} CL_Pool_t;

#define CL_POOL_DEFINE(name, capacity, cell_type, modifier) \
static cell_type name##_poolBuf[capacity]; \
static uint16_t name##_idxBuff[capacity]; \
modifier CL_Pool_t name = {(uint16_t)sizeof(cell_type), capacity, (void*)(name##_poolBuf), name##_idxBuff, capacity};

#define CL_POOL_DECL(name) \
extern CL_Pool_t name;


#define CL_POOL_ALLOC(pool, cell_type)  ((cell_type*)CL_PoolAlloc(pool))
#define CL_POOL_FREE(pool, cell_ptr)    (CL_PoolFree(pool, (void*)cell_ptr))

void CL_PoolInit(CL_Pool_t* pool);
void* CL_PoolAlloc(CL_Pool_t* pool);
void CL_PoolFree(CL_Pool_t* pool, void* pCell);


