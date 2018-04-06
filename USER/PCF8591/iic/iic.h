#ifndef __IIC_H
#define __IIC_H

//SDA_A接GPIO_Pin_7 B
//SCL_A接GPIO_Pin_6 B

//SDA_B接GPIO_Pin_11 B
//SCL_B接GPIO_Pin_10 B
#include "stm32f10x.h"

#define SDA_A_DATA(a)                    \
	if (a)                               \
		GPIO_SetBits(GPIOB, GPIO_Pin_7); \
	else                                 \
		GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define SCL_A_OUT(a)                     \
	if (a)                               \
		GPIO_SetBits(GPIOB, GPIO_Pin_6); \
	else                                 \
		GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define SDA_A_IO_IN GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define SDA_B_DATA(a)                     \
	if (a)                                \
		GPIO_SetBits(GPIOB, GPIO_Pin_11); \
	else                                  \
		GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define SCL_B_OUT(a)                      \
	if (a)                                \
		GPIO_SetBits(GPIOB, GPIO_Pin_10); \
	else                                  \
		GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define SDA_B_IO_IN GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

//IIC所有操作函数
void IIC_A_Init(void); //初始化IIC的IO口
void SDA_A_OUT(void);
void SDA_A_IN(void);
void IIC_A_Start(void);				   //发送IIC开始信号
void IIC_A_Stop(void);				   //发送IIC停止信号
void IIC_A_Send_Byte(u8 txd);		   //IIC发送一个字节
u8 IIC_A_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 IIC_A_Wait_Ack(void);			   //IIC等待ACK信号
void IIC_A_Ack(void);				   //IIC发送ACK信号
void IIC_A_NAck(void);				   //IIC不发送ACK信号
void IIC_A_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_A_Read_One_Byte(u8 daddr, u8 addr);

void IIC_B_Init(void); //初始化IIC的IO口
void SDA_B_OUT(void);
void SDA_B_IN(void);
void IIC_B_Start(void);				   //发送IIC开始信号
void IIC_B_Stop(void);				   //发送IIC停止信号
void IIC_B_Send_Byte(u8 txd);		   //IIC发送一个字节
u8 IIC_B_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 IIC_B_Wait_Ack(void);			   //IIC等待ACK信号
void IIC_B_Ack(void);				   //IIC发送ACK信号
void IIC_B_NAck(void);				   //IIC不发送ACK信号
void IIC_B_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_B_Read_One_Byte(u8 daddr, u8 addr);
#endif
