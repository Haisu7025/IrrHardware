#ifndef __IIC_H
#define __IIC_H

//SDA接GPIO_Pin_11
//SCL接GPIO_Pin_10

#include "stm32f10x.h"

#define SDA1_DATA(a)                     \
	if (a)                               \
		GPIO_SetBits(GPIOB, GPIO_Pin_7); \
	else                                 \
		GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define SCL1_OUT(a)                      \
	if (a)                               \
		GPIO_SetBits(GPIOB, GPIO_Pin_6); \
	else                                 \
		GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define SDA1_IO_IN GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define SDA2_DATA(a)                      \
	if (a)                                \
		GPIO_SetBits(GPIOB, GPIO_Pin_11); \
	else                                  \
		GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define SCL2_OUT(a)                       \
	if (a)                                \
		GPIO_SetBits(GPIOB, GPIO_Pin_10); \
	else                                  \
		GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define SDA2_IO_IN GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

//IIC所有操作函数
void IIC1_Init(void); //初始化IIC的IO口
void SDA1_OUT(void);
void SDA1_IN(void);
void IIC1_Start(void);				  //发送IIC开始信号
void IIC1_Stop(void);				  //发送IIC停止信号
void IIC1_Send_Byte(u8 txd);		  //IIC发送一个字节
u8 IIC1_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 IIC1_Wait_Ack(void);				  //IIC等待ACK信号
void IIC1_Ack(void);				  //IIC发送ACK信号
void IIC1_NAck(void);				  //IIC不发送ACK信号
void IIC1_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC1_Read_One_Byte(u8 daddr, u8 addr);

//IIC所有操作函数
void IIC2_Init(void); //初始化IIC的IO口
void SDA2_OUT(void);
void SDA2_IN(void);
void IIC2_Start(void);				  //发送IIC开始信号
void IIC2_Stop(void);				  //发送IIC停止信号
void IIC2_Send_Byte(u8 txd);		  //IIC发送一个字节
u8 IIC2_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 IIC2_Wait_Ack(void);				  //IIC等待ACK信号
void IIC2_Ack(void);				  //IIC发送ACK信号
void IIC2_NAck(void);				  //IIC不发送ACK信号
void IIC2_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC2_Read_One_Byte(u8 daddr, u8 addr);

#endif
