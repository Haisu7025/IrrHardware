#ifndef __DATA_H
#define __DATA_H
#include "sys.h"

typedef struct
{
    char Volt_low, Volt_high;
    char Humi_low, Humi_high;
    u16 pres_low, pres_high;
} keeper; //store the threshold of parameters

typedef struct
{
    char Volt, Current, Power;
    char Volt_high, Volt_low;
    char Current_high, Current_low;
    char Power_high, Power_low;
} battery_unit; //store the battery info and thresholds

typedef struct
{
    char Volt_cur;
    char Humi_cur;
    char Humi1, Humi2, Humi3, Humi4;
    u16 Pres_cur;
    u16 Flow_cur;
} curr; //store the current parameters

typedef struct
{
    char Volt_state;
    char Humi_state;
    char Pres_state;
} warnstate;

typedef struct
{
    u16 report_interval;
    u16 heartbeat_interval;
    u16 check_interval;
} timing; //store the interval time set by remote server

extern keeper kp;
extern curr cur;
extern warnstate ws;
extern timing tim;

extern u32 pre_count;
extern u32 cur_count;

//处理SIM通讯模块接受到的信息
char process_SIM_msg(unsigned char *msg);

//处理AD模块接收到的信息
char process_AD_msg(unsigned char *msg);

//设置内部触发阈值
void set_keeper(char tl, char th, char hl, char hh, u16 pl, u16 ph);

//设置时间间隔
void set_timing(u16 report_interval, u16 heartbeat_interval, u16 check_interval);

//检测是否报警
void alarm(void);

//获取传感器数据
void get_sensor_data(void);

//四通道数模转换数据统一获取
void FourChannelADRead(char AD_A_result[], char AD_B_result[]);

//更新report数据
void report_curstate(char *report);

#endif
