#include "STC89.h"
#include "ds1302.h"

//DS1302�뵥Ƭ��������
sbit SCK = P2 ^ 0;	
sbit SDA = P2 ^ 1;
sbit RST = P2 ^ 2;

#define RST_CLR	RST=0
#define RST_SET	RST=1

#define SDA_CLR	SDA=0
#define SDA_SET	SDA=1

#define SCK_CLR	SCK=0
#define SCK_SET	SCK=1

#define ds1302_sec_add			0x80
#define ds1302_min_add			0x82
#define ds1302_hr_add			0x84
#define ds1302_date_add			0x86
#define ds1302_month_add		0x88
#define ds1302_day_add			0x8a
#define ds1302_year_add			0x8c
#define ds1302_control_add		0x8e
#define ds1302_charger_add		0x90
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];
extern unsigned char time_buf[8] ;

unsigned char time_buf1[8] = {20, 10, 6, 5, 12, 55, 00, 6}; //��������ʱ������
unsigned char time_buf[8] ;                         //��������ʱ������


//��DS1302д��һ�ֽ�����
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

    unsigned char i;
    RST_SET;

    //д��Ŀ���ַ��addr
    addr = addr & 0xFE;     //���λ����
    for(i = 0; i < 8; i ++)
    {
        if(addr & 0x01)
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        SCK_SET;
        SCK_CLR;
        addr = addr >> 1;
    }

    //д�����ݣ�d
    for(i = 0; i < 8; i ++)
    {
        if(d & 0x01)
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        SCK_SET;
        SCK_CLR;
        d = d >> 1;
    }
    RST_CLR;					//ֹͣDS1302����
}

//��DS1302����һ�ֽ�����
unsigned char Ds1302_Read_Byte(unsigned char addr)
{

    unsigned char i;
    unsigned char temp;
    RST_SET;

    //д��Ŀ���ַ��addr
    addr = addr | 0x01;//���λ�ø�
    for(i = 0; i < 8; i ++)
    {

        if(addr & 0x01)
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        SCK_SET;
        SCK_CLR;
        addr = addr >> 1;
    }

    //������ݣ�temp
    for(i = 0; i < 8; i ++)
    {
        temp = temp >> 1;
        if(SDA)
        {
            temp |= 0x80;
        }
        else
        {
            temp &= 0x7F;
        }
        SCK_SET;
        SCK_CLR;
    }

    RST_CLR;	//ֹͣDS1302����
    return temp;
}

//��DS1302д��ʱ������
void Ds1302_Write_Time(void)
{

    unsigned char i, tmp;
    for(i = 0; i < 8; i++)
    {
        //BCD����
        tmp = time_buf1[i] / 10;
        time_buf[i] = time_buf1[i] % 10;
        time_buf[i] = time_buf[i] + tmp * 16;
    }
    Ds1302_Write_Byte(ds1302_control_add, 0x00);			//�ر�д����
    Ds1302_Write_Byte(ds1302_sec_add, 0x80);				//��ͣ
    //Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//������
    Ds1302_Write_Byte(ds1302_year_add, time_buf[1]);		//��
    Ds1302_Write_Byte(ds1302_month_add, time_buf[2]);	//��
    Ds1302_Write_Byte(ds1302_date_add, time_buf[3]);		//��
    Ds1302_Write_Byte(ds1302_day_add, time_buf[7]);		//��
    Ds1302_Write_Byte(ds1302_hr_add, time_buf[4]);		//ʱ
    Ds1302_Write_Byte(ds1302_min_add, time_buf[5]);		//��
    Ds1302_Write_Byte(ds1302_sec_add, time_buf[6]);		//��
    Ds1302_Write_Byte(ds1302_day_add, time_buf[7]);		//��
    Ds1302_Write_Byte(ds1302_control_add, 0x80);			//��д����
}

//��DS1302����ʱ������
void Ds1302_Read_Time(void)
{
    unsigned char i, tmp;
    time_buf[1] = Ds1302_Read_Byte(ds1302_year_add);		//��
    time_buf[2] = Ds1302_Read_Byte(ds1302_month_add);		//��
    time_buf[3] = Ds1302_Read_Byte(ds1302_date_add);		//��
    time_buf[4] = Ds1302_Read_Byte(ds1302_hr_add);		//ʱ
    time_buf[5] = Ds1302_Read_Byte(ds1302_min_add);		//��
    time_buf[6] = (Ds1302_Read_Byte(ds1302_sec_add)) & 0x7F; //��
    time_buf[7] = Ds1302_Read_Byte(ds1302_day_add);		//��


    for(i = 0; i < 8; i++)
    {
        //BCD����
        tmp = time_buf[i] / 16;
        time_buf1[i] = time_buf[i] % 16;
        time_buf1[i] = time_buf1[i] + tmp * 10;
    }
}

//DS1302��ʼ��
void Ds1302_Init(void)
{
    RST_CLR;			//RST���õ�
    SCK_CLR;			//SCK���õ�
    Ds1302_Write_Byte(ds1302_sec_add, 0x00);
}
