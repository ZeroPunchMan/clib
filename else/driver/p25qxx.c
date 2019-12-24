#include "p25qxx.h"
#include "spi_gpio.h"
#include "stdlib.h"
#include "sys_time.h"


//page size
#define PAGE_SIZE   256

//ID
#define P25Q40_ID   0x8512

//flash size
#define P25Q40_SIZE (4 * 1024 * 1024 / 8) //4M bit


//cmd
#define CMD_READ_ARRAY      0x0b
#define CMD_READ_ARRAY_LP   0x03  //low power

#define CMD_PAGE_ERASE      0x81
#define CMD_CHIP_ERASE      0x60

#define CMD_PAGE_WRITE      0x02
#define CMD_WRITE_ENABLE    0x06
#define CMD_WRITE_DISABLE   0x04

#define CMD_READ_SR         0x05    //read status register
#define CMD_READ_SR2        0x35    //read status register 2

#define CMD_WRITE_SR        0x01    //write status register

#define CMD_POWER_DOWN      0xb9    //enter power down
#define CMD_RELEASE_PD      0xab    //release power down

#define CMD_READ_ID         0x90    // Manufacturer/device ID



uint32_t P25QXX_Size = 0; //����flash��С

//��ȡid
CL_Result_t P25QXX_ReadID(uint16_t* pID)
{
    CL_Result_t res = CL_ResFailed;
    uint16_t temp = 0;
    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_READ_ID);
    SPI_GPIOSendByteFast(0x00);
    SPI_GPIOSendByteFast(0x00);
    SPI_GPIOSendByteFast(0x00);
    temp |= SPI_GPIOSendByteFast(0xFF) << 8;
    temp |= SPI_GPIOSendByteFast(0xFF);
    SPI_CS_High();

    if((temp & 0xff) == 0x12)   //P25Q40
    {
        P25QXX_Size = P25Q40_SIZE;
        res = CL_ResSuccess;
    }
    else
    {

    }

    if(CL_ResSuccess == res)
    {
        *pID = temp;
    }
    return res;
}

//��ʼ��
CL_Result_t P25QXX_Init(void)
{
    uint16_t id;
    CL_Result_t res;
    SPI_GPIOInit();
    res = P25QXX_ReadID(&id);

    return res;
}

uint8_t P25QXX_ReadStaReg(uint8_t staReg)
{
    uint8_t res;
    SPI_CS_Low();
    switch (staReg)
    {
    case 1:
        SPI_GPIOSendByteFast(CMD_READ_SR);
        break;
    case 2:
        SPI_GPIOSendByteFast(CMD_READ_SR2);
        break;
    }
    res = SPI_GPIOSendByteFast(0xff);

    SPI_CS_High();
    return res;
}

CL_Result_t P25QXX_WaitBusy(uint16_t timeout)
{
    uint64_t startTime;
    SetToCurTime(&startTime);
    while((P25QXX_ReadStaReg(1) & 0x01) != 0)
    {
        if(TimeElapsed(startTime) >= timeout)
        {
            return CL_ResFailed;
        }
    }
    return CL_ResSuccess;
}

//����͹���ģʽ,δ����
void P25QXX_PowerDown(void)
{
    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_POWER_DOWN);
    SPI_CS_High();
}

//�˳��͹���,δ����
void P25QXX_ReleasePowerDown(void)
{
    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_RELEASE_PD);
    SPI_CS_High();
}

void P25QXX_WriteEnable(void)
{
    SPI_CS_Low();                             //ʹ������
    SPI_GPIOSendByteFast(CMD_WRITE_ENABLE);
    SPI_CS_High();                          //ȡ��Ƭѡ
}

void P25QXX_WriteDisable(void)
{
    SPI_CS_Low();                          //ʹ������
    SPI_GPIOSendByteFast(CMD_WRITE_DISABLE);
    SPI_CS_High();                          //ȡ��Ƭѡ
}


void P25QXX_Read(uint8_t* pBuff, uint32_t addr, uint16_t readBytes)
{
    SPI_CS_Low();

    SPI_GPIOSendByteFast(CMD_READ_ARRAY_LP);
    SPI_GPIOSendByteFast((addr >> 16) & 0xff);
    SPI_GPIOSendByteFast((addr >> 8) & 0xff);
    SPI_GPIOSendByteFast(addr & 0xff);

    for(uint16_t i = 0; i < readBytes; i++)
    {
        pBuff[i] = SPI_GPIOSendByteFast(0xff);
    }

    SPI_CS_High();
}

