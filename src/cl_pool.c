#include "cl_pool.h"

void CL_PoolInit(CL_Pool_t *pool)
{
    for (uint16_t i = 0; i < pool->capacity; i++)
    {
        pool->idxBuff[i] = i;
    }
}

void *CL_PoolAlloc(CL_Pool_t *pool)
{
    void *result = CL_NULL;
    if (pool->freeCount > 0)
    {
        uint16_t cellIdx;
        pool->freeCount--;
        cellIdx = pool->idxBuff[pool->freeCount];

        result = pool->memBuff + cellIdx * pool->cellSize;
    }

    return result;
}

void CL_PoolFree(CL_Pool_t *pool, void *pCell)
{
    if(pCell < pool->memBuff || pool->freeCount >= pool->capacity)
        return;

    uint32_t addrOffset = pCell - pool->memBuff;
    if((addrOffset % pool->cellSize) != 0)
        return;

    uint16_t cellIdx = addrOffset / pool->cellSize;
    pool->idxBuff[pool->freeCount++] = cellIdx;
}
