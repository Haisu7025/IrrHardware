#include "led.h"

void LED_GPIO_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LED(char op)
{		
		if(!op){
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		else{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
}