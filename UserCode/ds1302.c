#include "stc89.h"
#include "ds1302.h"

// DS1302引脚定义
sbit SCK = P2 ^ 0;
sbit SDA = P2 ^ 1;
sbit RST = P2 ^ 2;

#define RST_CLR RST = 0
#define RST_SET RST = 1

#define SDA_CLR SDA = 0
#define SDA_SET SDA = 1

#define SCK_CLR SCK = 0
#define SCK_SET SCK = 1

#define ds1302_sec_add 0x80      // 秒
#define ds1302_min_add 0x82      // 分
#define ds1302_hr_add 0x84       // 小时
#define ds1302_date_add 0x86     // 日
#define ds1302_month_add 0x88    // 月
#define ds1302_day_add 0x8a      // 星期
#define ds1302_year_add 0x8c     // 年
#define ds1302_control_add 0x8e  // 控制寄存器
#define ds1302_charger_add 0x90  // 充电寄存器
#define ds1302_clkburst_add 0xbe // 时钟多字节方式

// extern unsigned char time_buf1[8];
// extern unsigned char time_buf[8] ;
// ???????????????????
//  struct RTC
//  {
//      unsigned char	Seconds;//??
//      unsigned char	Minutes;//????
//      unsigned char	Hour;//§³?
//      unsigned char	Week;//????
//      unsigned char	Date;//??
//      unsigned char	Month;//??
//      unsigned char	Year;//??
//      unsigned char NC;//?????????
//  };
//  static struct RTC RTC_Timer;		   // ???????RTC???

unsigned char currentTimeData[7] = {0};                     // 当前时间
unsigned char newTimeData[7] = {23, 10, 11, 12, 34, 56, 1}; // 需设置的时间
unsigned char timeBuff[7];                                  // 时间缓存 BCD

// unsigned char time_buf1[8] = {20, 10, 6, 5, 12, 55, 00, 6}; // ???????????????

// 向 DS1302 写入 1 字节数据
void write1ByteToDs1302(unsigned char addr, unsigned char dat)
{
    unsigned char i;
    RST_SET;            // RST使能
    addr = addr & 0xFE; // 11111110，最低位置0，写操作
    for (i = 0; i < 8; i++)
    {
        if (addr & 0x01) // 命令字节从最低位开始输入
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        SCK_SET; // 上升沿写入数据
        SCK_CLR;
        addr = addr >> 1;
    }
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x01) // 数据字节从最低位开始输入
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        SCK_SET; // 上升沿写入数据
        SCK_CLR;
        dat = dat >> 1;
    }
    RST_CLR; // RST失能
}

// 从 DS1302 读取 1 字节数据
unsigned char read1ByteFromDs1302(unsigned char addr)
{
    unsigned char i;
    unsigned char temp;
    RST_SET;
    addr = addr | 0x01; // 0000 0001，最低位置1，读操作
    for (i = 0; i < 8; i++)
    {

        if (addr & 0x01)
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
    for (i = 0; i < 8; i++)
    {
        temp = temp >> 1;
        if (SDA)
        {
            temp |= 0x80;
        }
        else
        {
            temp &= 0x7F; // 0111 1111
        }
        SCK_SET;
        SCK_CLR;
    }
    RST_CLR;
    return temp;
}

// 向 DS1302 写入小时数据
void writeHourToDs1302(void)
{
    unsigned char tmp;
    // BCD处理
    tmp = newTimeData[3] / 10;
    timeBuff[3] = newTimeData[3] % 10;
    timeBuff[3] = timeBuff + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00);   // 关闭写保护
    write1ByteToDs1302(ds1302_sec_add, 0x80);       // 停振
    write1ByteToDs1302(ds1302_hr_add, timeBuff[3]); // 写入小时
    write1ByteToDs1302(ds1302_control_add, 0x80);   // 打开写保护
}

