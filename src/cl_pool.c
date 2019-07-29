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

CL_RESULT CL_PoolFree(CL_Pool_t *pool, void *pCell)
{
    if(pCell < pool->memBuff || pool->freeCount >= pool->capacity)
        return CL_INVALID_PARAM;

    uint32_t addrOffset = pCell - pool->memBuff;
    if((addrOffset % pool->cellSize) != 0)
        return CL_INVALID_PARAM;

    uint16_t cellIdx = addrOffset / pool->cellSize;
    pool->idxBuff[pool->freeCount++] = cellIdx;
    return CL_SUCCESS;
}


#ifdef USE_POOL_CHECK
#include "stdlib.h"

CL_RESULT CL_PoolFullCheck(CL_Pool_t *pool)
{
    if(pool->capacity != pool->freeCount)
        return CL_FALSE;

    uint8_t* flags = malloc(pool->capacity);
    memset(flags, 0, pool->capacity);
    for(int i = 0; i < pool->capacity; i++)
    {
        uint16_t idx = pool->idxBuff[i];
        flags[i]++;
    }

    for(int i = 0; i < pool->capacity; i++)
    {
        if(flags[i] != 1)
        {
            return CL_FAILED;
        }
    }
    return CL_SUCCESS;
}
#endif
