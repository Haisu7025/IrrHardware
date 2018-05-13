#include "delay.h"
#include "sys.h"
#include "string.h"
#include "usart.h"
#include "PCF8591.h"
#include "data.h"
#include "SIM.h"
#include "timer.h"
#include "EMV.h"
#include "Verify_Helper.h"
#include "led.h"
#include "ps.h"

//状态
u32 report_flag;
u32 check_flag;
u32 warn_ignore_flag;
u16 auto_control_flag;

//数据包序号
u32 input_package_index;
u32 output_package_index;
u32 module_index;

//计数控制与数据
u16 count_flag;
u32 pre_count;
u32 cur_count;

//电池检测
u16 nGUA_Battery_Check_Value;

//AD检测
char ADa_result[4];
char ADb_result[4];

void GetIDAdd_12byte(char *id)
{
	//获取CPU唯一ID
	memcpy(id, (char *)(0x1ffff7e8), 12);
}

void SoftReset(void)
{
	__set_FAULTMASK(1); // 关闭所有中断
	NVIC_SystemReset(); // 复位
}

void control_EMV(char KEY)
{
	if (KEY == 0)
	{
		//关
		EMV_close();
	}
	if (KEY == 1)
	{
		//开
		EMV_open();
	}
}

void generate_header(char opcode, char *header)
{
	header[0] = (output_package_index * 4 / 256);
	header[1] = (output_package_index * 4 % 256);
	header[2] = (opcode); //rand=0
	header[3] = (module_index / 256);
	header[4] = (module_index % 256);
}

void generate_regist()
{
	char header[5];
	char STM_ID[12];
	unsigned char package[18];

	output_package_index++;

	GetIDAdd_12byte(STM_ID);

	generate_header(64, header);
	memcpy(package, header, 5);
	memcpy(package + 5, STM_ID, 12);

	sign_message(package, 18);

	UART_SendBytes(package, 18, 1);
}

void generate_report()
{
	char header[5];
	char report[9];
	unsigned char package[15];

	output_package_index++;

	generate_header(16, header);
	report_curstate(report);

	memcpy(package, header, 5);
	memcpy(package + 5, report, 10);

	sign_message(package, 15);

	UART_SendBytes(package, 15, 1);
}

void generate_status()
{
	char header[5];
	unsigned char package[16];

	output_package_index++;

	generate_header(48, header);
	memcpy(package, header, 5);
	package[5] = kp.Humi_low;
	package[6] = kp.Humi_high;
	package[7] = kp.Humi_low;
	package[8] = kp.Humi_high;
	package[9] = tim.heartbeat_interval / 256;
	package[10] = tim.heartbeat_interval % 256;
	package[11] = tim.report_interval / 10 / 256;
	package[12] = tim.report_interval / 10 % 256;
	package[13] = tim.check_interval / 256;
	package[14] = tim.check_interval % 256;

	sign_message(package, 16);

	UART_SendBytes(package, 16, 1);
}

void generate_warning(char warncode)
{
	char header[5];
	char report[4];
	unsigned char package[10];

	output_package_index++;

	generate_header(warncode, header);
	report_warningstate(report);

	memcpy(package, header, 5);
	memcpy(package + 5, report, 4);

	sign_message(package, 10);

	UART_SendBytes(package, 10, 1);
	warn_ignore_flag = 1;
}

void generate_ack(char if_succeed, u16 last_package_index)
{
	char header[5];
	char last_package_index_bytes[2];
	unsigned char package[8];

	//soft_reset_SIM();

	output_package_index++;

	if (if_succeed)
	{
		generate_header(32, header);
	}
	else
	{
		generate_header(47, header);
	}

	last_package_index_bytes[0] = last_package_index / 256;
	last_package_index_bytes[1] = last_package_index % 256;

	memcpy(package, header, 5);
	memcpy(package + 5, last_package_index_bytes, 2);

	sign_message(package, 8);

	UART_SendBytes(package, 8, 1);
}

void generate_wakeup_report()
{
	char header[5];
	unsigned char package[6];

	output_package_index++;

	generate_header(96, header);

	memcpy(package, header, 5);

	sign_message(package, 6);

	UART_SendBytes(package, 6, 1);
}

void generate_reconnect_report(void)
{
	char header[5];
	unsigned char package[6];

	output_package_index++;

	generate_header(112, header);

	memcpy(package, header, 5);

	sign_message(package, 6);

	UART_SendBytes(package, 6, 1);
}

void generate_id_ack(void)
{
	char header[5];
	unsigned char package[6];

	//soft_reset_SIM();

	output_package_index++;

	generate_header(80, header);

	memcpy(package, header, 5);

	sign_message(package, 6);

	UART_SendBytes(package, 6, 1);
}

