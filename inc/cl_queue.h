#pragma once

#include "cl_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        volatile void *data;
        volatile uint16_t head;
        volatile uint16_t tail;

        uint16_t capacity;
        uint16_t data_size;
    } CL_QueueInfo_t;

#define CL_QUEUE_DEF_INIT(q_name, capacity, data_type, modifier) \
    static data_type q_name##_buf[capacity + 1];                 \
    modifier CL_QueueInfo_t q_name = {q_name##_buf, 0, 0, capacity, sizeof(data_type)};

#define CL_QUEUE_DECL(q_name) \
    extern CL_QueueInfo_t q_name;

    CL_Result_t CL_QueueAdd(CL_QueueInfo_t *q, void *data);

    CL_Result_t CL_QueueMultiAdd(CL_QueueInfo_t *q, void *data, uint16_t len);

    CL_Result_t CL_QueuePoll(CL_QueueInfo_t *q, void *data);

    CL_Result_t CL_QueueMultiPoll(CL_QueueInfo_t *q, void *data, uint16_t len);

    CL_Result_t CL_QueuePeek(CL_QueueInfo_t *q, uint16_t index, void **pptr);

    static inline void CL_QueueClear(CL_QueueInfo_t *q)
    {
        q->head = q->tail = 0;
    }

    static inline uint16_t CL_QueueCapacity(CL_QueueInfo_t *q)
    {
        return q->capacity;
    }

    bool CL_QueueEmpty(CL_QueueInfo_t *q);

    bool CL_QueueFull(CL_QueueInfo_t *q);

    uint16_t CL_QueueLength(CL_QueueInfo_t *q);

    uint16_t CL_QueueFreeSpace(CL_QueueInfo_t *q);

#define CL_QUEUE_FOR_EACH(q_ptr, data_ptr, data_type)          \
    data_ptr = ((data_type *)((q_ptr)->data)) + (q_ptr)->head; \
    for (int ___q_offset___ = (q_ptr)->head;                   \
         ___q_offset___ != (q_ptr)->tail;                      \
         ___q_offset___ = (___q_offset___ + 1) % ((q_ptr)->capacity + 1), data_ptr = ((data_type *)((q_ptr)->data)) + ___q_offset___)

#ifdef __cplusplus
}
#endif
