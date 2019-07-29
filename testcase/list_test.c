#include "test_common.h"
#include "cl_list.h"
#include "cl_pool.h"

static CL_List_t testList;

typedef struct
{
    CL_ListNode_t node;
    int a;
} TestStruct_t;

#define TEST_POOL_SIZE  (200)
CL_POOL_DEFINE(testPool, TEST_POOL_SIZE, TestStruct_t, static);

CL_RESULT SequenceTest(void)
{
    CL_PoolInit(&testPool);
    CL_ListInit(&testList);

    for(int i = 0; i < TEST_POOL_SIZE; i++)
    {
        TestStruct_t* addCtn = CL_POOL_ALLOC(&testPool, TestStruct_t);
        addCtn->a = i;
        CL_ListAddLast(&testList, &addCtn->node);
    }

    int count = 0;
    CL_ListNode_t *pNode;
    TestStruct_t* pCtn;
    //check each node
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if(pCtn->a != count++)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //check each node reverse
    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_REVERSE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if(pCtn->a != count--)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //check each entry
    count = 0;
    CL_LIST_FOR_EACH_ENTRY(&testList, pCtn, TestStruct_t, node)
    {
        if(pCtn->a != count++)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //check each entry reverse
    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_ENTRY_REVERSE(&testList, pCtn, TestStruct_t, node)
    {
        if(pCtn->a != count--)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //remove all
    CL_LIST_FOR_EACH_REVERSE_SAFE(&testList, pNode)
    {
        CL_ListRemove(&testList, pNode);
        if(CL_POOL_FREE(&testPool, container_of(pNode, TestStruct_t, node)) != CL_SUCCESS)
        {
            return CL_FAILED;
        }
    }

    pNode = &testList.head;
    if(pNode != pNode->nextNode || pNode != pNode->preNode)
    {
        return CL_FAILED;
    }

    if(CL_ListSize(&testList) != 0 || testPool.freeCount != TEST_POOL_SIZE)
    {
        return CL_FAILED;
    }

    CL_LIST_FOR_EACH(&testList, pNode)
    {
        return CL_FAILED;
    }

    //add again
    for(int i = 0; i < TEST_POOL_SIZE; i++)
    {
        TestStruct_t* addCtn = CL_POOL_ALLOC(&testPool, TestStruct_t);
        addCtn->a = i;
        CL_ListAddLast(&testList, &addCtn->node);
    }

    count = 0;
    CL_LIST_FOR_EACH(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if(pCtn->a != count++)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    count = 0;
    CL_LIST_FOR_EACH_ENTRY(&testList, pCtn, TestStruct_t, node)
    {
        if(pCtn->a != count++)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //check each node reverse
    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_REVERSE(&testList, pNode)
    {
        pCtn = container_of(pNode, TestStruct_t, node);
        if(pCtn->a != count--)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    //check each entry reverse
    count = TEST_POOL_SIZE - 1;
    CL_LIST_FOR_EACH_ENTRY_REVERSE(&testList, pCtn, TestStruct_t, node)
    {
        if(pCtn->a != count--)
        {
            return CL_FAILED;
        }
        // printf("%d\n", pCtn->a);
    }

    return CL_SUCCESS;
}

TestCase_t testCases[] = {
    {SequenceTest, "sequence"},
};

int main(int argc, char** argv)
{
    TEST_CASE_PROC(testCases, "list");
    return 0;
}