void heartbeat_report_control()
{
	char heartbeat[6];
	heartbeat[0] = '0';
	heartbeat[1] = '0';
	heartbeat[2] = '0';
	heartbeat[3] = module_index / 256;
	heartbeat[4] = module_index % 256;
	sign_message(heartbeat, 6);

	modify_heartbeat_content(heartbeat);
}

void system_init()
{
	nGUA_Battery_Check_Value = 0;

	report_flag = 0;
	check_flag = 0;
	warn_ignore_flag = 0;

	input_package_index = 0;
	output_package_index = 0;
	module_index = 0;
	auto_control_flag = 0;

	count_flag = 0;
	pre_count = 0;
	cur_count = 0;

	//delay_init();									//延时函数初始化
	LED_GPIO_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);								//串口初始化为115200
	PCF8591a_Init();
	PCF8591b_Init();
	EMV_init();
	//GUA_Battery_Check_Init();
}

void cycle_check()
{
	//获取传感器数据
	//get_sensor_data();
	FourChannelADRead(ADa_result, ADb_result);

	/*暂定数据格式为：
	* ADa:ch0,ch1,ch2,ch3 - 四路湿度值
	* ADb:ch0 - 电池电压
	* ADb:ch1 - 水压值
	* ADb:ch2,ch3 - 闲置
	*/
	cur.Humi1 = ADa_result[0];
	cur.Humi2 = ADa_result[1];
	cur.Humi3 = ADa_result[2];
	cur.Humi4 = ADa_result[3];
	cur.Humi_cur = (cur.Humi1 + cur.Humi2 + cur.Humi3 + cur.Humi4) / 4;
	cur.Volt_cur = ADb_result[0];
	cur.Pres_cur = ADb_result[1];

	//自动控制
	// if (auto_control_flag)
	// {
	// 	if (cur.Humi_cur < kp.Humi_low)
	// 	{
	// 		EMV_open();
	// 	}
	// 	else if (cur.Humi_cur > kp.Humi_high)
	// 	{
	// 		EMV_close();
	// 	}
	// }

	//获取报警数据
	alarm();

	//判断是否需要报警
	switch (ws.Volt_state)
	{
	case -1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(243);
		}
		break;
	case 0:
		warn_ignore_flag = 0;
		break;
	case 1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(242);
		}
		break;
	}

	switch (ws.Humi_state)
	{
	case -1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(241);
		}
		break;
	case 0:
		warn_ignore_flag = 0;
		break;
	case 1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(240);
		}
		break;
	}

	switch (ws.Pres_state)
	{
	case -1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(245);
		}
		break;
	case 0:

		warn_ignore_flag = 0;
		break;
	case 1:
		if (warn_ignore_flag == 0)
		{
			generate_warning(244);
		}
		break;
	}

	//获取电池状态
	//nGUA_Battery_Check_Value = GUA_Battery_Check_Read();
	//cur.Volt_cur = nGUA_Battery_Check_Value * 3.3 / 4096;

	if (0) //检测阀门是否异常
	{
		generate_warning(246);
	}
}

