#include "sys.h"
#include "usart.h"
#include "data.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos 使用
#endif
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//////////////////////////////////////////////////////////////////////////////////

//unsigned char usart_record[1500];
//u16 usart_record_p;

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0)
        ; //循环发送,直到发送完毕
    USART1->DR = (u8)ch;
    return ch;
}
#endif

/*使用microLib的方法*/
/* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

#if EN_USART1_RX //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; //接收状态标记

void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化GPIOA.9

    //USART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA.10

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                                     //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(USART1, &USART_InitStructure);      //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(USART1, ENABLE);                     //使能串口1
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
    u8 res;
#if SYSTEM_SUPPORT_OS //如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntEnter();
#endif
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
    {
        res = USART_ReceiveData(USART1); //读取接收到的数据
                                         //   FE ->0A ->EF ->Rdy
                                         //000->010->011->100
                                         //0x00>0x40>0x60>0x80

        //usart_record[usart_record_p++]=res;
        switch (USART_RX_STA & 0xE000)
        {            //高3位的状态:
        case 0x0000: //还没收到FE
            if ((USART_RX_STA & 0x1FFF) > (USART_REC_LEN - 1))
            { //如果长度异常，则清空消息
                //printf("Long!\r\n");
                USART_RX_STA = 0;
                return;
            }
            if (res == 0xFE)
            {
                USART_RX_STA |= 0x4000; //010...
            }
            break;
        case 0x4000: //刚收到FE，还没收到0A
            if (res == 0x0A)
            {                           //如果下一个是0A，则继续标记
                USART_RX_STA |= 0x2000; //011...
            }
            else if (res == 0xFE)
            { //如果下一个仍为FE, 则保持40状态不变
                //Do nothing
            }
            else
            {                           //如果下一个不是0A，证明是尚为普通数据，将标记清空
                USART_RX_STA &= 0x1FFF; //000...
            }
            break;
        case 0x6000: //刚收到0A，还没收到EF
            if (res == 0xEF)
            {                           //如果下一个是EF，则做出完成标记
                USART_RX_STA -= 2;      //将之前的标记的长度减去
                USART_RX_STA &= 0x1FFF; //000...
                USART_RX_STA |= 0x8000; //100...
                return;
            }
            else
            {                           //如果下一个不是EF，证明是尚为普通数据，将标记清空
                USART_RX_STA &= 0x1FFF; //000...
            }
            break;
        case 0x8000:
            return; //如果已经接到一帧数据，还未作出处理，则拒绝接受byte
        }
        //在确定接受完成之前
        USART_RX_BUF[USART_RX_STA & 0X1FFF] = res;
        USART_RX_STA++;
    }
#if SYSTEM_SUPPORT_OS //如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntExit();
#endif
}

void UART_ClearSend(char target)
{
    if (target == 0)
    {
        UART_SendByte(0x0d);
        UART_SendByte(0x0a);
        return;
    }
    else if (target == 1)
    {
        UART_SendByte(0xfe);
        UART_SendByte(0x0a);
        UART_SendByte(0xef);
        return;
    }
}
void UART_SendByte(unsigned char c)
{
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
        ; //等待发送结束
    delay_us(30);
}

void UART_SendBytes(unsigned char *c, u16 n, char target)
{
    u16 i = 0;
    for (i = 0; i < n; i++)
    {
        USART_SendData(USART1, *(c + i));
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
            ; //等待发送结束
        delay_us(30);
    }
    if (target == 0)
    {
        UART_SendByte(0x0d);
        UART_SendByte(0x0a);
        return;
    }
    else if (target == 1)
    {
        UART_SendByte(0xfe);
        UART_SendByte(0x0a);
        UART_SendByte(0xef);
        return;
    }
}

void UART_sendWelcome()
{
    unsigned char str[] = "System Start...\n";
    UART_SendBytes(str, sizeof(str), 1);
    USART_RX_STA = 0;
}
#endif
