#include "spi_gpio.h"
#include "HAL_gpio.h"
#include "HAL_rcc.h"

#include "sys_time.h"



//-------------MOSI---------------------
static inline void SPI_MOSI_Low()
{
    GPIO_WriteBit(SPI_MOSI_PORT, SPI_MOSI_PIN, Bit_RESET);
}

static inline void SPI_MOSI_High()
{
    GPIO_WriteBit(SPI_MOSI_PORT, SPI_MOSI_PIN, Bit_SET);
}

//-------------CLK-------------------
static inline void SPI_CLK_Low()
{
    GPIO_WriteBit(SPI_CLK_PORT, SPI_CLK_PIN, Bit_RESET);
}

static inline void SPI_CLK_High()
{
    GPIO_WriteBit(SPI_CLK_PORT, SPI_CLK_PIN, Bit_SET);
}

//---------------MISO-------------------
static inline uint8_t SPI_MISO_Read()
{
    return GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN);
}

void SPI_GPIOInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin  = SPI_CS_PIN;  //cs
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = SPI_MOSI_PIN;  //MOSI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = SPI_CLK_PIN;  //CLK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_CLK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = SPI_MISO_PIN;  //MISO
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);
    
    SPI_CS_High();
    SPI_CLK_High();
    SPI_MOSI_High();
}


//发送1字节
uint8_t SPI_GPIOSendByte(uint8_t byte, uint16_t delay)
{
    uint8_t i, Dat = 0;

    DelayUs(delay);
    for(i=0; i<8; i++)
    {
        Dat<<=1;
        SPI_CLK_Low();
        //DelayUs(delay);
        (byte&0x80) ? SPI_MOSI_High() : SPI_MOSI_Low();     // ????????????????
        DelayUs(delay);
        SPI_CLK_High();
        Dat |= SPI_MISO_Read();
        byte<<=1;
        DelayUs(delay);
    }

    SPI_MOSI_High();                                    //  ???MOSI
    return Dat;
}

//快速发送,无延时
uint8_t SPI_GPIOSendByteFast(uint8_t byte)
{
    uint8_t i, Dat = 0;

    for(i=0; i<8; i++)
    {
        Dat<<=1;
        SPI_CLK_Low();
        (byte&0x80) ? SPI_MOSI_High() : SPI_MOSI_Low();     // ????????????????
        SPI_CLK_High();
        Dat |= SPI_MISO_Read();
        byte<<=1;
    }

    SPI_MOSI_High();                                    //  ???MOSI
    return Dat;
}



//uint8_t SPI_GPIOReadReg(uint8_t reg)
//{
//    uint8_t res;
//    CS_Low();
//    SendByteFast(reg);
//    res = SendByteFast(0xff);
//    CS_High();
//    return res;
//}
//
//void SPI_GPIOReadMultiReg(uint8_t reg, uint8_t* buff, uint16_t length)
//{
//    uint16_t i;
//    CS_Low();
//    SendByteFast(reg);
//    for(i = 0; i < length; i++)
//    {
//        buff[i] = SendByteFast(0xff);
//    }
//    CS_High();
//    
//}
//
//
//
//void SPI_GPIOWriteReg(uint8_t reg, uint8_t value)
//{
//    CS_Low();
//    SendByteFast(reg);
//    SendByteFast(value);
//    CS_High();
//}
//
//void SPI_GPIOWriteMultiReg(uint8_t reg, uint8_t* buff, uint16_t length)
//{
//    uint16_t i;
//    CS_Low();
//    SendByteFast(reg);
//    for(i = 0; i < length; i++)
//    {
//        SendByteFast(buff[i]);
//    }
//    CS_High();
//}



