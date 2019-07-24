#include "stdio.h"
#include "cl_queue.h"

typedef struct 
{
    uint8_t a;
    int16_t b;
    uint32_t c;
} TestStruct;

CL_QUEUE_DEF_INIT(test_q, 256, TestStruct, static);
CL_BOOL DiscardWhenFull(void)
{

}

CL_BOOL PollWhenFull(void)
{

}

int main(int argc, char** argv)
{
    TestStruct ts;
    for(int i = 0; i < 10; i++)
    {
        ts.a = i;
        ts.b = i;
        ts.c = i;
        CL_QueueAdd(&test_q, &ts);
    }
    printf("123123\n");
}
