#include "cl_queue.h"
#include "stdlib.h"
#include "string.h"
#include "clib_mtest_ext_api.h"
#include "clib_mtest.h"
#include "cl_log.h"

#define MARGIN_VALUE (0x5a)

typedef struct
{
    CL_QueueInfo_t queue;
    volatile uint8_t *qBuff;
    uint32_t buffSize;
    uint32_t marginSize;
    uint16_t lenOfBulk;
    QueueMtestAddStyle_t addStyle;
    QueueMtestPollStyle_t pollStyle;
    volatile bool initialized;

    volatile uint32_t pollCount;
    volatile uint32_t qLength;
} QueueMtestContext_t;

static QueueMtestContext_t context = {.initialized = false, .pollCount = 0};

// 检查边界内存是否为初始值
static void MemoryCheck(void)
{
    for (uint32_t i = 0; i < context.marginSize; i++)
    {
        if (context.qBuff[i] != MARGIN_VALUE)
        {
            CL_TestErrorHandler("margin check failed");
            return;
        }
    }

    uint32_t tailMarginOffset = context.marginSize + context.queue.data_size * (context.queue.capacity + 1);
    for (uint32_t i = tailMarginOffset; i < context.buffSize; i++)
    {
        if (context.qBuff[i] != MARGIN_VALUE)
        {
            CL_TestErrorHandler("margin check failed");
            return;
        }
    }
}

void CL_MtestQueueInit(uint32_t buffSize,
                       uint32_t marginSize,
                       uint16_t qCellSize,
                       uint16_t lenOfBulk,
                       QueueMtestAddStyle_t addStyle,
                       QueueMtestPollStyle_t pollStyle)
{
    // 分配测试RAM
    context.qBuff = (volatile uint8_t *)malloc(buffSize);
    if (!context.qBuff)
    {
        CL_TestErrorHandler("mcu queue test init failed");
        return;
    }

    memset((void *)context.qBuff, MARGIN_VALUE, buffSize);

    // 在buff上初始化队列,并保留margin
    context.queue.buff = context.qBuff + marginSize;
    context.queue.head = 0;
    context.queue.tail = 0;
    context.queue.data_size = qCellSize;
    context.queue.capacity = ((buffSize - (marginSize * 2)) / qCellSize) - 1; // 前后保留margin
    context.marginSize = marginSize;

    context.lenOfBulk = lenOfBulk; // 块操作长度
    context.addStyle = addStyle;
    context.pollStyle = pollStyle;

    context.initialized = true;
}

void CL_MtestQueueExit(void)
{
    context.initialized = false;

    free((void *)context.qBuff);
}

static void BulkAdd(uint8_t *addBuff, uint8_t *pAddValue);
static void BulkOrLessAdd(uint8_t *addBuff, uint8_t *pAddValue);
void CL_MTestQueueAdd(uint16_t delay)
{
    static uint8_t *addBuff = NULL;
    static uint8_t addValue = 0;

    if (addBuff == NULL)
    {
        addBuff = (uint8_t *)malloc(context.lenOfBulk * context.queue.data_size);
        if (!addBuff)
        {
            CL_TestErrorHandler("add malloc failed");
            return;
        }
    }

    if (context.addStyle == QMStyle_AddCell)
    { // add one cell
        for (uint16_t i = 0; i < context.queue.data_size; i++)
        {
            addBuff[i] = addValue;
        }
        if (CL_QueueAdd(&context.queue, addBuff) == CL_ResSuccess)
        {
            addValue++;
        }
    }
    else if (context.addStyle == QMStyle_AddBulk)
    {
        BulkAdd(addBuff, &addValue);
    }
    else if (context.addStyle == QMStyle_AddBulkOrLess)
    {
        BulkOrLessAdd(addBuff, &addValue);
    }

    if (delay)
    {
        MemoryCheck();
        CL_Test_Delay(delay);
        MemoryCheck();
    }
}

static void BulkAdd(uint8_t *addBuff, uint8_t *pAddValue)
{// add bulk if queue has enough space
    if (CL_QueueFreeSpace(&context.queue) >= context.lenOfBulk)
    {
        for (uint16_t i = 0; i < context.lenOfBulk; i++)
        {
            memset(addBuff + context.queue.data_size * i, pAddValue[0], context.queue.data_size);
            pAddValue[0]++;
        }
        if (CL_QueueMultiAdd(&context.queue, addBuff, context.lenOfBulk) != CL_ResSuccess)
        {
            CL_TestErrorHandler("queue bulk add failed");
            return;
        }
    }
}

