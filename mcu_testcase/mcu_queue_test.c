#include "cl_queue.h"
#include "stdlib.h"
#include "string.h"

void QueueTest_Add(bool delay);
void QueueTest_Poll(bool delay);
void QueueTest_MultiAdd(bool delay);
void QueueTest_MultiPoll(bool delay);

void QueueTest_Delay(uint16_t ms);
void CL_TestErrorHandler(void);

#define TEST_BUFF_LEN (100)
#define TEST_BUFF_MARGIN (64)

typedef struct
{
    int a;
    char b;
} TestUnit_t;

// CL_QUEUE_DEF_INIT(testQueue, 1024, TestUnit_t, static);
static uint8_t testQueueBuff[sizeof(TestUnit_t) * (TEST_BUFF_LEN + 1) + TEST_BUFF_MARGIN * 2] = {0}; // 预留多TEST_BUFF_MARGIN*2字节
// 前后留TEST_BUFF_MARGIN字节检测指针越界
static CL_QueueInfo_t testQueue = {
    .data = &testQueueBuff[TEST_BUFF_MARGIN], .head = 0, .tail = 0, .capacity = TEST_BUFF_LEN, .data_size = sizeof(TestUnit_t)};

static volatile bool inAdd = false;
static volatile bool inPoll = false;
static volatile uint32_t addCount = 0;
static volatile uint32_t pollCount = 0;

void MemoryCheck(void)
{
    uint8_t *p = testQueueBuff;
    for (int i = 0; i < TEST_BUFF_MARGIN; i++)
    {
        if (p[i] != 0)
            CL_TestErrorHandler();
    }

    p = &testQueueBuff[sizeof(testQueueBuff)] - TEST_BUFF_MARGIN;
    for (int i = 0; i < TEST_BUFF_MARGIN; i++)
    {
        if (p[i] != 0)
            CL_TestErrorHandler();
    }
}

void QueueTest_Add(bool delay)
{
    static TestUnit_t add = {
        .a = 0,
        .b = 0,
    };

    int r = rand() % 10;

    inAdd = true;
    CL_Result_t res = CL_QueueAdd(&testQueue, &add);
    inAdd = false;

    if (res == CL_ResSuccess)
    {
        add.a++;
        add.b++;
        addCount++;
    }

    if (delay)
        QueueTest_Delay(r);
}

void QueueTest_Poll(bool delay)
{
    static TestUnit_t exp = {
        .a = 0,
        .b = 0,
    };

    int r = rand() % 10;
    TestUnit_t poll;

    inPoll = true;
    CL_Result_t res = CL_QueuePoll(&testQueue, &poll);
    inPoll = false;

    if (res == CL_ResSuccess)
    {
        if (exp.a == poll.a && exp.b == poll.b)
        {
            exp.a++;
            exp.b++;

            pollCount++;
        }
        else
        {
            CL_TestErrorHandler();
        }
    }

    if (delay)
        QueueTest_Delay(r);

    MemoryCheck();
}

void QueueTest_MultiAdd(bool delay)
{
    static int addValue = 0;
    TestUnit_t input[TEST_BUFF_LEN];
    uint16_t r = (uint16_t)rand() % TEST_BUFF_LEN;

    if (CL_QueueFreeSpace(&testQueue) >= r)
    {
        for (uint16_t i = 0; i < r; i++)
        {
            input[i].a = addValue;
            input[i].b = addValue;

            addValue++;
        }

        inAdd = true;
        if (CL_QueueMultiAdd(&testQueue, input, r) != CL_ResSuccess)
            CL_TestErrorHandler();
        inAdd = false;

        addCount += r;
    }

    if (delay)
        QueueTest_Delay(r);

    MemoryCheck();
}

void QueueTest_MultiPoll(bool delay)
{
    static TestUnit_t exp = {
        .a = 0,
        .b = 0,
    };
    TestUnit_t output[TEST_BUFF_LEN];
    uint16_t qLen = CL_QueueLength(&testQueue) / 2;
    
    inPoll = true;
    if (CL_QueueMultiPoll(&testQueue, &output, qLen) != CL_ResSuccess)
        CL_TestErrorHandler();
    inPoll = false;

    for (uint16_t i = 0; i < qLen; i++)
    {
        if (exp.a == output[i].a && exp.b == output[i].b)
        {
            exp.a++;
            exp.b++;

            pollCount++;
        }
        else
        {
            CL_TestErrorHandler();
        }
    }

    uint16_t r = (uint16_t)rand() % TEST_BUFF_LEN;
    if (delay)
        QueueTest_Delay(r);

    MemoryCheck();
}
