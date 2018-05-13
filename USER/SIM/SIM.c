#include "SIM.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"

void SIM_module_init()
{
	link_gpio_init();
	reset_gpio_init();
	reload_gpio_init();
	
	reload_SIM();

	enter_com_mode();
	delay_ms(500);
	enter_configuration_mode();
	delay_ms(500);
	//UART_SendBytes("AT+E=\"off\"", 10, 0);
	//delay_ms(500);
	UART_SendBytes("AT+UART=115200,\"NONE\",8,1,\"NONE\"", 32, 0);
	delay_ms(500);
	UART_SendBytes("AT+APN=\"cmnet\"", 14, 0);
	delay_ms(500);
	UART_SendBytes("AT+HEARTDT=\"0000000000FFFE0AEF\"", 31, 0);
	delay_ms(500);
	UART_SendBytes("AT+HEARTTM=30", 13, 0);
	delay_ms(500);
	UART_SendBytes("AT+SOCKA=\"TCP\",\"123.206.21.240\",2318", 36, 0);
	delay_ms(500);
	UART_SendBytes("AT+SOCKASL=\"long\"", 17, 0);
	delay_ms(500);
	UART_SendBytes("AT+UATEN=\"on\"", 13, 0);
	delay_ms(500);
	UART_SendBytes("AT+CMDPW=\"irgt\"", 15, 0);
	delay_ms(500);
	UART_SendBytes("AT+STMSG=\"\"", 11, 0);
	delay_ms(500);
	UART_SendBytes("AT+SLEEPTIM=0", 13, 0);
	delay_ms(500);
	UART_SendBytes("AT+S", 4, 0);

	delay_ms(5000);
	while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12))
	{
		delay_ms(1000);
	}
}

void link_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			  //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);				  //根据设定参数初始化GPIOB.5
}

void reset_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		  //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOC, GPIO_Pin_11);
}

void reload_gpio_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOC, GPIO_Pin_10);
}

void reload_SIM(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	delay_ms(8000);
	GPIO_SetBits(GPIOC, GPIO_Pin_10);
	delay_ms(8000);
}

void soft_reset_SIM(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	delay_ms(100);
	GPIO_SetBits(GPIOC, GPIO_Pin_11);
	delay_ms(1000);

	//wait for reconnection
	while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12))
	{
		delay_ms(1000);
	}
}

void enter_com_mode()
{
	UART_SendBytes("AT+ENTM", 7, 0);
}

void enter_configuration_mode()
{
	char target;
	UART_SendByte('+');
	UART_SendByte('+');
	UART_SendByte('+');

	delay_ms(500);
	UART_SendByte('a');
}

void enter_sleep_mode(void)
{
	UART_SendBytes("irgt#AT+SLEEPTIM=20", 19, 0);
	delay_ms(500);
	UART_SendBytes("irgt#AT+S", 9, 0);
	delay_ms(500);
}

void modify_heartbeat_time(u16 time)
{
	u16 time_len;
	char *inst = "irgt#AT+HEARTTM=";
	char mod_pack[20];
	char time_s[5];
	if (time < 10)
	{
		time_len = 1;
	}
	else if (time >= 10 && time < 100)
	{
		time_len = 2;
	}
	else
	{
		time_len = 3;
	}
	sprintf(time_s, "%d", time);
	memcpy(mod_pack, inst, 16);
	memcpy(mod_pack + 16, time_s, time_len);
	UART_SendBytes(mod_pack, 16 + time_len, 0);
	delay_ms(500);
	UART_SendBytes("irgt#AT+S", 9, 0);
	delay_ms(500);
}

void init_hbid_and_slptim(char *content)
{
	//注意content要加双引号
	u16 i;
	char szTmp[2];
	char control_sig[36];
	char *inst = "irgt#AT+HEARTDT=";
	memcpy(control_sig, inst, 16);
	control_sig[16] = '\"';
	for (i = 0; i < 6; i++)
	{
		sprintf(szTmp, "%02X", content[i]);
		memcpy(control_sig + 17 + i * 2, szTmp, 2);
	}
	control_sig[29] = 'f';
	control_sig[30] = 'e';
	control_sig[31] = '0';
	control_sig[32] = 'a';
	control_sig[33] = 'e';
	control_sig[34] = 'f';
	control_sig[35] = '\"';

	UART_SendBytes(control_sig, 36, 0);
	delay_ms(500);

	enter_sleep_mode();
}
void modify_heartbeat_content(char *content)
{
	//注意content要加双引号
	u16 i;
	char szTmp[2];
	char control_sig[36];
	char *inst = "irgt#AT+HEARTDT=";
	memcpy(control_sig, inst, 16);
	control_sig[16] = '\"';
	for (i = 0; i < 6; i++)
	{
		sprintf(szTmp, "%02X", content[i]);
		memcpy(control_sig + 17 + i * 2, szTmp, 2);
	}
	control_sig[29] = 'f';
	control_sig[30] = 'e';
	control_sig[31] = '0';
	control_sig[32] = 'a';
	control_sig[33] = 'e';
	control_sig[34] = 'f';
	control_sig[35] = '\"';

	UART_SendBytes(control_sig, 36, 0);
	delay_ms(500);
	UART_SendBytes("irgt#AT+S", 9, 0);
	delay_ms(500);
}

void modify_SIM_server(char *ip_address, u16 ip_len, u16 port)
{
	//注意IP要加双引号
	u16 port_len;
	char package[50];
	char *inst = "irgt#AT+SOCKA=\"TCP\",";
	char *inst1 = "irgt#AT+SOCKASL=\"long\"";
	char port_s[6];

	if (port >= 10000)
	{
		port_len = 5;
	}
	else
	{
		port_len = 4;
	}
	memcpy(package, inst, 20);
	memcpy(package + 20, ip_address, ip_len + 2);
	package[20 + ip_len + 2] = ',';
	sprintf(port_s, "%d", port);

	memcpy(package + 20 + ip_len + 2 + 1, port_s, port_len);

	UART_SendBytes(package, 20 + ip_len + 2 + 1 + port_len, 0);
	delay_ms(1000);

	UART_SendBytes(inst1, 22, 0);
	delay_ms(100);
}

void module_restart()
{
	char *inst = "irgt#AT+Z";
	UART_SendBytes(inst, 9, 0);
	delay_ms(100);
}

void modify_cmnet()
{
	char *inst = "irgt#AT+APN=\"cmnet\"";
	UART_SendBytes(inst, 19, 0);
	delay_ms(100);
}

void save_modify()
{
	char *inst = "irgt#AT+S";
	UART_SendBytes(inst, 9, 0);
	delay_ms(100);
}
