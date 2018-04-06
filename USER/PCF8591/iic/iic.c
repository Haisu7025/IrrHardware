#include "iic.h"
#include "delay.h"

//初始化IIC
void IIC_A_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7); //PB10,PB11 输出高
}

void IIC_B_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); //PB10,PB11 输出高
}

void SDA_A_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_B_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA_A_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA_B_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//产生IIC起始信号
void IIC_A_Start(void)
{
	SDA_A_OUT(); //sda线输出
	SDA_A_DATA(1);
	SCL_A_OUT(1);
	delay_us(4);
	SDA_A_DATA(0); //START:when CLK is high,DATA change form high to low
	delay_us(4);
	SCL_A_OUT(0); //钳住I2C总线，准备发送或接收数据
}

void IIC_B_Start(void)
{
	SDA_B_OUT(); //sda线输出
	SDA_B_DATA(1);
	SCL_B_OUT(1);
	delay_us(4);
	SDA_B_DATA(0); //START:when CLK is high,DATA change form high to low
	delay_us(4);
	SCL_B_OUT(0); //钳住I2C总线，准备发送或接收数据
}

//产生IIC停止信号
void IIC_A_Stop(void)
{
	SDA_A_OUT(); //sda线输出
	SCL_A_OUT(0);
	SDA_A_DATA(0); //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	SCL_A_OUT(1);
	SDA_A_DATA(1); //发送I2C总线结束信号
	delay_us(4);
}

void IIC_B_Stop(void)
{
	SDA_B_OUT(); //sda线输出
	SCL_B_OUT(0);
	SDA_B_DATA(0); //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	SCL_B_OUT(1);
	SDA_B_DATA(1); //发送I2C总线结束信号
	delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_A_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_A_IN(); //SDA设置为输入
	SDA_A_DATA(1);
	delay_us(1);
	SCL_A_OUT(1);
	delay_us(1);

	while (SDA_A_IO_IN)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_A_Stop();
			return 1;
		}
	}
	SCL_A_OUT(0); //时钟输出0
	return 0;
}

u8 IIC_B_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_B_IN(); //SDA设置为输入
	SDA_B_DATA(1);
	delay_us(1);
	SCL_B_OUT(1);
	delay_us(1);

	while (SDA_B_IO_IN)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_B_Stop();
			return 1;
		}
	}
	SCL_B_OUT(0); //时钟输出0
	return 0;
}
//产生ACK应答
void IIC_A_Ack(void)
{
	SCL_A_OUT(0);
	SDA_A_OUT();
	SDA_A_DATA(0);
	delay_us(2);
	SCL_A_OUT(1);
	delay_us(2);
	SCL_A_OUT(0);
}

void IIC_B_Ack(void)
{
	SCL_B_OUT(0);
	SDA_B_OUT();
	SDA_B_DATA(0);
	delay_us(2);
	SCL_B_OUT(1);
	delay_us(2);
	SCL_B_OUT(0);
}

//不产生ACK应答
void IIC_A_NAck(void)
{
	SCL_A_OUT(0);
	SDA_A_OUT();
	SDA_A_DATA(1);
	delay_us(2);
	SCL_A_OUT(1);
	delay_us(2);
	SCL_A_OUT(0);
}

void IIC_B_NAck(void)
{
	SCL_B_OUT(0);
	SDA_B_OUT();
	SDA_B_DATA(1);
	delay_us(2);
	SCL_B_OUT(1);
	delay_us(2);
	SCL_B_OUT(0);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_A_Send_Byte(u8 txd)
{
	u8 t;
	SDA_A_OUT();
	SCL_A_OUT(0); //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			SDA_A_DATA(1);
		else
			SDA_A_DATA(0);
		txd <<= 1;
		delay_us(2); //对TEA5767这三个延时都是必须的
		SCL_A_OUT(1);
		delay_us(2);
		SCL_A_OUT(0);
		delay_us(2);
	}
	// SDA_A_DATA(1);
	// delay_us(2);
	// SCL_A_OUT(1);
	// delay_us(2);
	// SCL_A_OUT(0);
	// delay_us(2);
}

void IIC_B_Send_Byte(u8 txd)
{
	u8 t;
	SDA_B_OUT();
	SCL_B_OUT(0); //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			SDA_B_DATA(1);
		else
			SDA_B_DATA(0);
		txd <<= 1;
		delay_us(2); //对TEA5767这三个延时都是必须的
		SCL_B_OUT(1);
		delay_us(2);
		SCL_B_OUT(0);
		delay_us(2);
	}
	// SDA_A_DATA(1);
	// delay_us(2);
	// SCL_A_OUT(1);
	// delay_us(2);
	// SCL_A_OUT(0);
	// delay_us(2);
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_A_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA_A_IN(); //SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		SCL_A_OUT(0);
		delay_us(2);
		SCL_A_OUT(1);
		receive <<= 1;
		if (SDA_A_IO_IN)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_A_NAck(); //发送nACK
	else
		IIC_A_Ack(); //发送ACK
	return receive;
}

u8 IIC_B_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA_B_IN(); //SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		SCL_B_OUT(0);
		delay_us(2);
		SCL_B_OUT(1);
		receive <<= 1;
		if (SDA_B_IO_IN)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_B_NAck(); //发送nACK
	else
		IIC_B_Ack(); //发送ACK
	return receive;
}
