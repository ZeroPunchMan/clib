#include "cl_queue.h"
#include "string.h"

#define DATA_CPY(DST, SRC, SIZE) memcpy(DST, SRC, SIZE)

static inline uint16_t NextPos(uint16_t ptr, uint16_t max)
{
    ptr = (ptr + 1) % (max + 1);
    return ptr;
}

CL_Result_t CL_QueueAdd(CL_QueueInfo_t *q, void *data)
{
    if (CL_QueueFull(q))
        return CL_ResFailed;

    DATA_CPY((char *)q->data + (q->tail * q->data_size), data, q->data_size);
    q->tail = NextPos(q->tail, q->capacity);

    return CL_ResSuccess;
}

CL_Result_t CL_QueuePoll(CL_QueueInfo_t *q, void *data)
{
    if (CL_QueueEmpty(q))
        return CL_ResFailed;
    DATA_CPY(data, (char *)q->data + (q->head * q->data_size), q->data_size);
    q->head = NextPos(q->head, q->capacity);

    return CL_ResSuccess;
}

CL_Result_t CL_QueuePeek(CL_QueueInfo_t *q, void *data)
{
    if (CL_QueueEmpty(q))
        return CL_ResFailed;

    DATA_CPY(data, (char *)q->data + (q->head * q->data_size), q->data_size);

    return CL_ResSuccess;
}

bool CL_QueueEmpty(CL_QueueInfo_t *q)
{
    if (q->head == q->tail)
        return true;

    return false;
}

bool CL_QueueFull(CL_QueueInfo_t *q)
{
    uint16_t nextTail = NextPos(q->tail, q->capacity);
    if (nextTail == q->head)
        return true;

    return false;
}

uint16_t CL_QueueLength(CL_QueueInfo_t *q)
{
    uint16_t head, tail;
    head = q->head;
    tail = q->tail;

    if (tail >= head)
    {
        return tail - head;
    }
    else
    {
        return q->capacity - head + 1 + tail;
    }
}

uint16_t CL_QueueFreeSpace(CL_QueueInfo_t *q)
{
    return q->capacity - CL_QueueLength(q);
}
