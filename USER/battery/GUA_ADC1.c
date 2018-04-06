#include "stm32f10x.h"
#include "GUA_ADC1.h"

/*********************内部函数声明************************/
static GUA_U16 GUA_ADC1_Read(GUA_U8 nGUA_Channel);

//******************************************************************************
//name:             GUA_ADC1_Read
//introduce:        ADC1的通道数值读取
//parameter:        nGUA_Channel:ADC1的采集通道
//return:           该通道采集的数值
//author:           甜甜的大香瓜
//email:            897503845@qq.com
//QQ group          香瓜单片机之STM8/STM32(164311667)
//changetime:       2016.12.29
//******************************************************************************
static GUA_U16 GUA_ADC1_Read(GUA_U8 nGUA_Channel)
{
    //规则采样顺序值为1,采样时间为 239.5 周期
    ADC_RegularChannelConfig(ADC1, nGUA_Channel, 1, ADC_SampleTime_239Cycles5);

    //开始转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    //等待转换结束
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        ;

    //返回转换结果
    return ADC_GetConversionValue(ADC1);
}

//******************************************************************************
//name:             GUA_ADC1_ReadAverage
//introduce:        ADC1的通道数值读取
//parameter:        nGUA_Channel:ADC1的采集通道
//                                  nGUA_times:求平均值的次数
//return:           该通道采集的数值
//author:           甜甜的大香瓜
//email:            897503845@qq.com
//QQ group          香瓜单片机之STM8/STM32(164311667)
//changetime:       2016.12.29
//******************************************************************************
GUA_U16 GUA_ADC1_ReadAverage(GUA_U8 nGUA_Channel, GUA_U8 nGUA_times)
{
    GUA_U32 nGUA_Value_Sum = 0;
    GUA_U8 i;

    //累加读取到的数值
    for (i = 0; i < nGUA_times; i++)
    {

        nGUA_Value_Sum += GUA_ADC1_Read(nGUA_Channel);
    }

    //求出平均值并返回
    return nGUA_Value_Sum / nGUA_times;
}

//******************************************************************************
//name:             GUA_ADC1_Init
//introduce:        ADC1初始化
//parameter:        none
//return:           none
//author:           甜甜的大香瓜
//email:            897503845@qq.com
//QQ group          香瓜单片机之STM8/STM32(164311667)
//changetime:       2016.12.29
//******************************************************************************
void GUA_ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //开GPIOB、ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);

    //ADC最大频率不能超过14M，否则不准，因此使用72M/6=12M
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    //IO初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //ADC1配置复位
    ADC_DeInit(ADC1);

    //ADC1配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);                                 //初始化配置

    //使能
    ADC_Cmd(ADC1, ENABLE);

    //校准
    ADC_ResetCalibration(ADC1); //开启复位校准
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                       //等待复位校准结束
    ADC_StartCalibration(ADC1); //开启AD校准
    while (ADC_GetCalibrationStatus(ADC1))
        ; //等待校准结束
}