int main(void)
{
	//variable established
	unsigned char input_buffer[200];

	char input_msg_type;
	char input_msg_subtype;
	char operation;
	char ip[4];
	char ip_str[9];
	u16 port;
	char port_str[5];

	char input_package_index_high;
	char input_package_index_low;
	char output_package_index_high;
	char output_package_index_low;

	char module_index_high;
	char module_index_low;

	char checksum;
	char ACK_state;

	char STM_ID[12];
	char hb_content[6];

	unsigned char i, j, t;

	u16 tmp_check_time;
	u16 len = 0;
	u16 start_bit = -1;

	char cycle_times = 0;

	//usart_record_p=0;

	//===================初始化配置===================
	system_init();

	//等待网络模块连接服务器
	//	while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12))
	//	{
	//		delay_ms(1000);
	//	}
	//	//等待服务器配置网络模块使其退出睡眠状态
	//	delay_ms(5000);

	//	//通知服务器需要关闭睡眠
	//	flash_LED0(5,300);
	//	generate_reconnect_report();
	//================================================

	//===================配置sim模块===================
	LED1(1);
	LED0(1);
	SIM_module_init();

	//等待服务器
	delay_ms(5000);

	UART_SendBytes("AT+ENTM", 7, 0);
	//	delay_ms(5000 );
	//================================================

	//===================开机注册===================
	LED1(0);
	while (module_index <= 0)
	{
		flash_LED1(3, 300);
		generate_regist();
		USART_RX_STA = 0;
		i = 0;
		while (i < 10)
		{
			i++;
			delay_ms(3000);
			if (USART_RX_STA & 0x8000)
			{
				len = USART_RX_STA & 0x1fff; //得到此次接收到的数据长度

				for (t = 0; t < len - 2; t++)
				{
					if (USART_RX_BUF[t] == 0xef)
					{
						if (USART_RX_BUF[t + 1] == 0x0a)
						{
							if (USART_RX_BUF[t + 2] == 0xfe)
							{
								start_bit = t + 3;
							}
						}
					}
				}

				if (start_bit < 0)
				{
					continue;
				}

				j = 0;
				for (t = start_bit; t < len; t++)
				{
					input_buffer[j++] = USART_RX_BUF[t];
				}
				USART_RX_STA = 0;

				if (input_buffer[2] == 80)
				{
					if (!check_sign(input_buffer, 6))
					{
						USART_RX_STA = 0;
						continue;
					}
					module_index = input_buffer[3] * 256 + input_buffer[4];
					break;
				}
			}
		}
		//		if (module_index == 0)
		//		{
		//			//若从循环跳出后仍然没有收到正确id，则重启
		//			SoftReset();
		//		}
	}

	//生成正确id对应的心跳包
	hb_content[0] = 0x00;
	hb_content[1] = 0x00;
	hb_content[2] = 0x00;
	hb_content[3] = module_index / 256;
	hb_content[4] = module_index % 256;
	sign_message(hb_content, 6);

	init_hbid_and_slptim(hb_content);

	//等待上线
	LED1(0);
	delay_ms(5000);
	while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12))
	{
		delay_ms(1000);
	}
	generate_id_ack();

	USART_RX_STA = 0;
	for (t = 0; t < 200; t++)
	{
		USART_RX_BUF[t] = 0;
	}


	LED0(0);
	//进入轮询，LED快速闪烁
	//===================轮询接收===================
	while (1)
	{
		flash_LED0(2, 100);
		delay_ms(100);

		if ((++cycle_times) > 50)
		{
			cycle_times = 0;
			power_save_mode();
		}

		if (USART_RX_STA & 0x8000)
		{
			len = USART_RX_STA & 0x1fff; //得到此次接收到的数据长度

			for (t = 0; t < len - 2; t++)
			{
				if (USART_RX_BUF[t] == 0xef)
				{
					if (USART_RX_BUF[t + 1] == 0x0a)
					{
						if (USART_RX_BUF[t + 2] == 0xfe)
						{
							start_bit = t + 3;
						}
					}
				}
			}
			if (start_bit < 0)
			{
				continue;
			}

			j = 0;
			for (t = start_bit; t < len; t++)
			{
				input_buffer[j++] = USART_RX_BUF[t];
			}
			USART_RX_STA = 0;

			//checksum
			if (!check_sign(input_buffer, len - start_bit))
			{
				USART_RX_STA = 0;
				generate_ack(0, 0);
				continue;
			}
			//

			//parse messsage
			input_package_index = (((input_buffer[0] + 256) % 256) * 256 + (input_buffer[1] + 256) % 256);
			input_msg_type = (input_buffer[2] >> 4);
			input_msg_subtype = (input_buffer[2] % 16);

			switch (input_msg_type)
			{
			case 0:
				//控制指令
				operation = input_buffer[2];
				control_EMV(operation);
				USART_RX_STA = 0;
				generate_ack(1, input_package_index);
				break;
			case 1:
				//立即上报
				cycle_check();
				generate_report();
				delay_ms(1000);
				USART_RX_STA = 0;
				break;
			case 2:
				//反馈信息
				//暂定为只检测异常ACK
				switch (ACK_state)
				{
				case 0:
					//成功

					break;
				case 1:
					//失败

					break;
				}
				USART_RX_STA = 0;
				break;
			case 3:
				//系统设置
				switch (input_msg_subtype)
				{
				case 0:
					//修改温湿度阈值
					set_keeper(kp.Volt_low, kp.Volt_high,
							   (input_buffer[5] + 256) % 256,
							   (input_buffer[6] + 256) % 256,
							   kp.pres_low, kp.pres_high);
					break;
				case 1:
					//修改水压阈值
					set_keeper(kp.Volt_low, kp.Volt_high,
							   kp.Humi_low, kp.Humi_high,
							   (input_buffer[5] + 256) % 256,
							   (input_buffer[6] + 256) % 256);
					break;
				case 2:
					//调整IP注册包时间间隔
					tmp_check_time = ((input_buffer[5] + 256) % 256) * 256 + (input_buffer[6] + 256) % 256;
					set_timing(tim.report_interval,
							   tmp_check_time,
							   tim.check_interval);
					modify_heartbeat_time(tmp_check_time);
					break;
				}
				USART_RX_STA = 0;
				break;
			}
			continue;
		}
	}
}

void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);
	wakeup();
	generate_wakeup_report();

	USART_RX_STA = 0;
	EXTI_ClearITPendingBit(EXTI_Line10); //清除LINE0上的中断标志位
}
