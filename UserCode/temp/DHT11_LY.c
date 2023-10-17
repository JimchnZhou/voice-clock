#include "stc89.h"
#include "dht11.h"
#include "delay.h"
#include <intrins.h>

// #define DHT11_IO P35
sbit DHT11_IO = P2 ^ 4;
// ��ֲע�⣺��ͬ�ͺŵĵ�Ƭ������Ҫ�޸Ķ�Ӧ����ʱ������ DHT11�����ݽӿڶ���Ϊ���Լ��Ľӿڡ�

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

static unsigned char recive_byte(void) // һ���ֽڽ��պ���
{

	unsigned char i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		while (DHT11_IO)
			; // �ȴ�DHT11����.������������Ǵӵ͵�ƽ��ʼ��
		while (DHT11_IO == 0)
			; // �ȴ�DHT11����
		Delay_us(40);
		dat = dat << 1;
		if (DHT11_IO) // 27us����1�����ʾ����1
		{
			dat = dat + 1;
		}
	}
	return dat; // �����ܵ������ݷ���
}
void Read_DHT11(void)
{
	unsigned char Check_Res = 0;

	// Delay1ms(5000);//�����ã����˺���������ѭ����ʵ��5Sִ��һ��

	// ������ʼ�ź�
	DHT11_IO = 0;
	Delay_ms(25);
	DHT11_IO = 1;
	while (DHT11_IO)
		; // �����ǵȴ�DHT11����������
	while (DHT11_IO == 0)
		; // �ȴ�DHT11��������
	// ��ʼ��������
	Humi_H = recive_byte();
	Humi_L = recive_byte();
	Temp_H = recive_byte();
	Temp_L = recive_byte();
	CheckSum = recive_byte();
	// ���� DHT11 ����Ľ����ź�
	Delay1us(60); // �ӳٺ����߻��Ǹߵ�ƽ��ʵ��������ߵ�ƽ����DHT11�����
	Check_Res = Humi_H + Humi_L + Temp_H + Temp_L;
	if (Check_Res != CheckSum)
	{
		return;
	}

	/*************������ ���ڷ��ͽ��յ�������***********************************/
	//	Send_Data1(Humi_H);
	//	Send_Data1(Humi_L);
	//	Send_Data1(Temp_H);
	//	Send_Data1(Temp_L);
	/**************************************************************************/
}
// Humi �����ֽ�    Temp �����ֽ�
void Read_DHT11_Str(unsigned char *Temp, unsigned char *Humi)
{
	// ʪ����λ�������� 95   �¶�һλС��������  +32.6  ��-04.7 ��
	Read_DHT11(); // ��ȡDHT11���ݵ�ȫ�ֱ���

	Humi[0] = Humi_H / 10 + 0x30;//0011 0000
	Humi[1] = Humi_H % 10 + 0x30;
	Humi[2] = 0;

	if ((Temp_L & 0X80) == 0x80) // �¶ȵ���0��
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
	else // �¶ȸ���0��
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
