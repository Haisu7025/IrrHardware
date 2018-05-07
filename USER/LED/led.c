#include "led.h"

void LED_GPIO_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
}

//void LED1(char op)
//{		
//		if(op){
//				GPIO_ResetBits(GPIOD, GPIO_Pin_2);
//		}
//		else{
//				GPIO_SetBits(GPIOD, GPIO_Pin_2);
//		}
//}

//void LED0(char op){
//	
//}


void flash_LED0(u8 n, u16 delay){
	int i;
	for(i=0;i<n;i++){
		LED0(1);
		delay_ms(30);
		LED0(0);
		delay_ms(100);
	}
	delay_ms(delay);
}

void flash_LED1(u8 n, u16 delay){
	int i;
	for(i=0;i<n;i++){
		LED1(1);
		delay_ms(30);
		LED1(0);
		delay_ms(100);
	}
	delay_ms(delay);
}