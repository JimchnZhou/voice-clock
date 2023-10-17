#include "stc89.h"
#include "dht11.h"
#include "delay.h"
#include <intrins.h>

// #define DHT11_IO P35
sbit DHT11_IO = P2 ^ 4;
// 移植注意：不同型号的单片机，需要修改对应的延时函数。 DHT11的数据接口定义为你自己的接口。

unsigned char Humi_H, Humi_L, Temp_H, Temp_L, CheckSum;
unsigned char humiHigh, humiLow, tempHigh, tempLow;

// static void Delay1us(unsigned int a) //@22.1184MHz
// {
// 	unsigned char i;
// 	unsigned int b = 0;
// 	for (b = 0; b < a; b++)
// 	{
// 		i = 1;
// 		while (--i)
// 			;
// 	}
// }

// static void Delay1ms(unsigned int a) //@22.1184MHz
// {
// 	unsigned char i, j;
// 	unsigned int b = 0;
// 	for (b = 0; b < a; b++)
// 	{
// 		_nop_();
// 		_nop_();
// 		i = 22;
// 		j = 128;
// 		do
// 		{
// 			while (--j)
// 				;
// 		} while (--i);
// 	}
// }

static unsigned char recive_byte(void) // 一个字节接收函数
{

	unsigned char i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		while (DHT11_IO)
			; // 等待DHT11拉低.输出的数据总是从低电平开始的
		while (DHT11_IO == 0)
			; // 等待DHT11拉高
		Delay_us(40);
		dat = dat << 1;
		if (DHT11_IO) // 27us后还是1，则表示数据1
		{
			dat = dat + 1;
		}
	}
	return dat; // 将接受到的数据返回
}
void Read_DHT11(void)
{
	unsigned char Check_Res = 0;

	// Delay1ms(5000);//调试用，将此函数放入主循环，实现5S执行一次

	// 发送起始信号
	DHT11_IO = 0;
	Delay_ms(25);
	DHT11_IO = 1;
	while (DHT11_IO)
		; // 这里是等待DHT11将总线拉低
	while (DHT11_IO == 0)
		; // 等待DHT11拉高总线
	// 开始接收数据
	Humi_H = recive_byte();
	Humi_L = recive_byte();
	Temp_H = recive_byte();
	Temp_L = recive_byte();
	CheckSum = recive_byte();
	// 接收 DHT11 输出的结束信号
	Delay1us(60); // 延迟后，总线会是高电平。实际上这个高电平是由DHT11输出的
	Check_Res = Humi_H + Humi_L + Temp_H + Temp_L;
	if (Check_Res != CheckSum)
	{
		return;
	}

	/*************调试用 串口发送接收到的数据***********************************/
	//	Send_Data1(Humi_H);
	//	Send_Data1(Humi_L);
	//	Send_Data1(Temp_H);
	//	Send_Data1(Temp_L);
	/**************************************************************************/
}
// Humi 三个字节    Temp 六个字节
void Read_DHT11_Str(unsigned char *Temp, unsigned char *Humi)
{
	// 湿度两位数，比如 95   温度一位小数，比如  +32.6  ，-04.7 等
	Read_DHT11(); // 读取DHT11数据到全局变量

	Humi[0] = Humi_H / 10 + 0x30;//0011 0000
	Humi[1] = Humi_H % 10 + 0x30;
	Humi[2] = 0;

	if ((Temp_L & 0X80) == 0x80) // 温度低于0度
	{
		Temp_L &= 0X7F;
		Temp_L *= 10;
		Temp[0] = 0x2D;
		Temp[1] = Temp_H / 10 + 0x30;
		Temp[2] = Temp_H % 10 + 0x30;
		Temp[3] = 0x2E;
		Temp[4] = Temp_L / 10 + 0x30;
		Temp[5] = 0;
	}
	else // 温度高于0度
	{
		Temp_L *= 10;
		Temp[0] = 0x2B;
		Temp[1] = Temp_H / 10 + 0x30;
		Temp[2] = Temp_H % 10 + 0x30;
		Temp[3] = 0x2E;
		Temp[4] = Temp_L / 10 + 0x30;
		Temp[5] = 0;
	}
}
//
