#include "stdio.h"
#include "cl_queue.h"
#include "pthread.h"
#include "unistd.h"
#include <sys/select.h>

typedef struct
{
    uint8_t a;
    int16_t b;
    uint32_t c;
} TestStruct;

CL_QUEUE_DEF_INIT(foreach_q, 256, TestStruct, static);
//*************for each test*************************
static CL_RESULT ForeachTest(void)
{
    TestStruct testStruct;
    testStruct.a = 1;
    testStruct.b = 2;
    testStruct.c = 3;

    TestStruct temp;

    //************************
    //add something
    for (int i = 0; i < (CL_QueueCapacity(&foreach_q) * 2 + 100); i++)
    {

        if (CL_QueueFull(&foreach_q))
            CL_QueuePoll(&foreach_q, &temp);

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //flush with regular data
    for (int i = 0; i < CL_QueueCapacity(&foreach_q); i++)
    {
        testStruct.a = 0;
        testStruct.b = i;
        testStruct.c = i;

        if (CL_QueueFull(&foreach_q))
            CL_QueuePoll(&foreach_q, &temp);

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //verify regular data
    TestStruct *pTS = CL_NULL;
    int count = 0;
    CL_QUEUE_FOR_EACH(&foreach_q, pTS, TestStruct)
    {
        if ((pTS->a) != 0 || pTS->b != count || pTS->c != count)
            return CL_FAILED;

        // printf("%d, %d, %d\n", pTS->a, pTS->b, pTS->c);
        count++;
    }

    //***************************
    //add regular data
    CL_QueueClear(&foreach_q);
    for (int i = 0; i < CL_QueueCapacity(&foreach_q) - 10; i++)
    {
        testStruct.a = 0;
        testStruct.b = i;
        testStruct.c = i;

        CL_QueueAdd(&foreach_q, &testStruct);
    }

    //verify regular data
    count = 0;
    CL_QUEUE_FOR_EACH(&foreach_q, pTS, TestStruct)
    {
        if ((pTS->a) != 0 || pTS->b != count || pTS->c != count)
            return CL_FAILED;

        // printf("%d, %d, %d\n", pTS->a, pTS->b, pTS->c);
        count++;
    }

    return CL_SUCCESS;
}

void select_sleep(time_t sec, suseconds_t us)
{
    struct timeval timeout;

    timeout.tv_sec = sec;
    timeout.tv_usec = us;

    select(0, NULL, NULL, NULL, &timeout);
}

static CL_BOOL slowPoll = CL_FALSE;
static CL_RESULT asyncTestResult = CL_SUCCESS;
#define LOOP_MAX_VAL 10
CL_QUEUE_DEF_INIT(async_q, 100, int, static);
static void* PollThread(void *arg)
{
    int lastVal = -1;
    while (1)
    {
        int pollVal;
        if(CL_QueuePoll(&async_q, &pollVal) == CL_SUCCESS)
        {
            int nextVal = (lastVal + 1) % LOOP_MAX_VAL;
            if(pollVal != nextVal)
            {
                asyncTestResult = CL_FAILED;
                pthread_exit(NULL);
                return NULL;
            }
            lastVal = pollVal;

            // if(nextVal == 0)
            //     printf("\n");

            // printf("%d ", pollVal);
            // fflush(stdout);
        }

        if(slowPoll)
            select_sleep(0, 10000);
        else
            select_sleep(0, 1000);
    }
}

static CL_RESULT AsyncTest(void)
{
    pthread_t pollThr;
    pthread_create(&pollThr, NULL, PollThread, NULL);
    const int addTimes = 500000;
    int count = 0;
    while (1)
    {
        for(int i = 0; i < LOOP_MAX_VAL; i++)
        {
            while(CL_QueueAdd(&async_q, &i) != CL_SUCCESS)
            {
                select_sleep(0, 5000);
                if(asyncTestResult != CL_SUCCESS || count > addTimes)
                {
                    pthread_cancel(pollThr);
                    return asyncTestResult;
                }

                if(count > addTimes / 2)
                {
                    slowPoll = CL_TRUE;
                }
            }
            count++;
        }
    }
}

typedef CL_RESULT (*TestFunc)(void);

TestFunc testCases[] = {
    ForeachTest,
    // AsyncTest,
};

int main(int argc, char **argv)
{
    for (int i = 0; i < CL_ARRAY_LENGTH(testCases); i++)
    {
        if (testCases[i]() != CL_SUCCESS)
        {
            printf("queue test failed at %d\n", i);
            return 0;
        }
    }

    printf("queue all test ok\n");
    return 0;    
}
