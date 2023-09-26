#include "STC89.h"
#include "1602.h"
#include "delay.h"

sbit RS = P2 ^ 4; 
sbit RW = P2 ^ 5;
sbit EN = P2 ^ 6;

#define RS_CLR RS=0
#define RS_SET RS=1

#define RW_CLR RW=0
#define RW_SET RW=1

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0

bit LCD_Check_Busy(void)
{
    DataPort = 0xFF;
    RS_CLR;
    RW_SET;
    EN_CLR;
    Delay_us(1);
    EN_SET;
    return (bit)(DataPort & 0x80);
}


void LCD_Write_Com(unsigned char com)
{
// while(LCD_Check_Busy()); 
    Delay_ms(5);
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort = com;
    Delay_us(1);
    EN_CLR;
}


void LCD_Write_Data(unsigned char Data)
{
//while(LCD_Check_Busy());
    Delay_ms(5);
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort = Data;
    Delay_us(1);
    EN_CLR;
}


void LCD_Clear(void)
{
    LCD_Write_Com(0x01);
    Delay_ms(5);
}


void LCD_Write_String(unsigned char x, unsigned char y, unsigned char* s)
{
    if(y == 0)
    {
        LCD_Write_Com(0x80 + x);
    }
    else
    {
        LCD_Write_Com(0xC0 + x);
    }
    while(*s)
    {
        LCD_Write_Data(*s);
        s ++;
    }
}


/* void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
 {
 if (y == 0)
 	{
 	LCD_Write_Com(0x80 + x);
 	}
 else
 	{
 	LCD_Write_Com(0xC0 + x);
 	}
 LCD_Write_Data( Data);
 }*/

//LCD1602初始化
void LCD_Init(void)
{
    LCD_Write_Com(0x38);
    Delay_ms(5);
    LCD_Write_Com(0x38);
    Delay_ms(5);
    LCD_Write_Com(0x38);
    Delay_ms(5);
    LCD_Write_Com(0x38);
    LCD_Write_Com(0x08);
    LCD_Write_Com(0x01);
    LCD_Write_Com(0x06);
    Delay_ms(5);
    LCD_Write_Com(0x0C);
}


