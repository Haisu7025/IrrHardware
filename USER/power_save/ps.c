#include "ps.h"
#include "usart.h"
#include "EMV.h"
#include "PCF8591.h"
#include "led.h"

void power_save_mode(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //初始化KEY1-->GPIOE.0  上拉输入

    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); //关闭串口接受中断
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);      

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXIT寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;         //使能按键WK_UP所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;        //子优先级3

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //使能时钟

    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

void wakeup(void)
{
    SystemInit();
		LED_GPIO_init();
    EXTI_DeInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    PCF8591a_Init();
    PCF8591b_Init();
    EMV_init();

    uart_init(115200); //串口初始化为115200
    USART_RX_STA = 0;
}
