#include "test_common.h"
#include "cl_list.h"
#include "cl_pool.h"

static CL_List_t testList;

typedef struct
{
    CL_ListNode_t node;
    int a, b;
} TestStruct_t;

#define TEST_POOL_SIZE (200)
CL_POOL_DEFINE(testPool, TEST_POOL_SIZE, TestStruct_t, static);
CL_RESULT ListTest(void)
{
    CL_PoolInit(&testPool);
    CL_ListInit(&testList);

    TestStruct_t *ts;
    CL_ListNode_t *pNode;
    TestStruct_t *pCtn;

    //add 0~200
    for (int i = 100; i < TEST_POOL_SIZE; i++)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = i;
        ts->b = i;
        CL_ListAddLast(&testList, &ts->node);
    }

    for (int i = 99; i >= 0; i--)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = i;
        CL_ListAddFirst(&testList, &ts->node);
    }

    int count = 0;
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            // return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
        count++;
    }

    //remove even number

    CL_LIST_FOR_EACH_SAFE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if (pCtn->a % 2 == 0)
        {
            CL_ListRemove(&testList, pNode);
            if (CL_POOL_FREE(&testPool, pCtn) != CL_SUCCESS)
            {
                return CL_FAILED;
            }
            // printf("ctn: %d, node: %d\n", pCtn, pNode);
        }
    }

    count = 1;
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
        count += 2;
    }

    CL_LIST_FOR_EACH_REVERSE_SAFE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = pCtn->a - 1;
        CL_ListAddBefore(pNode, &ts->node);
    }

    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_REVERSE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
        count--;
    }
    if(count != -1)
    {
        return CL_FAILED;
    }

    //remove odd number
    CL_LIST_FOR_EACH_ENTRY_REVERSE_SAFE(&testList, pCtn, TestStruct_t, node)
    {
        if (pCtn->a % 2 != 0)
        {
            CL_ListRemove(&testList, &pCtn->node);
            CL_POOL_FREE(&testPool, pCtn);
        }
    }

    count = 0;
    CL_LIST_FOR_EACH_ENTRY(&testList, pCtn, TestStruct_t, node)
    {
        if (pCtn->a != count || pCtn->b != count)
        {
            return CL_FAILED;
        }
        count += 2;
        // printf("%d\n", pCtn->a);
    }

    CL_LIST_FOR_EACH_ENTRY_SAFE(&testList, pCtn, TestStruct_t, node)
    {
        ts = CL_POOL_ALLOC(&testPool, TestStruct_t);
        ts->a = ts->b = pCtn->a + 1;
        CL_ListAddAfter(&pCtn->node, &ts->node);
    }

    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_ENTRY_REVERSE(&testList, pCtn, TestStruct_t, node)
    {
        if(pCtn->a != count || pCtn->b != count)
        {
            return CL_FAILED;
        }
        count--;
        // printf("%d\n", pCtn->a);
    }
    if(count != -1)
    {
        return CL_FAILED;
    }

    CL_LIST_FOR_EACH_REVERSE_SAFE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if(CL_POOL_FREE(&testPool, pCtn) != CL_SUCCESS)
        {
            return CL_FAILED;
        }
    }

    return CL_PoolFullCheck(&testPool);
}

TestCase_t testCases[] = {
    {ListTest, "add last"},
};

int main(int argc, char **argv)
{
    TEST_CASE_PROC(testCases, "list");
    return 0;
}
