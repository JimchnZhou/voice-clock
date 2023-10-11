#ifndef __DS1302_H__
#define __DS1302_H__


// 向DS1302写入一字节数据
void write1ByteToDs1302(unsigned char addr, unsigned char d);

// 从DS1302读出一字节数据
unsigned char read1ByteFromDs1302(unsigned char addr);

// 向 ds1302 写入 小时
void writeHourToDs1302(void);

// 向 ds1302 写入 分钟
void writeMinuteToDs1302(void);

// 向 ds1302 写入 秒钟
void writeSecondToDs1302(void);

// 向DS1302写入时间数据
void writeTimeToDs1302(void);

// 从DS1302读出时钟数据
void readTimeFromDs1302(void);

// 获取当前时间数据
unsigned char getCurrentTimeData(unsigned char value);
// 获取新时间数据
unsigned char getNewTimeData(unsigned char value);

// 设置新时间数据
void setNewTimeData(unsigned char addr,unsigned char value);

// DS1302初始化
void ds1302Init(void);


#endif