static void BulkOrLessAdd(uint8_t *addBuff, uint8_t *pAddValue)
{
    uint16_t addLen = CL_QueueFreeSpace(&context.queue);
    if (addLen > context.lenOfBulk)
        addLen = context.lenOfBulk;

    for (uint16_t i = 0; i < addLen; i++)
    {
        memset(addBuff + context.queue.data_size * i, pAddValue[0], context.queue.data_size);
        pAddValue[0]++;
    }
    if (CL_QueueMultiAdd(&context.queue, addBuff, addLen) != CL_ResSuccess)
    {
        CL_TestErrorHandler("queue bulk add failed");
        return;
    }
}

static void BulkPoll(uint8_t *pollBuff, uint8_t *pPollValue);
static void BulkOrLessPoll(uint8_t *pollBuff, uint8_t *pPollValue);
void CL_MTestQueuePoll(uint16_t delay)
{
    static uint8_t *pollBuff = NULL;
    static uint8_t pollValue = 0;

    if (pollBuff == NULL)
    {
        pollBuff = (uint8_t *)malloc(context.lenOfBulk * context.queue.data_size);
        if (!pollBuff)
        {
            CL_TestErrorHandler("poll malloc error");
            return;
        }
    }
    if (context.pollStyle == QMStyle_PollCell)
    {
        if (CL_QueuePoll(&context.queue, pollBuff) == CL_ResSuccess)
        {
            for (uint16_t i = 0; i < context.queue.data_size; i++)
            { // check one cell
                if (pollBuff[i] != pollValue)
                {
                    CL_TestErrorHandler("poll value error");
                    return;
                }
            }
            pollValue++;
            context.pollCount++;
            context.qLength = CL_QueueLength(&context.queue);
        }
    }
    else if (context.pollStyle == QMStyle_PollBulk)
    {
        BulkPoll(pollBuff, &pollValue);
    }
    else if (context.pollStyle == QMStyle_PollBulkOrLess)
    {
        BulkOrLessPoll(pollBuff, &pollValue);
    }

    if (delay)
    {
        MemoryCheck();
        CL_Test_Delay(delay);
        MemoryCheck();
    }
}

static void BulkPoll(uint8_t *pollBuff, uint8_t *pPollValue)
{
    if (CL_QueueLength(&context.queue) >= context.lenOfBulk)
    {
        if (CL_QueueMultiPoll(&context.queue, pollBuff, context.lenOfBulk) != CL_ResSuccess)
        {
            CL_TestErrorHandler("bulk poll failed");
            return;
        }
        else
        { // check bulk
            context.qLength = CL_QueueLength(&context.queue);
            for (uint16_t i = 0; i < context.lenOfBulk; i++)
            {
                for (uint16_t k = 0; k < context.queue.data_size; k++)
                {
                    if (pollBuff[context.queue.data_size * i + k] != pPollValue[0])
                    {
                        CL_TestErrorHandler("bulk poll value error");
                    }
                }
                pPollValue[0]++;
                context.pollCount++;
            }
        }
    }
}

static void BulkOrLessPoll(uint8_t *pollBuff, uint8_t *pPollValue)
{
    uint16_t pollLen = CL_QueueLength(&context.queue);
    if (pollLen > context.lenOfBulk)
        pollLen = context.lenOfBulk;

    if (CL_QueueMultiPoll(&context.queue, pollBuff, pollLen) != CL_ResSuccess)
    {
        CL_TestErrorHandler("bulk poll failed");
        return;
    }
    else
    { // check bulk
        context.qLength = CL_QueueLength(&context.queue);
        for (uint16_t i = 0; i < pollLen; i++)
        {
            for (uint16_t k = 0; k < context.queue.data_size; k++)
            {
                if (pollBuff[context.queue.data_size * i + k] != pPollValue[0])
                {
                    CL_TestErrorHandler("bulk poll value error");
                }
            }
            pPollValue[0]++;
            context.pollCount++;
        }
    }
}
