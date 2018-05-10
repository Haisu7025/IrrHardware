#include "EMV.h"

#define EMV_C0 GPIO_Pin_8
#define EMV_C1 GPIO_Pin_9
#define EMV_Port GPIOC

void EMV_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = EMV_C0;         //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(EMV_Port, &GPIO_InitStructure);            //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(EMV_Port, EMV_C0);                  //PB.5 输出高

    GPIO_InitStructure.GPIO_Pin = EMV_C1;         //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(EMV_Port, &GPIO_InitStructure);            //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(EMV_Port, EMV_C1);                  //PB.5 输出高
}

void EMV_open()
{
    GPIO_ResetBits(EMV_Port, EMV_C0);
//		delay_ms(50);
    GPIO_SetBits(EMV_Port, EMV_C1);
    delay_ms(350);
    GPIO_ResetBits(EMV_Port, EMV_C0);
    GPIO_ResetBits(EMV_Port, EMV_C1);
}

void EMV_close()
{
    GPIO_SetBits(EMV_Port, EMV_C0);
//		delay_ms(50);
    GPIO_ResetBits(EMV_Port, EMV_C1);
    delay_ms(350);
    GPIO_ResetBits(EMV_Port, EMV_C0);
    GPIO_ResetBits(EMV_Port, EMV_C1);
}
