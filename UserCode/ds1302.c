#include "stc89.h"
#include "ds1302.h"

// DS1302???????????
sbit SCK = P2 ^ 0;
sbit SDA = P2 ^ 1;
sbit RST = P2 ^ 2;

#define RST_CLR RST = 0
#define RST_SET RST = 1

#define SDA_CLR SDA = 0
#define SDA_SET SDA = 1

#define SCK_CLR SCK = 0
#define SCK_SET SCK = 1

#define ds1302_sec_add 0x80
#define ds1302_min_add 0x82
#define ds1302_hr_add 0x84
#define ds1302_date_add 0x86
#define ds1302_month_add 0x88
#define ds1302_day_add 0x8a
#define ds1302_year_add 0x8c
#define ds1302_control_add 0x8e
#define ds1302_charger_add 0x90
#define ds1302_clkburst_add 0xbe

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

unsigned char currentTimeData[7] = {0};                   // ???????? ds1302 ???????????????????
unsigned char newTimeData[7] = {23, 10, 11, 12, 0, 0, 1}; // ???????????

unsigned char time_buf1[8] = {20, 10, 6, 5, 12, 55, 00, 6}; // ???????????????
unsigned char time_buf[8];                                  // ???????????????

// ?DS1302???????
void write1ByteToDs1302(unsigned char addr, unsigned char d)
{
    unsigned char i;
    RST_SET;

    // §Õ?????????addr
    addr = addr & 0xFE; // ???¦Ë????
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

    // §Õ???????d
    for (i = 0; i < 8; i++)
    {
        if (d & 0x01)
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
    RST_CLR; // ??DS1302????
}

// ??DS1302????????????
unsigned char read1ByteFromDs1302(unsigned char addr)
{

    unsigned char i;
    unsigned char temp;
    RST_SET;

    // §Õ?????????addr
    addr = addr | 0x01; // ???¦Ë???
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

    // ????????temp
    for (i = 0; i < 8; i++)
    {
        temp = temp >> 1;
        if (SDA)
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

    RST_CLR; // ??DS1302????
    return temp;
}

// ? ds1302 ?? ??
unsigned char setHourToDs1302(unsigned char hourValue)
{
    unsigned char tmp, time_buf;
    // BCD????
    tmp = hourValue / 10;      // ??¦Ë
    time_buf = hourValue % 10; // ???¦Ë
    time_buf = time_buf + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00); // ???§Õ????
    write1ByteToDs1302(ds1302_sec_add, 0x80);     // ???
    write1ByteToDs1302(ds1302_hr_add, time_buf);  // ?
    write1ByteToDs1302(ds1302_control_add, 0x80); // ??§Õ????
}

// ? ds1302 ?? ??
unsigned char setMinuteToDs1302(unsigned char minuteValue)
{
    unsigned char tmp, time_buf;
    // BCD????
    tmp = minuteValue / 10;      // ??¦Ë
    time_buf = minuteValue % 10; // ???¦Ë
    time_buf = time_buf + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00); // ???§Õ????
    write1ByteToDs1302(ds1302_sec_add, 0x80);     // ???
    write1ByteToDs1302(ds1302_min_add, time_buf); // ??
    write1ByteToDs1302(ds1302_control_add, 0x80); // ??§Õ????
}

// ? ds1302 ?? ??
unsigned char setSecondToDs1302(unsigned char secondValue)
{
    unsigned char tmp, time_buf;
    // BCD??
    tmp = secondValue / 10;      // ??¦Ë
    time_buf = secondValue % 10; // ???¦Ë
    time_buf = time_buf + tmp * 16;
    write1ByteToDs1302(ds1302_control_add, 0x00); // ???§Õ????
    write1ByteToDs1302(ds1302_sec_add, 0x80);     // ???
    write1ByteToDs1302(ds1302_sec_add, time_buf); // ??
    write1ByteToDs1302(ds1302_control_add, 0x80); // ??§Õ????
}

// ?DS1302??????
void writeTimeToDs1302(void)
{

    unsigned char i, tmp;
    for (i = 0; i < 8; i++)
    {
        // BCD????
        tmp = time_buf1[i] / 10;
        time_buf[i] = time_buf1[i] % 10;
        time_buf[i] = time_buf[i] + tmp * 16;
    }
    write1ByteToDs1302(ds1302_control_add, 0x00); // ?????
    write1ByteToDs1302(ds1302_sec_add, 0x80);     // ??
    // Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//????
    write1ByteToDs1302(ds1302_year_add, time_buf[1]);  // ?
    write1ByteToDs1302(ds1302_month_add, time_buf[2]); // ?
    write1ByteToDs1302(ds1302_date_add, time_buf[3]);  // ?
    write1ByteToDs1302(ds1302_hr_add, time_buf[4]);    // ?
    write1ByteToDs1302(ds1302_min_add, time_buf[5]);   // ?
    write1ByteToDs1302(ds1302_sec_add, time_buf[6]);   // ?
    write1ByteToDs1302(ds1302_day_add, time_buf[7]);   // ?
    write1ByteToDs1302(ds1302_control_add, 0x80);      // ?????
}

// ?DS1302??????
void readTimeFromDs1302(void)
{
    unsigned char i, tmp;
    currentTimeData[0] = read1ByteFromDs1302(ds1302_year_add);         // ?
    currentTimeData[1] = read1ByteFromDs1302(ds1302_month_add);        // ?
    currentTimeData[2] = read1ByteFromDs1302(ds1302_date_add);         // ?
    currentTimeData[3] = read1ByteFromDs1302(ds1302_hr_add);           // ?
    currentTimeData[4] = read1ByteFromDs1302(ds1302_min_add);          // ?
    currentTimeData[5] = (read1ByteFromDs1302(ds1302_sec_add)) & 0x7F; // ?
    currentTimeData[6] = read1ByteFromDs1302(ds1302_day_add);          // ?

    for (i = 0; i < 8; i++)
    {
        // BCD??
        tmp = time_buf[i] / 16;
        time_buf1[i] = time_buf[i] % 16;
        time_buf1[i] = time_buf1[i] + tmp * 10;
    }
}

unsigned char *getTimeData()
{
    return currentTimeData;
}

// DS1302???
void ds1302Init(void)
{
    RST_CLR;
    SCK_CLR;
    write1ByteToDs1302(ds1302_sec_add, 0x00);
}
