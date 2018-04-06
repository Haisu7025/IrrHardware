/******************** (C) COPYRIGHT 2013 CEPARK多功能开发学习板********************
 * 文件名  ：delay.c
 * 描述    ：delay 应用函数库
 *          
 * 实验平台：CEPARK多功能开发学习板
																				 
**********************************************************************************/

#include "delay.h"

//微秒
void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;   
      while(i--) ;    
   }
}


//毫秒
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}

//延时函数
void delay(u16 m)
{
	while(m--);
}


/******************* (C) COPYRIGHT 2013 CEPARK多功能开发学习板*****END OF FILE****/
