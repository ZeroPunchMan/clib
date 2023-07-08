#include "cl_common.h"

//*********************queue******************************

typedef enum
{
    QMStyle_AddCell,
    QMStyle_AddBulk,
    QMStyle_AddBulkOrLess,
} QueueMtestAddStyle_t;

typedef enum
{
    QMStyle_PollCell,
    QMStyle_PollBulk,
    QMStyle_PollBulkOrLess,
} QueueMtestPollStyle_t;

void CL_MtestQueueInit(uint32_t buffSize,
                       uint32_t marginSize,
                       uint16_t qCellSize,
                       uint16_t lenOfBulk,
                       QueueMtestAddStyle_t addStyle,
                       QueueMtestPollStyle_t pollStyle);
void CL_MtestQueueExit(void);
void CL_MTestQueueAdd(uint16_t delay, bool checkMem);
void CL_MTestQueuePoll(uint16_t delay, bool checkMem);

//----------------------------------------------------
