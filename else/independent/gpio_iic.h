#pragma once

#include "cl_common.h"

//SDA
#define IIC_SDA_PORT_GROUP      GpioPortB
#define IIC_SDA_PORT_NUM        GpioPin7

//SCL
#define IIC_SCL_PORT_GROUP      GpioPortB
#define IIC_SCL_PORT_NUM        GpioPin6

// IIC所有操作函数
void IIC_Init(void);                        // 初始化IIC的IO口   
void IIC_Exit(void);
void IIC_Start(void);                       // 发送IIC开始信号
void IIC_Stop(void);                        // 发送IIC停止信号
void IIC_Send_Byte(uint8_t byte);                 // IIC发送一个字节
uint8_t IIC_Read_Byte(CL_BOOL ack);        // IIC读取一个字节
CL_RESULT IIC_Wait_Ack(void);                      // IIC等待ACK信号
void IIC_Ack(void);                         // IIC发送ACK信号
void IIC_NAck(void);                        // IIC不发送ACK信号




