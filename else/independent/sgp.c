#include "sgp.h"
#include "crc.h"
#include "sys_time.h"
#include "uart.h"
#include "stdlib.h"
#include "cl_event_system.h"

//protocol head
static const uint8_t protoHead[] = {0xfe, 0xef};

//received massage
static SGP_RecvMsg_t recvMsg;

//send function
SGP_SendFunc sendFunc = CL_NULL;

static bool initialized = false;

typedef enum
{
    PS_Head,       //2 bytes
    PS_MsgType,    //1 byte
    PS_Length,     //1 byte
    PS_Data,       //n bytes
    PS_Verify,     //2 byte
} ParseStatus_t;

ParseStatus_t parseStatus = PS_Head;

static uint16_t headCount = 0;
static void ToParseHead()
{
    parseStatus = PS_Head;
    headCount = 0;
}

static void ToParseCmd()
{
    parseStatus = PS_MsgType;
}

static void ToParseLength()
{
    parseStatus = PS_Length;
}

int recvDataCount = 0;
static void ToParseData()
{
    parseStatus = PS_Data;
    recvDataCount = 0;
}

uint8_t verifyCount = 0;
static void ToParseVerify()
{
    parseStatus = PS_Verify;
    verifyCount = 0;
}


uint16_t verifyValue;
static void VeriyProcess(uint8_t byte)
{
    uint16_t crc = 0;
    switch (verifyCount)
    {
    case 0:
        verifyValue = ((uint16_t)byte) << 8;  //verify code 1st byte
        break;
    case 1:
        verifyValue |= (uint16_t)byte;  //verify code secode byte
        crc = CalcCRC16((uint8_t*)(&recvMsg), 2 + recvMsg.length);   //calculate crc

//        Log("recv crc: %x, calc: %x\r\n", verifyValue, crc);

        if(crc == verifyValue)
        {
            //verify code matched
            // RaiseRecvMsgEvent(&recvMsg);
            CL_EventSysRaise(CL_Event_SgpRecvMsg, 0, &recvMsg); 
            // CL_EventSysRaise(CL_Event_SgpRecvMsg, recvMsg.msgType, &recvMsg); 
            ToParseHead(); //reset parse status
//            Log("recv msg: %x, length: %d\r\n",  recvMsg.msgType, recvMsg.length);
        }
        else
        {
            //verify code mismatched
            ToParseHead(); //reset parse status
            Log("msg verify error\r\n");
        }
        break;
    default:
        break;
    }

    verifyCount++;
}

void SGP_ProtoRecvByte(uint8_t byte)
{
    static uint64_t lastRecvTime = 0;
    if(TimeElapsed(lastRecvTime) > SGP_FRAME_TIMEOUT)
    {
        //frame timeout
        ToParseHead();
    }
    SetToCurTime(&lastRecvTime);    //save current time

    switch (parseStatus)
    {
    case PS_Head:
//        Log("recv: %x; head: %2x\r\n", byte, protoHead[headCount]);
        if(byte == protoHead[headCount])
        {
            //parse frame head
//            Log("head inc\r\n");
            headCount++;
            if(headCount >= CL_ARRAY_LENGTH(protoHead))
            {
//                Log("recv head\r\n");
                ToParseCmd();
            }
        }
        else
        {
            headCount = 0;
        }
        break;
    case PS_MsgType:
//        Log("recv msg: %2x\r\n", byte);
        recvMsg.msgType = byte;
        ToParseLength();
        break;
    case PS_Length:
//        Log("recv length: %2x\r\n", byte);
        if(byte == 0)
        {
            //length is 0, means no data
            recvMsg.length = 0;
            ToParseVerify();   //prepare to parse verify code
        }
        else
        {
            //length is not 0
            recvMsg.length = byte;
            ToParseData();  //prepare to receive data
        }
        break;
    case PS_Data:
        recvMsg.data[recvDataCount++] = byte;
        if(recvDataCount >= recvMsg.length)
        {
            //receiving data
            ToParseVerify();
//            Log("recv data ok\r\n");
        }
        break;
    case PS_Verify:
        VeriyProcess(byte);
        break;
    default:
        break;
    }
}

void SGP_ProtoRecvData(const uint8_t* buff, uint16_t count)
{
    for(uint16_t i = 0; i < count; i++)
    {
        SGP_ProtoRecvByte(buff[i]);
    }
}


void SGP_Init(SGP_SendFunc s)
{
    sendFunc = s;
    initialized = true;
}

void SGP_Exit(void)
{
    sendFunc = CL_NULL;
    initialized = false;
}

CL_Result_t SGP_SendMsg(DevMsgType_t msgType, const uint8_t* data, uint8_t length)
{
    uint8_t mt[2];
    uint16_t crc = 0;
    CL_Result_t res;

    if(sendFunc == CL_NULL)
        return CL_ResFailed;

    //send frame head
    res = sendFunc(protoHead, sizeof(protoHead));
    //UARTx_Send(UART1, protoHead, sizeof(protoHead), &sendBytes);
    if(res != CL_ResSuccess)
        return res;


    //send frame type and length of data

    mt[0] = msgType;
    mt[1] = length;
    res = sendFunc(mt, 2);
    if(res != CL_ResSuccess)
        return res;

    //send data if it has
    if(length != 0)
    {
        res = sendFunc(data, length);
        if(res != CL_ResSuccess)
            return res;
    }

    //crc

    crc = CalcCRC16(mt, 2);
    if(length != 0)
        crc = CalcCRC16Ex(crc, data, length);

    mt[0] = (crc >> 8) & 0xff;
    mt[1] = crc & 0xff;
    res = sendFunc(mt, 2);
    if(res != CL_ResSuccess)
        return res;

    return CL_ResSuccess;
}

void SGP_Process(void)
{
    if(initialized == false)
        return;
        
    //TODO optimize receiving process
    uint8_t data;
    for(uint8_t i = 0; i < 200; i++)    //200 times max
    {
        if(CL_QueuePoll(&UART1_RecvBuffer, &data) == CL_ResSuccess)
        {
            //receive one byte
            SGP_ProtoRecvByte(data);
            //Log("recv: %2x, ", data);
        }
        else
        {
            break;
        }
    }
}

