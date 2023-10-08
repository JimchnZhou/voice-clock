#include "STC89.h"
#include "lcd1602.h"
#include "delay.h"

sbit RS = P2 ^ 7;
sbit RW = P2 ^ 6;
sbit EN = P2 ^ 5;

#define RS_CLR RS=0		//RS引脚拉低，输入指令
#define RS_SET RS=1		//RS引脚拉高，输出数据

#define RW_CLR RW=0		//RW引脚拉低，向LCD写指令或数据
#define RW_SET RW=1		//RW引脚拉高，从LCD读取信息

#define EN_CLR EN=0		//EN引脚拉低，EN下降沿(1→0)时执行指令
#define EN_SET EN=1		//EN引脚拉高，EN=1时读取信息

#define DATAPORT P0		//8位数据总线引脚

//LCD1602判忙
bit LCD1602_Check_Busy(void)
{
    DATAPORT = 0xFF;
    RS_CLR;
    RW_SET;
    EN_CLR;
    Delay_us(1);
    EN_SET;
    return (bit)(DATAPORT & 0x80);
}

//向LCD写指令
void LCD1602_Write_Com(unsigned char com)
{
// while(LCD_Check_Busy());
    Delay_ms(5);
    RS_CLR;
    RW_CLR;
    EN_SET;
    DATAPORT = com;
    Delay_us(1);
    EN_CLR;
}

//向LCD写数据
void LCD1602_Write_Data(unsigned char Data)
{
//while(LCD_Check_Busy());
    Delay_ms(5);
    RS_SET;
    RW_CLR;
    EN_SET;
    DATAPORT = Data;
    Delay_us(1);
    EN_CLR;
}

//LCD1602清屏
void LCD1602_Clear(void)
{
    LCD1602_Write_Com(0x01);
    Delay_ms(5);
}

//向LCD写
void LCD1602_Write_String(unsigned char x, unsigned char y, unsigned char* s)
{
    if(y == 0)
    {
        LCD1602_Write_Com(0x80 + x);
    }
    else
    {
        LCD1602_Write_Com(0xC0 + x);
    }
    while(*s)
    {
        LCD1602_Write_Data(*s);
        s ++;
    }
}

//LCD1602指定位置写数据
/* void LCD1602_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
 {
 if (y == 0)
 	{
 	LCD1602_Write_Com(0x80 + x);
 	}
 else
 	{
 	LCD1602_Write_Com(0xC0 + x);
 	}
 LCD_Write_Data( Data);
 }*/

//LCD1602初始化
void LCD1602_Init(void)
{
    LCD1602_Write_Com(0x38);
    Delay_ms(5);
    LCD1602_Write_Com(0x38);
    Delay_ms(5);
    LCD1602_Write_Com(0x38);
    Delay_ms(5);
    LCD1602_Write_Com(0x38);
    LCD1602_Write_Com(0x08);
    LCD1602_Write_Com(0x01);
    LCD1602_Write_Com(0x06);
    Delay_ms(5);
    LCD1602_Write_Com(0x0C);
}


