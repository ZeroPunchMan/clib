#include "htc3.h"
#include "gpio_iic.h"
#include "sys_time.h"

#define HTC3_DEV_READ   (0xe1)
#define HTC3_DEV_WRITE  (0xe0)

#define HTC3_CMD_ID    (0xefc8)
#define HTC3_CMD_WAKE   (0x3517)
#define HTC3_CMD_MEASURE    (0x7866)  //no scl stretching, Temp first


//娴嬮噺瓒呮椂 ms
#define HTC3_MEASURE_TIMEOUT     (100)

#define CRC_POLY    (0x31)
static uint8_t crc8(uint8_t* data, uint8_t size)
{
    uint8_t i,j,temp,crc = 0xff;
    for(i=0; i<size; i++)
    {
        temp = data[i];
        for(j=0; j<8; j++)
        {
            if( (crc ^ temp) & 0x80 )
            {
                crc = CRC_POLY ^(crc<<1);
            }
            else
            {
                crc <<=1;
            }
            temp<<=1;
        }
        crc&=0xff;
    }
    return crc;
}

static CL_Result_t HTC3_Cmd(uint16_t cmd)
{
    CL_Result_t res;

    //起始信号+设备写地址
    IIC_Start();
    IIC_Send_Byte(HTC3_DEV_WRITE);
    res = IIC_Wait_Ack();
    if(res != CL_ResSuccess)
        goto out;

    //寄存器写地址
    IIC_Send_Byte(cmd >> 8);
    res = IIC_Wait_Ack();
    if(res != CL_ResSuccess)
        goto out;

    //写入值
    IIC_Send_Byte(cmd & 0xff);
    res = IIC_Wait_Ack();
    if(res != CL_ResSuccess)
        goto out;

out:
    IIC_Stop();

    return res;
}

static CL_Result_t HTC3_Read(uint8_t* buff, uint8_t length)
{
    CL_Result_t res;
    IIC_Start();
    IIC_Send_Byte(HTC3_DEV_READ);

    res = IIC_Wait_Ack();
    if(res != CL_ResSuccess)
        goto out;

    for(uint8_t i = 0; i < length; i++)
    {
        *(buff++) = IIC_Read_Byte(true);
//        Log("data: %d\r\n", data);
    }

out:
    IIC_Stop();
    return res;
}

CL_Result_t HTC3_ReadId(uint16_t* pData)
{
    uint8_t crc = 0;
    CL_Result_t res = HTC3_Cmd(HTC3_CMD_ID);
    if(res != CL_ResSuccess)
        goto out;

    uint8_t buff[3];
    res = HTC3_Read(buff, 3);
    if(res != CL_ResSuccess)
        goto out;


    crc = crc8(buff, 2);
    if(crc != buff[2])
    {
        res = CL_ResFailed;
        goto out;
    }

    *pData = buff[0] << 8;
    *pData |= buff[1];
out:

    return res;
}

CL_Result_t HTC3_Init(void)
{
    CL_Result_t res;
    uint16_t id;
    IIC_Init();

    res = HTC3_ReadId(&id);
    if(res != CL_ResSuccess)
        goto out;

//    Log("htc3 id: %x\r\n", id);    
out:
    return res;
}

CL_Result_t HTC3_StartAsyncMeasure(void)
{
    return CL_ResFailed;
}

CL_Result_t HTC3_SyncMeasure(uint16_t* temp, uint16_t* humi)
{
    uint64_t time;
    CL_Result_t res;
    uint8_t buff[6];

    //wake up
    res = HTC3_Cmd(HTC3_CMD_WAKE);
    if(res != CL_ResSuccess)
        goto out;

    //wake up delay
    DelayMs(20);

    //measure
    res = HTC3_Cmd(HTC3_CMD_MEASURE);
    if(res != CL_ResSuccess)
        goto out;

    SetToCurTime(&time);
    while(1)
    {
        if(TimeElapsed(time) > HTC3_MEASURE_TIMEOUT)
        {
            res = CL_ResFailed;
            goto out;
        }

        res = HTC3_Read(buff, 6);
        if(res == CL_ResSuccess)
        {
            uint8_t crc = crc8(buff, 2);
            if(crc != buff[2])
            {
                res = CL_ResFailed;
                goto out;
            }

            crc = crc8(buff+3, 2);
            if(crc != buff[5])
            {
                res = CL_ResFailed;
                goto out;
            }

            uint16_t iTemp = buff[0] << 8;
            iTemp |= buff[1];
            *temp = iTemp;


            uint16_t iHumi = buff[3] << 8;
            iHumi |= buff[4];
            *humi = iHumi;
            
            break;
        }
    }

out:

    IIC_Stop();

    return res;

}


void HTC3_Exit(void)
{
    IIC_Exit();
}


