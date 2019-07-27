#include "cl_list.h"

void CL_ListInit(CL_List_t* list)
{
    list->head = CL_NULL;
    list->size = 0;
}

CL_RESULT CL_ListAddLast(CL_List_t* list, CL_ListNode_t* node)
{
    if(list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    if(list->head == CL_NULL)
    {
        list->head = node;
        node->preNode = node;
        node->nextNode = node;
        list->size = 1;
    }
    else
    {
        CL_ListNode_t* head = list->head;
        CL_ListNode_t* last = head->preNode;
        last->nextNode = node;
        node->preNode = last;
        node->nextNode = list->head;
        head->preNode = node;
        list->size++;
    }

    return CL_SUCCESS;
}

CL_RESULT CL_ListAddFirst(CL_List_t* list, CL_ListNode_t* node)
{
    if(list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    if(list->head == CL_NULL)
    {
        list->head = node;
        node->preNode = node;
        node->nextNode = node;
        list->size = 1;
    }
    else
    {
        CL_ListNode_t* head = list->head;
        CL_ListNode_t* last = head->preNode;
        head->preNode = node;
        node->nextNode = head;
        last->nextNode = node;
        node->preNode = last;

        list->size++;
    }

    return CL_SUCCESS;
}

CL_RESULT CL_ListRemove(CL_List_t* list, CL_ListNode_t* node)
{
    if(list == CL_NULL || node == CL_NULL)
        return CL_INVALID_PARAM;

    if(list->size == 0)
        return CL_INVALID_PARAM;

    //find the node, then remove it
    CL_ListNode_t* itr = list->head;
    for(int i = 0; i < list->size; i++, itr = itr->nextNode)
    {
        if(itr == node)
        {
            
            return CL_SUCCESS;
        }
    }

    return CL_FAILED;
}
