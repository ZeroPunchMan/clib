#ifndef _CL_QUEUE_H_
#define _CL_QUEUE_H_

#include "cl_common.h"

#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct{ 
    volatile void *data;
    volatile int32_t head;
    volatile int32_t tail;

    volatile int32_t capacity;
    volatile int32_t data_size;
} CL_QueueInfo_t; 


#define CL_QUEUE_DEF_INIT(q_name, capacity, data_type, modifier) \
static data_type q_name##_buf[capacity + 1]; \
modifier CL_QueueInfo_t q_name = {q_name##_buf, 0, 0, capacity, sizeof(data_type)};

#define CL_QUEUE_DECL(q_name) \
extern CL_QueueInfo_t q_name;

CL_RESULT CL_QueueAdd(CL_QueueInfo_t *q, void *data);

CL_RESULT CL_QueuePoll(CL_QueueInfo_t *q, void *data);

CL_STATIC_INLINE void CL_QueueClear(CL_QueueInfo_t *q){
    q->head = q->tail = 0;
}

CL_STATIC_INLINE int32_t CL_QueueCapacity(CL_QueueInfo_t *q){
    return q->capacity;
}

CL_BOOL CL_QueueEmpty(CL_QueueInfo_t *q);

CL_BOOL CL_QueueFull(CL_QueueInfo_t *q);

#ifdef __cplusplus
}
#endif

#endif
