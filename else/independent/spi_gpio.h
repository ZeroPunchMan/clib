#pragma once

#include "cl_common.h"
#include "board_config.h"

//gpioÄ£Äâspi

//------------CS-------------------
static inline void SPI_CS_Low()
{
    GPIO_WriteBit(SPI_CS_PORT, SPI_CS_PIN, Bit_RESET);
}

static inline void SPI_CS_High()
{
    GPIO_WriteBit(SPI_CS_PORT, SPI_CS_PIN, Bit_SET);
}

void SPI_GPIOInit(void);

uint8_t SPI_GPIOSendByte(uint8_t byte, uint16_t delay);
uint8_t SPI_GPIOSendByteFast(uint8_t byte);


//uint8_t SPI_GPIOReadReg(uint8_t reg);
//void SPI_GPIOWriteReg(uint8_t reg, uint8_t value);
//
//void SPI_GPIOWriteMultiReg(uint8_t reg, uint8_t* buff, uint16_t length);
//void SPI_GPIOReadMultiReg(uint8_t reg, uint8_t* buff, uint16_t length);


