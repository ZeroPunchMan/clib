#pragma once

#include "cl_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        volatile void *buff;
        volatile uint16_t head;
        volatile uint16_t tail;

        volatile uint16_t capacity;
        volatile uint16_t data_size;
    } CL_Queue_t;

// ringbuffer实现队列, 实际缓存为容量+1, 用于区分是否满
#define CL_QUEUE_DEF_INIT(q_name, capacity, data_type, modifier) \
    static data_type q_name##_buf[capacity + 1];                 \
    modifier CL_Queue_t q_name = {q_name##_buf, 0, 0, capacity, sizeof(data_type)};

#define CL_QUEUE_DECL(q_name) \
    extern CL_Queue_t q_name;

    // 添加单个数据
    CL_Result_t CL_QueueAdd(CL_Queue_t *q, void *data);

    // 添加多个数据
    CL_Result_t CL_QueueMultiAdd(CL_Queue_t *q, void *data, uint16_t len);

    // 获取单个数据
    CL_Result_t CL_QueuePoll(CL_Queue_t *q, void *data);

    // 获取多个数据
    CL_Result_t CL_QueueMultiPoll(CL_Queue_t *q, void *data, uint16_t len);

    // 查看指定索引的数据
    CL_Result_t CL_QueuePeek(CL_Queue_t *q, uint16_t index, void **pptr);

    // 获取连续内存的数据,用于DMA之类的操作
    CL_Result_t CL_QeueuGetContinousData(CL_Queue_t *q, uint16_t len, void **pptr, uint16_t *pOutLen);

    // Poll,但不需要copy出来,用于数据处理完之后从队列丢掉已用的数据
    CL_Result_t CL_QueuePollWithoutCopy(CL_Queue_t *q, uint16_t len);

    // 获取连续空闲的内存,用于DMA之类的操作
    CL_Result_t CL_QeueuGetContinousFreeSpace(CL_Queue_t *q, uint16_t len, void **pptr, uint16_t *pOutLen);

    // Add,但是不需要复制数据,用于DMA填充完数据之后,移动尾指针
    CL_Result_t CL_QueueAddWithoutCopy(CL_Queue_t *q, uint16_t len);

    // 清理队列,如果生产和消费是异步的,要注意线程安全问题
    static inline void CL_QueueClear(CL_Queue_t *q)
    {
        q->head = q->tail = 0;
    }

    // 获取容量
    static inline uint16_t CL_QueueCapacity(CL_Queue_t *q)
    {
        return q->capacity;
    }

    bool CL_QueueEmpty(CL_Queue_t *q);

    bool CL_QueueFull(CL_Queue_t *q);

    uint16_t CL_QueueLength(CL_Queue_t *q);

    uint16_t CL_QueueFreeSpace(CL_Queue_t *q);

#define CL_QUEUE_FOR_EACH(q_ptr, data_ptr, data_type)          \
    data_ptr = ((data_type *)((q_ptr)->buff)) + (q_ptr)->head; \
    for (int ___q_offset___ = (q_ptr)->head;                   \
         ___q_offset___ != (q_ptr)->tail;                      \
         ___q_offset___ = (___q_offset___ + 1) % ((q_ptr)->capacity + 1), data_ptr = ((data_type *)((q_ptr)->buff)) + ___q_offset___)

#ifdef __cplusplus
}
#endif
