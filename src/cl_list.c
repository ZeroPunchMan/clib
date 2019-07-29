#include "cl_list.h"

void CL_ListInit(CL_List_t *list)
{
    list->head.preNode = &list->head;
    list->head.nextNode = &list->head;    
    list->size = 0;
}

CL_RESULT CL_ListAddLast(CL_List_t *list, CL_ListNode_t *node)
{
    if (list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *last = head->preNode;
    last->nextNode = node;
    node->preNode = last;
    node->nextNode = head;
    head->preNode = node;
    list->size++;

    return CL_SUCCESS;
}

CL_RESULT CL_ListAddFirst(CL_List_t *list, CL_ListNode_t *node)
{
    if (list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *last = head->preNode;
    head->preNode = node;
    node->nextNode = head;
    last->nextNode = node;
    node->preNode = last;

    list->size++;

    return CL_SUCCESS;
}

CL_RESULT CL_ListRemove(CL_List_t *list, CL_ListNode_t *node)
{
    if (list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    if (list->size == 0)
        return CL_INVALID_PARAM;

    CL_ListNode_t* preNode;
    CL_ListNode_t* nextNode;
    preNode = node->preNode;
    nextNode = node->nextNode;

    preNode->nextNode = nextNode;
    nextNode->preNode = preNode;

    list->size--;

    return CL_FAILED;
}