// 向 DS1302 写入分钟数据
void writeMinuteToDs1302(void)
{
    unsigned char tmp;
    // BCD????
    tmp = newTimeData[4] / 10;         // ??¦Ë
    timeBuff[4] = newTimeData[4] % 10; // ???¦Ë
    timeBuff[4] = timeBuff[4] + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00);    // ???§Õ????
    write1ByteToDs1302(ds1302_sec_add, 0x80);        // ???
    write1ByteToDs1302(ds1302_min_add, timeBuff[4]); // ??
    write1ByteToDs1302(ds1302_control_add, 0x80);    // ??§Õ????
}

// 向 DS1302 写入秒钟数据
void writeSecondToDs1302(void)
{
    unsigned char tmp;
    // BCD??
    tmp = newTimeData[5] / 10;         // ??¦Ë
    timeBuff[5] = newTimeData[5] % 10; // ???¦Ë
    timeBuff[5] = timeBuff[5] + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00);    // ???§Õ????
    write1ByteToDs1302(ds1302_sec_add, 0x80);        // ???
    write1ByteToDs1302(ds1302_sec_add, timeBuff[5]); // ??
    write1ByteToDs1302(ds1302_control_add, 0x80);    // ??§Õ????
}

// 向 DS1302 写入年月日时分秒周数据
void writeTimeToDs1302(void)
{
    unsigned char i, tmp;
    for (i = 0; i < 8; i++)
    {
        // BCD处理
        tmp = newTimeData[i] / 10;
        timeBuff[i] = newTimeData[i] % 10;
        timeBuff[i] = timeBuff[i] + tmp * 16;
    }
    write1ByteToDs1302(ds1302_control_add, 0x00); // 关闭写保护
    write1ByteToDs1302(ds1302_sec_add, 0x80);     // 停止振荡
    // Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//????
    write1ByteToDs1302(ds1302_year_add, timeBuff[0]);  // 年
    write1ByteToDs1302(ds1302_month_add, timeBuff[1]); // 月
    write1ByteToDs1302(ds1302_date_add, timeBuff[2]);  // 日
    write1ByteToDs1302(ds1302_hr_add, timeBuff[3]);    // 时
    write1ByteToDs1302(ds1302_min_add, timeBuff[4]);   // 分
    write1ByteToDs1302(ds1302_sec_add, timeBuff[5]);   // 秒
    // write1ByteToDs1302(ds1302_day_add, timeBuff[6]);   // 周
    write1ByteToDs1302(ds1302_control_add, 0x80); // 开启写保护
}

// 从 DS1302 读取年月日时分秒周数据
void readTimeFromDs1302(void)
{
    unsigned char i, byteHigh, byteLow;
    timeBuff[0] = read1ByteFromDs1302(ds1302_year_add);         // 年
    timeBuff[1] = read1ByteFromDs1302(ds1302_month_add);        // 月
    timeBuff[2] = read1ByteFromDs1302(ds1302_date_add);         // 日
    timeBuff[3] = read1ByteFromDs1302(ds1302_hr_add);           // 时
    timeBuff[4] = read1ByteFromDs1302(ds1302_min_add);          // 分
    timeBuff[5] = (read1ByteFromDs1302(ds1302_sec_add)) & 0x7F; // 秒 0111 1111
    timeBuff[6] = read1ByteFromDs1302(ds1302_day_add);          // 周
    for (i = 0; i < 7; i++)
    {
        // BCD处理
        byteHigh = timeBuff[i] / 16; // 取高4位
        byteLow = timeBuff[i] % 16;  // 取低4位
        currentTimeData[i] = byteLow + byteHigh * 10;
    }
}

// 获取当前时间数据
unsigned char getCurrentTimeData(unsigned char value)
{
    return currentTimeData[value];
}

// 获取新时间数据
unsigned char getNewTimeData(unsigned char value)
{
    return newTimeData[value];
}

// 设置新时间数据
void setNewTimeData(unsigned char addr, unsigned char value)
{
    newTimeData[addr] = value;
}

// DS1302 初始化
void ds1302Init(void)
{
    RST_CLR;
    SCK_CLR;
    write1ByteToDs1302(ds1302_sec_add, 0x00);
    // writeTimeToDs1302();
}
