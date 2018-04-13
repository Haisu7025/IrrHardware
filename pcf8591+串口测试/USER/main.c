#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "PCF8591.h"

/************************************************
 ALIENTEK战舰STM32开发板实验4
 串口实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

void USART1_SEND(char c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
		; //等待发送结束
	delay_ms(50);
}

u8 ADa_generate_data(u8 channel){
	//channel:0,1,2,3
	u8 AD_data;
	pcf8591a_ad_write(0x41+channel);
	delay_us(50);
	AD_data=pcf8591a_ad_read();
	return AD_data;
}

u8 ADb_generate_data(u8 channel){
	//channel:0,1,2,3
	u8 AD_data;
	pcf8591b_ad_write(0x41+channel);
	delay_us(50);
	AD_data=pcf8591b_ad_read();
	return AD_data;
}

int main(void)
{
	u8 pcf8591_adchannel = 0;
	u8 i = 0;
	u8 ad_temp = 0, ad_result1, ad_result2, ad_result3, ad_result4;
	u16 ad_result = 0;

	u16 t;
	u16 len;
	u16 times = 0;
	delay_init();									//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);								//串口初始化为115200

	PCF8591a_Init();
	PCF8591b_Init();

	while (1)
	{
//		switch (i++)
//		{
//		case 0:
//			pcf8591a_ad_write(0x41); //Specify channel to ch0
//			delay_us(50);
//			break;
//		case 1:
//			pcf8591a_ad_write(0x42); //Specify channel to ch1
//			delay_us(50);
//			break;
//		case 2:
//			pcf8591a_ad_write(0x43); //Specify channel to ch2
//			delay_us(50);
//			break;
//		case 3:
//			pcf8591a_ad_write(0x44); //Specify channel to ch3
//			delay_us(50);
//			break;
//		}

		//ad_temp = pcf8591a_ad_read();
		ad_temp = ADa_generate_data(i);
		ad_result = (ad_temp * 5000) / 256;
		ad_result = ad_result % 9999;
		ad_result1 = ad_result / 1000;
		ad_result2 = ad_result % 1000 / 100;
		ad_result3 = ad_result % 100 / 10;
		ad_result4 = ad_result % 10;
		
		USART_SendData(USART1, 'A');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, 'c');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, 'h');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, i + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ':');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束

		USART_SendData(USART1, ad_result1 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result2 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result3 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result4 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ' ');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		
		ad_temp = ADb_generate_data(i);
		ad_result = (ad_temp * 5000) / 256;
		ad_result = ad_result % 9999;
		ad_result1 = ad_result / 1000;
		ad_result2 = ad_result % 1000 / 100;
		ad_result3 = ad_result % 100 / 10;
		ad_result4 = ad_result % 10;
		
		USART_SendData(USART1, 'B');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, 'c');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, 'h');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, i + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ':');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束

		USART_SendData(USART1, ad_result1 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result2 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result3 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ad_result4 + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		USART_SendData(USART1, ' ');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
			; //等待发送结束
		
		
				USART_SendData(USART1, '\n');
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
				;
			
		if (i++ == 4)
		{
			USART_SendData(USART1, '\n');
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
				;
			USART_SendData(USART1, '\n');
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET)
				;
		} //等待发送结束
		delay_ms(1000);
		i = i % 4;
	}
}
