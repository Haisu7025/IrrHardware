#ifndef __PCF8591_H__
#define __PCF8591_H__

#include "stm32f10x.h"
#include "iic.h"

#define pcf8591_read 0x91  //PCF8591读控制字
#define pcf8591_write 0x90 //PCF8591写控制字

void PCF8591a_Init(void);
void pcf8591a_ad_write(u8 c);
u8 pcf8591a_ad_read(void);
void pcf8591a_da_write(u8 dat);

void PCF8591b_Init(void);
void pcf8591b_ad_write(u8 c);
u8 pcf8591b_ad_read(void);
void pcf8591b_da_write(u8 dat);

#endif
