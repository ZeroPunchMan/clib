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
    CL_ListNode_t* head;
    uint16_t size;
} CL_List_t;

#define CL_LIST_DEF_INIT(name, modifer) \
modifier CL_List_t name = {CL_NULL, 0};

#define CL_LIST_DECL(name) \
extern CL_List_t name;

void CL_ListInit(CL_List_t* list);

CL_RESULT CL_ListAddLast(CL_List_t* list, CL_ListNode_t* node);

CL_RESULT CL_ListAddFirst(CL_List_t* list, CL_ListNode_t* node);

CL_RESULT CL_ListRemove(CL_List_t* list, CL_ListNode_t* node);

static inline uint16_t CL_ListSize(CL_List_t* list)
{
    return list->size;
}

#define CL_LIST_FOR_EACH(list, node_ptr) \
node_ptr = list->head; \
for(int __list_count_ = 0; \
    __list_count_ < list->size; \
    __list_count_++, node_ptr = list->nextNode)

#define CL_LIST_FOR_EACH_ENTRY(list, ctn_ptr, type, member) \
CL_ListNode_t* __node_ptr_ = list->head; \
for(int __list_count_ = 0; \
    __list_count_ < list->size; \
    __list_count_++, __node_ptr_ = list->nextNode, ctn_ptr = container_of(__node_ptr_, type, member))

#define CL_LIST_FOR_EACH_REVERSE(list, node_ptr) \
node_ptr = (list->head != CL_NULL) ? list->head->preNode : CL_NULL; \
for(int __list_count_ = 0; \
    __list_count_ < list->size; \
    __list_count_++, node_ptr = list->preNode)

#define CL_LIST_FOR_EACH_ENTRY_REVERSE(list, ctn_ptr, type, member) \
CL_ListNode_t* __node_ptr_ = (list->head != CL_NULL) ? list->head->preNode : CL_NULL; \
for(int __list_count_ = 0; \
    __list_count_ < list->size; \
    __list_count_++, __node_ptr_ = list->preNode, ctn_ptr = container_of(__node_ptr_, type, member))

//add safe for each
