#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "data.h"
#include "PCF8591.h"

keeper kp = {0, 100, 0, 100, 0, 100};
curr cur = {20, 20, 20, 20, 20, 20, 20, 20};
warnstate ws = {0, 0, 0};
timing tim = {10, 20, 5};

char debug_a_c1, debug_a_c2, debug_a_c3, debug_a_c4;
char debug_b_c1, debug_b_c2, debug_b_c3, debug_b_c4;

u8 ADa_generate_data(u8 channel)
{
    //channel:0,1,2,3
    u8 AD_data;
    pcf8591a_ad_write(0x41 + channel);
    delay_us(50);
    AD_data = pcf8591a_ad_read();
    return AD_data;
}

u8 ADb_generate_data(u8 channel)
{
    //channel:0,1,2,3
    u8 AD_data;
    pcf8591b_ad_write(0x41 + channel);
    delay_us(50);
    AD_data = pcf8591b_ad_read();
    return AD_data;
}

char *byte2bit(char c)
{
    char rt[8];
    rt[0] = (char)((c >> 7) & 0x1);
    rt[1] = (char)((c >> 6) & 0x1);
    rt[2] = (char)((c >> 5) & 0x1);
    rt[3] = (char)((c >> 4) & 0x1);
    rt[4] = (char)((c >> 3) & 0x1);
    rt[5] = (char)((c >> 2) & 0x1);
    rt[6] = (char)((c >> 1) & 0x1);
    rt[7] = (char)((c >> 0) & 0x1);
    return rt;
}

char bit2byte(char *c)
{
    char rt;
    rt = (c[0] << 7) + (c[1] << 6) + (c[2] << 5) + (c[3] << 4) + (c[4] << 3) + (c[5] << 2) + (c[6] << 1) + (c[7] << 0);
    return rt;
}

void set_keeper(char tl, char th, char hl, char hh, u16 pl, u16 ph)
{
    kp.Volt_low = tl;
    kp.Volt_high = th;
    kp.Humi_low = hl;
    kp.Humi_high = hh;
    kp.pres_low = pl;
    kp.pres_high = ph;
}

void set_timing(u16 report_interval, u16 heartbeat_interval, u16 check_interval)
{
    tim.report_interval = report_interval;
    tim.heartbeat_interval = heartbeat_interval;
    tim.check_interval = check_interval;
}

void alarm()
{
    if (cur.Volt_cur < kp.Volt_low)
    {
        ws.Volt_state = -1;
    }
    else if (cur.Volt_cur > kp.Volt_high)
    {
        ws.Volt_state = 1;
    }
    if (cur.Humi_cur < kp.Humi_low)
    {
        ws.Humi_state = -1;
    }
    else if (cur.Humi_cur > kp.Humi_high)
    {
        ws.Humi_state = 1;
    }
    if (cur.Pres_cur < kp.pres_low)
    {
        ws.Pres_state = -1;
    }
    else if (cur.Pres_cur > kp.pres_high)
    {
        ws.Pres_state = 1;
    }
}

void get_sensor_data()
{
    //调试用
    debug_a_c1 = ADa_generate_data(3);
    debug_a_c2 = ADa_generate_data(2);
    debug_a_c3 = ADa_generate_data(1);
    debug_a_c4 = ADa_generate_data(0);

    debug_b_c1 = ADb_generate_data(3);
    debug_b_c2 = ADb_generate_data(2);
    debug_b_c3 = ADb_generate_data(1);
    debug_b_c4 = ADb_generate_data(0);
}

void FourChannelADRead(char AD_A_result[], char AD_B_result[])
{
    AD_A_result[0] = ADa_generate_data(0);
    AD_A_result[1] = ADa_generate_data(1);
    AD_A_result[2] = ADa_generate_data(2);
    AD_A_result[3] = ADa_generate_data(3);

    AD_B_result[0] = ADb_generate_data(0);
    AD_B_result[1] = ADb_generate_data(1);
    AD_B_result[2] = ADb_generate_data(2);
    AD_B_result[3] = ADb_generate_data(3);
}

void report_curstate(char *report)
{
    report[0] = (cur.Volt_cur);
    report[1] = (cur.Humi1);
    report[2] = (cur.Humi2);
    report[3] = (cur.Humi3);
    report[4] = (cur.Humi4);
    report[5] = (cur.Pres_cur / 256);
    report[6] = (cur.Pres_cur % 256);
    report[7] = (cur.Flow_cur / 256);
    report[8] = (cur.Flow_cur % 256);

    //流量计数器清零，只记录距离上一个上报时间间隔内的流量数值
    cur_count = 0;
}

void report_warningstate(char *report)
{
    report[0] = cur.Volt_cur;
    report[1] = cur.Humi_cur;
    report[3] = cur.Pres_cur / 256;
    report[4] = cur.Pres_cur % 256;
}