CL_Result_t P25QXX_PageErase(uint32_t addr)
{
    CL_Result_t res;
    P25QXX_WriteEnable(); //ÿ�ζ�Ҫ����д,�ڲ�����֮��,���Զ�disable
    res = P25QXX_WaitBusy(10);
    if(res != CL_ResSuccess)
        return res;

    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_PAGE_ERASE);
    SPI_GPIOSendByteFast((addr >> 16) & 0xff);
    SPI_GPIOSendByteFast((addr >> 8) & 0xff);
    SPI_GPIOSendByteFast(addr & 0xff);

    SPI_CS_High();

    return P25QXX_WaitBusy(10);
}

CL_Result_t P25QXX_PageWrite(const uint8_t* pBuff, uint32_t addr, uint16_t writeBytes)
{
    CL_Result_t res;
    if(writeBytes > PAGE_SIZE)
        return CL_ResInvalidParam;

    P25QXX_WriteEnable(); //ÿ�ζ�Ҫ����д,�ڲ�����֮��,���Զ�disable
    res = P25QXX_WaitBusy(10);
    if(res != CL_ResSuccess)
        return res;

    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_PAGE_WRITE);

    SPI_GPIOSendByteFast((addr >> 16) & 0xff);
    SPI_GPIOSendByteFast((addr >> 8) & 0xff);
    SPI_GPIOSendByteFast(addr & 0xff);

    for(uint16_t i = 0; i < writeBytes; i++)
    {
        SPI_GPIOSendByteFast(pBuff[i]);
    }

    SPI_CS_High();

    return P25QXX_WaitBusy(10);
}

CL_Result_t P25QXX_PageWriteCheck(const uint8_t* pBuff, uint32_t addr, uint16_t writeBytes, uint8_t tryTimes)
{
    static uint8_t readBuff[P25Q_PAGE_SIZE];

    for(uint8_t i = 0; i < tryTimes; i++)
    {
        CL_Result_t res;
        res = P25QXX_PageErase(addr);
        if(res != CL_ResSuccess)
        {
            continue;
        }

        res = P25QXX_PageWrite(pBuff, addr, writeBytes);
        if(res != CL_ResSuccess)
        {
            continue;
        }

        P25QXX_Read(readBuff, addr, writeBytes);
        if(memcmp(readBuff, pBuff, writeBytes) == 0)
        {
            return CL_ResSuccess;
        }
        else
        {
            continue;
        }
        
    }

    return CL_ResFailed;
}

CL_Result_t P25QXX_ChipErase(void)
{
    CL_Result_t res;
    P25QXX_WriteEnable(); //ÿ�ζ�Ҫ����д,�ڲ�����֮��,���Զ�disable
    res = P25QXX_WaitBusy(10);
    if(res != CL_ResSuccess)
        return res;
    
    SPI_CS_Low();
    SPI_GPIOSendByteFast(CMD_CHIP_ERASE);
    SPI_CS_High();

    return P25QXX_WaitBusy(100);
}

#if P25QXX_USE_TEST
static uint8_t testData[PAGE_SIZE];
void P25QXX_Test(void)
{
#define TEST_TIMES  10
    uint32_t addr;
    uint32_t maxPage = P25QXX_Size / PAGE_SIZE;
    uint32_t pageSegment = maxPage / TEST_TIMES;
    Log("page size: %d, page num: %d\r\n", P25QXX_Size, maxPage);

    P25QXX_ChipErase();
    //------дpage����-----
    for(int i = 0; i < TEST_TIMES; i++)
    {
        //����xx��
        //��ȡһ��ҳ��ַ
        addr = i * PAGE_SIZE * pageSegment;

        //��������
        for(int k = 0; k < PAGE_SIZE; k++)
        {
            testData[k] = (uint8_t)(i + k);
        }
        //����+д��
        //P25QXX_PageErase(addr);
        P25QXX_PageWrite(testData, addr, PAGE_SIZE);
        Log("p25q addr %d write done\r\n", addr);
    }

    for(int i = 0; i < TEST_TIMES; i++)
    {
        addr = i * PAGE_SIZE * pageSegment;
        //����ҳ��ȡ����
        memset(testData, 0, PAGE_SIZE);
        P25QXX_Read(testData, addr, PAGE_SIZE);

        //�Ƚ϶���д�Ƿ���ͬ
        for(int k = 0; k < PAGE_SIZE; k++)
        {
//            Log("%x, ", read[k]);
            if(testData[k] != (uint8_t)(i + k))
            {
                Log("p25q test failed\r\n");
                return;
            }
        }

        Log("p25q addr %d test ok\r\n", addr);
    }
    

    Log("p25q whole chip test ok\r\n");
}
#endif


