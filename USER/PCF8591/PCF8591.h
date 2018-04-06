#ifndef __PCF8591_H__
#define __PCF8591_H__

#include "stm32f10x.h"
#include "iic.h"  

#define pcf8591_read  0x91              //PCF8591读控制字
#define pcf8591_write 0x90              //PCF8591写控制字 
	
void PCF8591_A_Init(void);				  
void pcf8591_A_ad_write(u8 c);
u8 pcf8591_A_ad_read(void);
void pcf8591_A_da_write(u8 dat);

void PCF8591_B_Init(void);				  
void pcf8591_B_ad_write(u8 c);
u8 pcf8591_B_ad_read(void);
void pcf8591_B_da_write(u8 dat);

u8 AD_A_generate_data(u8 channel);
u8 AD_B_generate_data(u8 channel);
#endif
















