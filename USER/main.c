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
#include "GUA_Battery_Check.h"
#include "LED.h"

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
	char package[18];

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
	char package[15];

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
	char package[16];

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
	char package[10];

	output_package_index++;

	generate_header(warncode, header);
	report_warningstate(report);

	memcpy(package, header, 5);
	memcpy(package + 5, report, 4);

	sign_message(package, 10);

	UART_SendBytes(package, 10, 1);
	warn_ignore_flag = 1;
}

void heartbeat_report_control()
{
	unsigned char heartbeat[6];
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);								//串口初始化为115200
	PCF8591a_Init();
	PCF8591b_Init();
	EMV_init();
	GUA_Battery_Check_Init();
}

int main(void)
{
	//variable established
	char AD_A_result[4];
	char AD_B_result[4];
	char output_buffer[200];
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

	unsigned char i, t;
	char regist_success_flag = 0;
	u16 tmp_check_time;
	u16 len, len_r = 0;

	//===================初始化配置===================
	system_init();
	//================================================

	//===================配置sim模块===================
	SIM_module_init();
	delay_ms(5000);
	UART_SendBytes("AT+ENTM", 7, 0);
	//	delay_ms(5000);
	//================================================

	//===================开机注册===================
	while (module_index <= 0)
	{
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
				for (t = 0; t < len; t++)
				{
					input_buffer[t] = USART_RX_BUF[t];
				}
				USART_RX_STA = 0;
				//check success reply

				if (input_buffer[2] == 80)
				{
					if (input_buffer[3] == 80)
					{
						//if '00' is inserted at the beginning
						len--;
						for (t = 0; t < len; t++)
						{
							input_buffer[t] = input_buffer[t + 1];
						}
					}
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
		if (module_index < 0)
		{
			//若从循环跳出后仍然没有收到正确id，则重启
			SoftReset();
		}
	}

	//生成正确id对应的心跳包
	hb_content[0] = 0x00;
	hb_content[1] = 0x00;
	hb_content[2] = 0x00;
	hb_content[3] = module_index / 256;
	hb_content[4] = module_index % 256;
	sign_message(hb_content, 6);

	modify_heartbeat_content(hb_content);
	//=============================================

	//===================计时器初始化==============
	TIM3_Int_Init(10000, 7199); //10Khz的计数频率，计数到5000为500ms

	USART_RX_STA = 0;
	for (t = 0; t < 200; t++)
	{
		USART_RX_BUF[t] = 0;
	}

	//进入轮询，LED快速闪烁

	//===================轮询接收===================
	while (1)
	{
		LED(1);
		delay_ms(100);
		LED(0);

		//DEBUG USE: usart record long buffer.
		//usart_record_p=0;

		if (USART_RX_STA & 0x8000)
		{
			len = USART_RX_STA & 0x1fff; //得到此次接收到的数据长度
			for (t = 0; t < len; t++)
			{
				input_buffer[t] = USART_RX_BUF[t];
				USART_RX_BUF[t] = 0;
			}
			USART_RX_STA = 0;

			//checksum
			if (!check_sign(input_buffer, len))
			{
				USART_RX_STA = 0;
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
				break;
			case 1:
				//立即上报
				generate_report();
				delay_ms(1000);
				generate_status();
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
					// case 4:
					// 	//调整普通上报包时间间隔
					// 	set_timing(((input_buffer[5] + 256) % 256) * 256 * 10 + (input_buffer[6] + 256) * 10 % 256,
					// 			   tim.heartbeat_interval,
					// 			   tim.check_interval);

					// 	break;
					// case 8:
					// 	//调整异常检测时间
					// 	set_timing(tim.report_interval,
					// 			   tim.heartbeat_interval,
					// 			   ((input_buffer[5] + 256) % 256) * 256 + (input_buffer[6] + 256) % 256);

					// 	break;
				}
				break;
			case 4:
				//修改服务器IP、端口
				ip[0] = (input_buffer[5] + 256) % 256;
				ip[1] = (input_buffer[6] + 256) % 256;
				ip[2] = (input_buffer[7] + 256) % 256;
				ip[3] = (input_buffer[8] + 256) % 256;

				port = (input_buffer[9] + 256) % 256 * 256 + (input_buffer[10] + 256) % 256;

				//可能需要用到sim模块的AT指令
				ip_str[0] = '\"';
				ip_str[1] = ip[0];
				ip_str[3] = ip[1];
				ip_str[5] = ip[2];
				ip_str[7] = ip[3];
				ip_str[2] = ip_str[4] = ip_str[6] = '.';
				ip_str[8] = '\"';

				modify_SIM_server(ip_str, 14, port);
				break;
			case 7:
				//自动控制
				auto_control_flag = input_msg_subtype;
				break;
			case 8:
				//固件升级
				//IAP未解决
				break;
			}
			continue;
		}
	}
}

void cycle_check()
{
	//获取传感器数据
	get_sensor_data();

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
	nGUA_Battery_Check_Value = GUA_Battery_Check_Read();
	cur.Volt_cur = nGUA_Battery_Check_Value * 3.3 / 4096;

	if (0) //检测阀门是否异常
	{
		generate_warning(246);
	}
}

void TIM3_IRQHandler(void) //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIMx更新中断标志
		//中断程序
		check_flag++;
		report_flag++;

		if (check_flag == tim.check_interval)
		{
			//printf("%ds!!\n",tim.check_interval);
			cycle_check();
			check_flag = 0;
		}

		if (report_flag == tim.report_interval)
		{
			//printf("%ds!!\n",tim.report_interval);
			generate_report();
			report_flag = 0;
		}
	}
}
//检测周期