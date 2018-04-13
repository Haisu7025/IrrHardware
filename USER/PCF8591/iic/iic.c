
#include "iic.h"
#include "delay.h"

//初始化IIC
void IIC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7); //PB10,PB11 输出高
}

void SDA1_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA1_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//产生IIC起始信号
void IIC1_Start(void)
{
	SDA1_OUT(); //sda线输出
	SDA1_DATA(1);
	SCL1_OUT(1);
	delay_us(4);
	SDA1_DATA(0); //START:when CLK is high,DATA change form high to low
	delay_us(4);
	SCL1_OUT(0); //钳住I2C总线，准备发送或接收数据
}

//产生IIC停止信号
void IIC1_Stop(void)
{
	SDA1_OUT(); //sda线输出
	SCL1_OUT(0);
	SDA1_DATA(0); //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	SCL1_OUT(1);
	SDA1_DATA(1); //发送I2C总线结束信号
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC1_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA1_IN(); //SDA设置为输入
	SDA1_DATA(1);
	delay_us(1);
	SCL1_OUT(1);
	delay_us(1);

	while (SDA1_IO_IN)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC1_Stop();
			return 1;
		}
	}
	SCL1_OUT(0); //时钟输出0
	return 0;
}

//产生ACK应答
void IIC1_Ack(void)
{
	SCL1_OUT(0);
	SDA1_OUT();
	SDA1_DATA(0);
	delay_us(2);
	SCL1_OUT(1);
	delay_us(2);
	SCL1_OUT(0);
}

//不产生ACK应答
void IIC1_NAck(void)
{
	SCL1_OUT(0);
	SDA1_OUT();
	SDA1_DATA(1);
	delay_us(2);
	SCL1_OUT(1);
	delay_us(2);
	SCL1_OUT(0);
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC1_Send_Byte(u8 txd)
{
	u8 t;
	SDA1_OUT();
	SCL1_OUT(0); //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			SDA1_DATA(1);
		else
			SDA1_DATA(0);
		txd <<= 1;
		delay_us(2); //对TEA5767这三个延时都是必须的
		SCL1_OUT(1);
		delay_us(2);
		SCL1_OUT(0);
		delay_us(2);
	}

	SDA1_DATA(1);
	delay_us(2);
	SCL1_OUT(1);
	delay_us(2);
	SCL1_OUT(0);
	delay_us(2);
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC1_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA1_IN(); //SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		SCL1_OUT(0);
		delay_us(2);
		SCL1_OUT(1);
		receive <<= 1;
		if (SDA1_IO_IN)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC1_NAck(); //发送nACK
	else
		IIC1_Ack(); //发送ACK
	return receive;
}

//初始化IIC
void IIC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); //PB10,PB11 输出高
}

void SDA2_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA2_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//产生IIC起始信号
void IIC2_Start(void)
{
	SDA2_OUT(); //sda线输出
	SDA2_DATA(1);
	SCL2_OUT(1);
	delay_us(4);
	SDA2_DATA(0); //START:when CLK is high,DATA change form high to low
	delay_us(4);
	SCL2_OUT(0); //钳住I2C总线，准备发送或接收数据
}

//产生IIC停止信号
void IIC2_Stop(void)
{
	SDA2_OUT(); //sda线输出
	SCL2_OUT(0);
	SDA2_DATA(0); //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	SCL2_OUT(1);
	SDA2_DATA(1); //发送I2C总线结束信号
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA2_IN(); //SDA设置为输入
	SDA2_DATA(1);
	delay_us(1);
	SCL2_OUT(1);
	delay_us(1);

	while (SDA2_IO_IN)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	SCL2_OUT(0); //时钟输出0
	return 0;
}

//产生ACK应答
void IIC2_Ack(void)
{
	SCL2_OUT(0);
	SDA2_OUT();
	SDA2_DATA(0);
	delay_us(2);
	SCL2_OUT(1);
	delay_us(2);
	SCL2_OUT(0);
}

//不产生ACK应答
void IIC2_NAck(void)
{
	SCL2_OUT(0);
	SDA2_OUT();
	SDA2_DATA(1);
	delay_us(2);
	SCL2_OUT(1);
	delay_us(2);
	SCL2_OUT(0);
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC2_Send_Byte(u8 txd)
{
	u8 t;
	SDA2_OUT();
	SCL2_OUT(0); //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
			SDA2_DATA(1);
		else
			SDA2_DATA(0);
		txd <<= 1;
		delay_us(2); //对TEA5767这三个延时都是必须的
		SCL2_OUT(1);
		delay_us(2);
		SCL2_OUT(0);
		delay_us(2);
	}

	SDA2_DATA(1);
	delay_us(2);
	SCL2_OUT(1);
	delay_us(2);
	SCL2_OUT(0);
	delay_us(2);
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC2_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA2_IN(); //SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		SCL2_OUT(0);
		delay_us(2);
		SCL2_OUT(1);
		receive <<= 1;
		if (SDA2_IO_IN)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC2_NAck(); //发送nACK
	else
		IIC2_Ack(); //发送ACK
	return receive;
}
