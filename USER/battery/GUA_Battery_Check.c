#include "stm32f10x.h"
#include "GUA_ADC1.h"
#include "GUA_Battery_Check.h"

/*********************宏定义************************/
#define GUA_BATTERY_CHECK_COUNT_PER_COLLECT 5 //每次采集的次数

//******************************************************************************
//name:             GUA_Battery_Check_Read
//introduce:        读电量
//parameter:        none
//return:           返回16bit的电量，右对齐
//author:           甜甜的大香瓜
//email:            897503845@qq.com
//QQ group          香瓜单片机之STM8/STM32(164311667)
//changetime:       2016.12.28
//******************************************************************************
GUA_U16 GUA_Battery_Check_Read(void)
{
    GUA_U16 nGUA_Battery_Check_Value = 0;
    GUA_U16 nGUA_Num = 0;

    //采集BATTERY_CHECK_COUNT_PER_COLLECT次，累加采集的电量值
    for (nGUA_Num = 0; nGUA_Num < GUA_BATTERY_CHECK_COUNT_PER_COLLECT; nGUA_Num++)
    {
        //累加读取adc的转换值
        nGUA_Battery_Check_Value += GUA_ADC1_ReadAverage(ADC_Channel_9, 1);
    }

    //求出电量平均值
    nGUA_Battery_Check_Value /= GUA_BATTERY_CHECK_COUNT_PER_COLLECT;

    //返回
    return nGUA_Battery_Check_Value;
}

//******************************************************************************
//name:             GUA_Battery_Check_Init
//introduce:        电量检测初始化
//parameter:        none
//return:           none
//author:           甜甜的大香瓜
//email:            897503845@qq.com
//QQ group          香瓜单片机之STM8/STM32(164311667)
//changetime:       2016.12.28
//******************************************************************************
void GUA_Battery_Check_Init(void)
{
    //ADC1初始化
    GUA_ADC1_Init();
}