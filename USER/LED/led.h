#ifndef __LED_H
#define __LED_H
#include "sys.h"

void LED_GPIO_init(void);
//void LED(char op);


// a macro to operate the LED1
#define LED1(a)	if (a)	\
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);\
					else		\
					GPIO_SetBits(GPIOD,GPIO_Pin_2)
					
// a macro to operate the LED1
#define LED0(a)	if (a)	\
					GPIO_ResetBits(GPIOA,GPIO_Pin_8);\
					else		\
					GPIO_SetBits(GPIOA,GPIO_Pin_8)
					
#endif