#include "SIM.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"

void SIM_module_init(){
		enter_com_mode();
		delay_ms(500);
		enter_configuration_mode();
		delay_ms(500);	
		UART_SendBytes("AT+UART=115200,\"NONE\",8,1,\"NONE\"",32, 0);
		delay_ms(500);
		UART_SendBytes("AT+APN=\"cmnet\"",14, 0);
		delay_ms(500);
		UART_SendBytes("AT+HEARTDT=\"0000000000FFFE0AEF\"",31, 0);
		delay_ms(500);
		UART_SendBytes("AT+HEARTTM=29",13, 0);
		delay_ms(500);
		UART_SendBytes("AT+SOCKA=\"TCP\",\"211.159.151.145\",2317",37, 0);
		delay_ms(500);
		UART_SendBytes("AT+SOCKASL=\"long\"",17, 0);
		delay_ms(500);
		UART_SendBytes("AT+UATEN=\"on\"",13, 0);
		delay_ms(500);
		UART_SendBytes("AT+CMDPW=\"irgt\"",15, 0);
		delay_ms(500);
		UART_SendBytes("AT+S",4, 0);
}

void enter_com_mode(){
		UART_SendBytes("AT+ENTM",7, 0);
}

void enter_configuration_mode(){
		char target;
		UART_SendByte('+');
		UART_SendByte('+');
		UART_SendByte('+');
//		while (1)
//		{
//			if (USART_RX_STA & 0x8000)
//			{
//				target=USART_RX_BUF[0];
//			}
//			USART_RX_STA = 0;
//			if(target=='a')
//				break;
//		}
		delay_ms(500);
		UART_SendByte('a');
}

void modify_heartbeat_time(u16 time)
{
		u16 time_len;
    char *inst = "irgt#AT+HEARTTM=";
		char mod_pack[20];
    char time_s[5];
		if(time<10){
				time_len=1;
		}
		else if(time>=10 && time<100){
				time_len=2;
		}
		else{
				time_len=3;
		}
		sprintf(time_s,"%d",time);
		memcpy(mod_pack,inst,16);
		memcpy(mod_pack+16,time_s,time_len);
    UART_SendBytes(mod_pack,16+time_len, 0);
    delay_ms(100);
}

void modify_heartbeat_content(char* content)
{
    //注意content要加双引号
		u16 i;
		char szTmp[2];
		char control_sig[36];
    char *inst = "irgt#AT+HEARTDT=";
		memcpy(control_sig,inst,16);
		control_sig[16]='\"';
		 for( i = 0; i < 6; i++ )
    {
        sprintf( szTmp,"%02X",content[i]);
        memcpy(control_sig+17+ i*2,szTmp,2);
    }
		control_sig[29]='f';
		control_sig[30]='e';
		control_sig[31]='0';
		control_sig[32]='a';
		control_sig[33]='e';
		control_sig[34]='f';
		control_sig[35]='\"';

    UART_SendBytes(control_sig,36, 0);
		delay_ms(100);
}

void modify_SIM_server(char *ip_address, u16 ip_len, u16 port)
{
    //注意IP要加双引号
		u16 port_len;
		char package[50];
    char *inst = "irgt#AT+SOCKA=\"TCP\",";
	  char *inst1 = "irgt#AT+SOCKASL=\"long\"";
		char port_s[6];
		
		if(port>=10000){
				port_len=5;
		}
		else{
				port_len=4;
		}
		memcpy(package,inst,20);
		memcpy(package+20,ip_address,ip_len+2);
		package[20+ip_len+2]=',';
		sprintf(port_s,"%d",port);
		
    memcpy(package+20+ip_len+2+1,port_s,port_len);
		
    UART_SendBytes(package, 20+ip_len+2+1+port_len, 0);
		delay_ms(1000);
		
    UART_SendBytes(inst1, 22, 0);
		delay_ms(100);
}

void module_restart()
{
    char *inst = "irgt#AT+Z";
    UART_SendBytes(inst,9, 0);
		delay_ms(100);
}

void modify_cmnet()
{
		char *inst="irgt#AT+APN=\"cmnet\"";
		UART_SendBytes(inst,19, 0);
		delay_ms(100);
}

void save_modify()
{
		char *inst="irgt#AT+S";
		UART_SendBytes(inst,9, 0);
		delay_ms(100);
}