
#include "PCF8591.h"
#include "delay.h"

//**********************************PCF8591操作函数****************************

//初始化IIC接口
void PCF8591a_Init(void)
{
	IIC1_Init();
}

void pcf8591a_ad_write(u8 c)
{
	IIC1_Start();
	IIC1_Send_Byte(pcf8591_write); //发送器件地址+写指令
	IIC1_Send_Byte(c);
	IIC1_Stop();
}

u8 pcf8591a_ad_read(void)
{
	u8 dat;
	IIC1_Start();
	IIC1_Send_Byte(pcf8591_read); //发送器件地址+读指令
	dat = IIC1_Read_Byte(0);
	IIC1_Stop();
	return (dat);
}

void pcf8591a_da_write(u8 dat)
{
	IIC1_Start();
	IIC1_Send_Byte(pcf8591_write); //发送器件地址+写指令
	IIC1_Send_Byte(0x40);		   //发送控制字节
	IIC1_Send_Byte(dat);		   //发送DA数值
	IIC1_Stop();
}

//初始化IIC接口
void PCF8591b_Init(void)
{
	IIC2_Init();
}

void pcf8591b_ad_write(u8 c)
{
	IIC2_Start();
	IIC2_Send_Byte(pcf8591_write); //发送器件地址+写指令
	IIC2_Send_Byte(c);
	IIC2_Stop();
}

u8 pcf8591b_ad_read(void)
{
	u8 dat;
	IIC2_Start();
	IIC2_Send_Byte(pcf8591_read); //发送器件地址+读指令
	dat = IIC2_Read_Byte(0);
	IIC2_Stop();
	return (dat);
}

void pcf8591b_da_write(u8 dat)
{
	IIC2_Start();
	IIC2_Send_Byte(pcf8591_write); //发送器件地址+写指令
	IIC2_Send_Byte(0x40);		   //发送控制字节
	IIC2_Send_Byte(dat);		   //发送DA数值
	IIC2_Stop();
}