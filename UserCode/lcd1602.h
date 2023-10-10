#ifndef __1602_H__
#define __1602_H__

// lcd1602初始化
void lcd1602Init(void);

bit LCD1602_Check_Busy(void);

void LCD1602_Write_Com(unsigned char com);

void LCD1602_Write_Data(unsigned char Data);

void LCD1602_Clear(void);

void LCD1602_Write_String(unsigned char x, unsigned char y, unsigned char *s);

void LCD1602_Write_Char(unsigned char x, unsigned char y, unsigned char Data);

void lcd1602Display(void);

#endif
