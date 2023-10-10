#ifndef __DS1302_H__
#define __DS1302_H__

// ds1302初始化
void ds1302Init(void);

void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
unsigned char Ds1302_Read_Byte(unsigned char addr);
void Ds1302_Write_Time(void);
void Ds1302_Read_Time(void);

#endif
