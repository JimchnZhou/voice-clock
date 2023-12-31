#include "STC89.h"
#include "iic.h"
#include "delay.h"

//定义IIC数据接口
sbit SCL = P1 ^ 0;
sbit SDA = P1 ^ 1;

void IIC_Start(void)
{
    SCL = 0;
    Delay_us(1);
    SDA = 1;
    SCL = 1;
    Delay_us(1);
    SDA = 0;
    Delay_us(1);
    SCL = 0;
}

void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    Delay_us(1);
    SDA = 1;
    Delay_us(1);
    SCL = 0;
}

void SEND0(void)
{
    SDA = 0;
    SCL = 1;
    Delay_us(1);
    SCL = 0;
}

void SEND1(void)
{
    SDA = 1;
    Delay_us(1);
    SCL = 1;
    Delay_us(1);
    SCL = 0;
}

bit Check_Ack(void)
{
    unsigned char errtime = 250;
    Delay_us(1);
    SCL = 1;
    Delay_us(1);
    CY = SDA;
    while(CY)
    {
        errtime--;
        CY = SDA;
        if(!errtime)
        {
            IIC_Stop();
            return 1;
        }
    }
    Delay_us(1);
    SCL = 0;
    return 0;
}

void Write_byte(unsigned char dat)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        if((dat << i) & 0x80)
            SEND1();
        else
            SEND0();
    }
}

unsigned char Read_byte(void)
{
    unsigned char i, temp = 0;
    for(i = 0; i < 8; i++)
    {
        SDA = 1;
        SCL = 1;
        Delay_us(1);
        if(SDA == 1)
        {
            temp = temp << 1;
            temp = temp | 0x01;
        }
        else
            temp = temp << 1;
        SCL = 0;
    }
    return temp;
}
