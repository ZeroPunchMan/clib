#include "cl_list.h"

void CL_ListInit(CL_List_t *list)
{
    list->head.preNode = &list->head;
    list->head.nextNode = &list->head;
}

CL_Result_t CL_ListAddLast(CL_List_t *list, CL_ListNode_t *newNode)
{
    if (list == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *last = head->preNode;
    last->nextNode = newNode;
    newNode->preNode = last;
    newNode->nextNode = head;
    head->preNode = newNode;

    return CL_ResSuccess;
}

CL_Result_t CL_ListAddFirst(CL_List_t *list, CL_ListNode_t *newNode)
{
    if (list == CL_NULL || newNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *head = &list->head;
    CL_ListNode_t *next = head->nextNode;
    head->nextNode = newNode;
    newNode->preNode = head;
    next->preNode = newNode;
    newNode->nextNode = next;

    return CL_ResSuccess;
}

CL_Result_t CL_ListAddAfter(CL_ListNode_t *node, CL_ListNode_t *newNode)
{
    if (node == CL_NULL || node == CL_NULL)
        return CL_ResInvalidParam;

    if (node->preNode == CL_NULL || node->nextNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *next = node->nextNode;
    node->nextNode = newNode;
    newNode->preNode = node;
    newNode->nextNode = next;
    next->preNode = newNode;

    return CL_ResSuccess;
}

CL_Result_t CL_ListAddBefore(CL_ListNode_t *node, CL_ListNode_t *newNode)
{
    if (node == CL_NULL || node == CL_NULL)
        return CL_ResInvalidParam;

    if (node->preNode == CL_NULL || node->nextNode == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t* previous = node->preNode;
    previous->nextNode = newNode;
    newNode->preNode = previous;
    newNode->nextNode = node;
    node->preNode = newNode;

    return CL_ResSuccess;
}

CL_Result_t CL_ListRemove(CL_List_t *list, CL_ListNode_t *node)
{
    if (list == CL_NULL || node == CL_NULL)
        return CL_ResInvalidParam;

    CL_ListNode_t *preNode;
    CL_ListNode_t *nextNode;
    preNode = node->preNode;
    nextNode = node->nextNode;

    preNode->nextNode = nextNode;
    nextNode->preNode = preNode;

    return CL_ResSuccess;
}
