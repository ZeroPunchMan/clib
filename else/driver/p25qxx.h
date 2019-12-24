#pragma once 
#include "cl_common.h"

#define P25QXX_USE_TEST 1

//page size
#define P25Q_PAGE_SIZE   256

#define P25Q_PAGE2ADDR(page) (page * P25Q_PAGE_SIZE) //page 转地址


CL_Result_t P25QXX_ReadID(uint16_t* pID);
CL_Result_t P25QXX_Init(void);
void P25QXX_PowerDown(void); //δ����
void P25QXX_ReleasePowerDown(void); //δ����

//��ȡ
void P25QXX_Read(uint8_t* pBuff, uint32_t addr, uint16_t readBytes);

//����һҳ
CL_Result_t P25QXX_PageErase(uint32_t addr);

//һ��ֻ��дһҳ,256 bytes
CL_Result_t P25QXX_PageWrite(const uint8_t* pBuff, uint32_t addr, uint16_t writeBytes);

//page write with check
CL_Result_t P25QXX_PageWriteCheck(const uint8_t* pBuff, uint32_t addr, uint16_t writeBytes, uint8_t tryTimes);

//��Ƭ����
CL_Result_t P25QXX_ChipErase(void);


#if P25QXX_USE_TEST
void P25QXX_Test(void); //for test
#endif

