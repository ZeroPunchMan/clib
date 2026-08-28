#pragma once

#include "cl_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Ring buffer queue implementation: Suitable for single-threaded queue buffering, or thread-safe in single-consumer/single-producer scenarios
    typedef struct
    {
        volatile void *buff;
        volatile uint16_t head;
        volatile uint16_t tail;

        volatile uint16_t capacity;
        volatile uint16_t data_size;
    } CL_Queue_t;

// Ring buffer queue definition macro: actual buffer size = capacity + 1 (to distinguish full/empty)
#define CL_QUEUE_DEF_INIT(q_name, capacity, data_type, modifier) \
    static data_type q_name##_buf[capacity + 1];                 \
    modifier CL_Queue_t q_name = {q_name##_buf, 0, 0, capacity, sizeof(data_type)};

// Queue declaration macro (for external use)
#define CL_QUEUE_DECL(q_name) \
    extern CL_Queue_t q_name;

    // Add single element to queue
    CL_Result_t CL_QueueAdd(CL_Queue_t *q, void *data);

    // Add multiple elements to queue
    CL_Result_t CL_QueueMultiAdd(CL_Queue_t *q, void *data, uint16_t len);

    // Remove single element from queue (copy out)
    CL_Result_t CL_QueuePoll(CL_Queue_t *q, void *data);

    // Remove multiple elements from queue (copy out)
    CL_Result_t CL_QueueMultiPoll(CL_Queue_t *q, void *data, uint16_t len);

    // Peek at element at specified index without removing it
    CL_Result_t CL_QueuePeek(CL_Queue_t *q, uint16_t index, void **pptr);

    // Get contiguous memory from queue (for DMA or zero-copy operations)
    CL_Result_t CL_QeueuGetContinousData(CL_Queue_t *q, uint16_t len, void **pptr, uint16_t *pOutLen);

    // Remove data from queue without copying (used after processing direct data pointer)
    CL_Result_t CL_QueuePollWithoutCopy(CL_Queue_t *q, uint16_t len);

    // Get contiguous free space in queue (for DMA or zero-copy operations)
    CL_Result_t CL_QeueuGetContinousFreeSpace(CL_Queue_t *q, uint16_t len, void **pptr, uint16_t *pOutLen);

    // Add without copying: advance tail pointer after DMA fills the buffer
    CL_Result_t CL_QueueAddWithoutCopy(CL_Queue_t *q, uint16_t len);

    // Clear the queue. Note: if producer and consumer are asynchronous, thread safety must be ensured.
    static inline void CL_QueueClear(CL_Queue_t *q)
    {
        q->head = q->tail = 0;
    }

    // Get queue capacity
    static inline uint16_t CL_QueueCapacity(CL_Queue_t *q)
    {
        return q->capacity;
    }

    bool CL_QueueEmpty(CL_Queue_t *q);

    bool CL_QueueFull(CL_Queue_t *q);

    uint16_t CL_QueueLength(CL_Queue_t *q);

    uint16_t CL_QueueFreeSpace(CL_Queue_t *q);

// Macro for iterating through queue elements
#define CL_QUEUE_FOR_EACH(q_ptr, data_ptr, data_type)          \
    data_ptr = ((data_type *)((q_ptr)->buff)) + (q_ptr)->head; \
    for (int ___q_offset___ = (q_ptr)->head;                   \
         ___q_offset___ != (q_ptr)->tail;                      \
         ___q_offset___ = (___q_offset___ + 1) % ((q_ptr)->capacity + 1), data_ptr = ((data_type *)((q_ptr)->buff)) + ___q_offset___)

#ifdef __cplusplus
}
#endif
