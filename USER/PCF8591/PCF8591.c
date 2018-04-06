
#include "PCF8591.h"
#include "delay.h"

//**********************************PCF8591操作函数****************************

//初始化IIC接口
void PCF8591_A_Init(void)
{
	IIC_A_Init();
}

void pcf8591_A_ad_write(u8 c)
{
  	IIC_A_Start();
  	IIC_A_Send_Byte(pcf8591_write); //发送器件地址+写指令
  	IIC_A_Send_Byte(c);
  	IIC_A_Stop();
}

u8 pcf8591_A_ad_read(void)
{
  	u8 dat;
  	IIC_A_Start();
  	IIC_A_Send_Byte(pcf8591_read); //发送器件地址+读指令
  	dat=IIC_A_Read_Byte(0);
  	IIC_A_Stop();
  	return(dat*2);
}

void pcf8591_A_da_write(u8 dat)
{
  	IIC_A_Start();
  	IIC_A_Send_Byte(pcf8591_write); //发送器件地址+写指令
  	IIC_A_Send_Byte(0x40);          //发送控制字节
  	IIC_A_Send_Byte(dat);           //发送DA数值
  	IIC_A_Stop();
}

void PCF8591_B_Init(void)
{
	IIC_B_Init();
}

void pcf8591_B_ad_write(u8 c)
{
  	IIC_B_Start();
  	IIC_B_Send_Byte(pcf8591_write); //发送器件地址+写指令
  	IIC_B_Send_Byte(c);
  	IIC_B_Stop();
}

u8 pcf8591_B_ad_read(void)
{
  	u8 dat;
  	IIC_B_Start();
  	IIC_B_Send_Byte(pcf8591_read); //发送器件地址+读指令
  	dat=IIC_B_Read_Byte(0);
  	IIC_B_Stop();
  	return(dat*2);
}

void pcf8591_B_da_write(u8 dat)
{
  	IIC_B_Start();
  	IIC_B_Send_Byte(pcf8591_write); //发送器件地址+写指令
  	IIC_B_Send_Byte(0x40);          //发送控制字节
  	IIC_B_Send_Byte(dat);           //发送DA数值
  	IIC_B_Stop();
}

//用户使用函数
u8 AD_A_generate_data(u8 channel){
	//channel:0,1,2,3
	u8 AD_data;
	pcf8591_A_ad_write(0x41+channel);
	delay_us(50);
	AD_data=pcf8591_A_ad_read();
	return AD_data;
}

u8 AD_B_generate_data(u8 channel){
	//channel:0,1,2,3
	u8 AD_data;
	pcf8591_B_ad_write(0x41+channel);
	delay_us(50);
	AD_data=pcf8591_B_ad_read();
	return AD_data;
}