#include "test_common.h"
#include "cl_pool.h"
#include "stdlib.h"
#include "time.h"

typedef struct
{
    uint8_t a;
    int16_t b;
    int32_t c;
    int32_t d;
} TestStruct;

static CL_RESULT PrtToIdx(CL_Pool_t *pool, void *ptr, uint16_t *pIdx)
{
    if (ptr < pool->memBuff)
        return CL_INVALID_PARAM;

    uint32_t addrOffset = ptr - pool->memBuff;
    if ((addrOffset % pool->cellSize) != 0)
        return CL_INVALID_PARAM;

    *pIdx = addrOffset / pool->cellSize;
    if (*pIdx >= pool->capacity)
        return CL_INVALID_PARAM;

    return CL_SUCCESS;
}

#define RAND_POOL_SIZE (512)
CL_POOL_DEFINE(randPool, RAND_POOL_SIZE, uint64_t, static);
CL_RESULT RandomTest(void)
{
    static uint64_t *cells[RAND_POOL_SIZE];
    memset(cells, 0, sizeof(cells));
    srand(time(NULL));

    CL_PoolInit(&randPool);

     for (int i = 0; i < RAND_POOL_SIZE; i++)
    {
        cells[i] = CL_POOL_ALLOC(&randPool, uint64_t);
    }

    for (int i = 0; i < RAND_POOL_SIZE * 50; i++)
    {
        int r = rand() % RAND_POOL_SIZE;
        if (cells[r] == NULL)
        {
            cells[r] = CL_POOL_ALLOC(&randPool, uint64_t);
        }
        else
        {
            CL_POOL_FREE(&randPool, cells[r]);
            cells[r] = NULL;
        }
    }

    // printf("free: %d\n", randPool.freeCount);

    //array to mark cells
    static uint8_t cellsFlag[RAND_POOL_SIZE];
    memset(cellsFlag, 0, sizeof(cellsFlag));
    //traverse used cells
    for (int i = 0; i < RAND_POOL_SIZE; i++)
    {
        if (cells[i] != NULL)
        {
            uint16_t idx;
            if (PrtToIdx(&randPool, cells[i], &idx) != CL_SUCCESS)
            {
                return CL_FAILED;
            }

            cellsFlag[idx]++;
        }
    }

    //traverse unused cells
    for (int i = 0; i < randPool.freeCount; i++)
    {
        uint16_t idx = randPool.idxBuff[i];
        if (idx >= randPool.capacity)
        {
            return CL_FAILED;
        }
        // printf("unused %d\n", idx);
        cellsFlag[idx]++;
    }

    for(int i = 0; i< RAND_POOL_SIZE; i++)
    {
        if(cellsFlag[i] != 1)
        {
            // printf("idx %d is %d\n", i, cellsFlag[i]);
            return CL_FAILED;
        }
    }

    return CL_SUCCESS;
}

//
#define EF_POOL_SIZE (100)
CL_POOL_DEFINE(efPool, EF_POOL_SIZE, TestStruct, static);
CL_RESULT EmptyFullTest(void)
{
    static TestStruct *cells[EF_POOL_SIZE];
    CL_PoolInit(&efPool);

    //empty
    for (int i = 0; i < EF_POOL_SIZE; i++)
    {
        cells[i] = CL_POOL_ALLOC(&efPool, TestStruct);
    }
    if (efPool.freeCount != 0)
    {
        return CL_FAILED;
    }
    // printf("free: %d\n", efPool.freeCount);

    //full
    for (int i = 0; i < EF_POOL_SIZE; i++)
    {
        CL_POOL_FREE(&efPool, cells[i]);
    }
    if (efPool.freeCount != EF_POOL_SIZE)
    {
        return CL_FAILED;
    }
    // printf("free: %d\n", efPool.freeCount);

    return CL_SUCCESS;
}

TestCase_t testCases[] = {
    {EmptyFullTest, "empty full"},
    {RandomTest, "random"}
};

int main(int argc, char **argv)
{
    TEST_CASE_PROC(testCases, "pool");
    return 0;
}
