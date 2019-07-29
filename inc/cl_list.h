#pragma once

#include "cl_common.h"

//linked loop list
typedef struct CL_ListNode
{
    struct CL_ListNode* preNode;
    struct CL_ListNode* nextNode;
} CL_ListNode_t;

typedef struct CL_List
{
    CL_ListNode_t head;
    uint16_t size;
} CL_List_t;

void CL_ListInit(CL_List_t* list);

CL_RESULT CL_ListAddLast(CL_List_t* list, CL_ListNode_t* node);

CL_RESULT CL_ListAddFirst(CL_List_t* list, CL_ListNode_t* node);

//this function doesn't check whether node in list
CL_RESULT CL_ListRemove(CL_List_t* list, CL_ListNode_t* node);

static inline uint16_t CL_ListSize(CL_List_t* list)
{
    return list->size;
}

//for each
#define CL_LIST_FOR_EACH(list_ptr, node_ptr) \
for(node_ptr = (list_ptr)->head.nextNode; node_ptr != &(list_ptr)->head; node_ptr = node_ptr->nextNode)

//for each safe
#define CL_LIST_FOR_EACH_SAFE(list_ptr, node_ptr) \
node_ptr = (list_ptr)->head.nextNode; \
for(CL_ListNode_t* ___next_ptr_ = node_ptr->nextNode; \
    node_ptr != &(list_ptr)->head; \
    node_ptr = ___next_ptr_, ___next_ptr_ = ___next_ptr_->nextNode)

//for each reverse
#define CL_LIST_FOR_EACH_REVERSE(list_ptr, node_ptr) \
for(node_ptr = (list_ptr)->head.preNode; node_ptr != &(list_ptr)->head; node_ptr = node_ptr->preNode)

//for each reverse safe
#define CL_LIST_FOR_EACH_REVERSE_SAFE(list_ptr, node_ptr) \
node_ptr = (list_ptr)->head.preNode; \
for(CL_ListNode_t* ___pre_ptr_ = node_ptr->preNode; \
    node_ptr != &(list_ptr)->head; \
    node_ptr = ___pre_ptr_, ___pre_ptr_ = ___pre_ptr_->preNode)

//for each entry
#define CL_LIST_FOR_EACH_ENTRY(list_ptr, ctn_ptr, type, member) \
for(ctn_ptr = container_of((list_ptr)->head.nextNode, type, member); \
    &ctn_ptr->member != &(list_ptr)->head; \
    ctn_ptr = container_of(ctn_ptr->member.nextNode, type, member))

//for each entry safe
#define CL_LIST_FOR_EACH_ENTRY_SAFE(list_ptr, ctn_ptr, type, member) \
ctn_ptr = container_of((list_ptr)->head.nextNode, type, member); \
for(type* ___next_ctn_ = container_of(ctn_ptr->member.nextNode, type, member); \
    &ctn_ptr->member != &(list_ptr)->head; \
    ctn_ptr = ___next_ctn_, ___next_ctn_ = container_of(___next_ctn_->member.nextNode, type, member)) 

//for each entry reverse
#define CL_LIST_FOR_EACH_ENTRY_REVERSE(list_ptr, ctn_ptr, type, member) \
for(ctn_ptr = container_of((list_ptr)->head.preNode, type, member); \
    &ctn_ptr->member != &(list_ptr)->head; \
    ctn_ptr = container_of(ctn_ptr->member.preNode, type, member))

//for each entry reverse safe
#define CL_LIST_FOR_EACH_ENTRY_REVERSE_SAFE(list_ptr, ctn_ptr, type, member) \
ctn_ptr = container_of((list_ptr)->head.preNode, type, member); \
for(type* ___pre_ctn_ = container_of(ctn_ptr->member.preNode, type, member); \
    &ctn_ptr->member != &(list_ptr)->head; \
    ctn_ptr = ___pre_ctn_, ___pre_ctn_ = container_of(___pre_ctn_->member.preNode, type, member)) 

