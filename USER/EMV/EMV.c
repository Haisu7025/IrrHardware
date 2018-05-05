#include "EMV.h"

void EMV_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;         //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);            //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);                  //PB.5 输出高

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;         //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);            //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);                  //PB.5 输出高
}

void EMV_open()
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
//		delay_ms(50);
    GPIO_SetBits(GPIOC, GPIO_Pin_7);
    delay_ms(350);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

void EMV_close()
{
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
//		delay_ms(50);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);
    delay_ms(350);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}