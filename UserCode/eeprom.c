#include "STC89.h"
#include "eeprom.h"
#include "delay.h"
#include "iic.h"

sbit DATA = P1 ^ 7;
unsigned char U8FLAG, U8temp, U8comdata;

unsigned char rdeeprom(unsigned char addr)
{
    unsigned char temp = 0;
    bit flag = 0;
    IIC_Start();
    Write_byte(0xa0);
    Check_Ack();
    Write_byte(addr);
    Check_Ack();
    IIC_Start();
    Write_byte(0xa1);
    Check_Ack();
    temp = Read_byte();
    SEND1();
    IIC_Stop();
    return temp;
}


void wrteeprom(unsigned char addr, unsigned char dat)
{
    IIC_Start();
    Write_byte(0xa0);
    Check_Ack();
    Write_byte(addr);
    Check_Ack();
    Write_byte(dat);
    Check_Ack();
    IIC_Stop();
}

