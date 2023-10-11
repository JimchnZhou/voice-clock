#include "stc89.h"
#include "wt588.h"
#include "delay.h"
#include "mode.h"
#include "ds1302.h"
#include "dht11.h"

sbit Busy = P3 ^ 5;
sbit One_Wire_SDA = P3 ^ 6;
sbit Two_Wire_SCL = P3 ^ 6;
sbit Two_Wire_SDA = P3 ^ 7;

// WT588F 一线 单字节 控制程序
void One_Wire_Send_Byte(unsigned char Byte_Value)
{
    unsigned char S_DATA, i;
    S_DATA = Byte_Value;
    One_Wire_SDA = 0;       // 拉低数据线
    Delay_ms(5);            // 延时5ms
    for (i = 0; i < 8; i++) // 1Byte=8bit
    {
        if ((S_DATA & 0X01) == 1) // 发送1
        {
            One_Wire_SDA = 1;
            Delay_us(600); // 延时 600us
            One_Wire_SDA = 0;
            Delay_us(200); // 延时 200us
        }
        else // 发送0
        {
            One_Wire_SDA = 1;
            Delay_us(200); // 延时 200us
            One_Wire_SDA = 0;
            Delay_us(600); // 延时 600us
        }
        S_DATA = S_DATA >> 1;
    }
    One_Wire_SDA = 1; // 发送完毕，拉高数据线
}

// WT588F 一线 双字节 控制程序
void One_Wire_Send_Double(unsigned int Double_Value)
{
    unsigned char i;
    //	    unsigned char i, num_temp;
    unsigned char B_DATA;
    unsigned int ddata_temp, pdata_temp;
    ddata_temp = Double_Value;
    pdata_temp = ddata_temp & 0X00FF; // 移位前准备
    pdata_temp <<= 8;                 // 低字节移位到高字节
    ddata_temp >>= 8;                 // 高字节移位到低字节
    ddata_temp |= pdata_temp;         // 移位后重新合并
    //    num_temp = 16;

    One_Wire_SDA = 0; // 拉低数据线
    Delay_ms(5);      // 延时 5mS
    B_DATA = ddata_temp & 0X0001;
    //    for(i = 0; i < num_temp; i++)
    for (i = 0; i < 16; i++)
    {
        if (i == 8)
        {
            One_Wire_SDA = 1;
            Delay_ms(2); // 延时 2MS
            One_Wire_SDA = 0;
            Delay_ms(5); // 延时 5MS
        }
        One_Wire_SDA = 1; // 拉高数据传输线 ,准备传输数据
        if (B_DATA == 0)
        {
            /*表示逻辑电平 0*/
            Delay_us(200); // 延时 200us
            One_Wire_SDA = 0;
            Delay_us(600); // 延时 600us
        }
        else
        {
            /*表示逻辑电平 1*/
            Delay_us(600); // 延时 600us
            One_Wire_SDA = 0;
            Delay_us(200); // 延时 200us
        }
        ddata_temp = ddata_temp >> 1;
        B_DATA = ddata_temp & 0x0001;
    }
    One_Wire_SDA = 1;
}

// WT588F 一线 单字节 连码示例(F3 + 01 + F3 + 02)
void List_1A_Play_WT588F(void)
{
    One_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    One_Wire_Send_Byte(0x01);
    Delay_ms(5); // 延时 5ms
    One_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    One_Wire_Send_Byte(0x02);
    Delay_ms(5); // 延时 5ms
}

// WT588F 一线 双字节 连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_1A_Play_WT588F_Couple(void)
{
    One_Wire_Send_Double(0xFFF3);
    Delay_ms(5); // 延时 5ms
    One_Wire_Send_Double(0x0001);
    Delay_ms(10); // 延时 10ms
    One_Wire_Send_Double(0xFFF3);
    Delay_ms(5); // 延时 5ms
    One_Wire_Send_Double(0x0001);
    Delay_ms(10); // 延时 10ms
}

// WT588F 二线 单字节 控制程序
void Two_Wire_Send_Byte(unsigned char Byte_Value)
{
    unsigned char S_DATA, i;
    unsigned char B_DATA;
    Two_Wire_SCL = 1; // 拉高 CLK
    Two_Wire_SDA = 1; // 拉高 DATA
    S_DATA = Byte_Value;

    Two_Wire_SCL = 0; // 拉低 CLK
    Delay_ms(5);      // 延时 5ms
    B_DATA = S_DATA & 0X01;

    for (i = 0; i < 8; i++)
    {
        Two_Wire_SCL = 0;      // 拉低CLK，为发送数据做准备
        Two_Wire_SDA = B_DATA; // 传输低位数据
        Delay_us(300);         // 延时300us
        Two_Wire_SCL = 1;      // 拉高CLK,上升沿接收数据
        Delay_us(300);         // 延时350us
        S_DATA = S_DATA >> 1;  // 数据右移
        B_DATA = S_DATA & 0X01;
    }
    Two_Wire_SDA = 1;
    Two_Wire_SCL = 1;
}

// WT588F 二线 双字节 控制程序
void Two_Wire_Send_Double(unsigned int Double_Value)
{
    unsigned char i;
    unsigned int ddata_temp, pdata_temp;
    ddata_temp = Double_Value;

    // 高低字节交换
    pdata_temp = ddata_temp & 0X00FF; // 取低字节
    ddata_temp >>= 8;                 // 右移8位，高字节变为低字节
    pdata_temp <<= 8;                 // 低字节左移，放到高字节
    ddata_temp |= pdata_temp;         // 合并

    Two_Wire_SCL = 0; // 拉低 CLK
    Delay_ms(5);      // 延时 5ms
    for (i = 0; i < 16; i++)
    {
        if (i == 8)
        {
            Two_Wire_SCL = 1;
            Two_Wire_SDA = 1;
            Delay_ms(2); // 延时 2ms
            Two_Wire_SCL = 0;
            Delay_ms(5); // 延时 5ms
        }
        Two_Wire_SCL = 0;
        Two_Wire_SDA = ddata_temp & 0X0001;
        Delay_us(200); // 延时 200us
        Two_Wire_SCL = 1;
        Delay_us(200); // 延时 200us
        ddata_temp = ddata_temp >> 1;
    }
    Two_Wire_SCL = 1;
    Two_Wire_SDA = 1;
}

// WT588F 二线 单字节 连码示例(F3 + 01 + F3 + 02)
void List_2A_Play_WT588F(void)
{

    // 温度
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(54);
    Delay_ms(5); // 延时 5ms

    // 3
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(3);
    Delay_ms(5); // 延时 5ms

    // 10
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(10);
    Delay_ms(5); // 延时 5ms

    // 2
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(2);
    Delay_ms(5); // 延时 5ms

    // 湿度
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(55);
    Delay_ms(5); // 延时 5ms

    // 3
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(2);
    Delay_ms(5); // 延时 5ms

    // 10
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(10);
    Delay_ms(5); // 延时 5ms

    // 2
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(2);
    Delay_ms(5); // 延时 5ms

    // 点
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(65);
    Delay_ms(5); // 延时 5ms

    // 8
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(8);
    Delay_ms(5); // 延时 5ms
}

// WT588F 二线 双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_2A_Play_WT588F_Couple(void)
{
    Two_Wire_Send_Double(0xFFF3);
    Delay_ms(5); // 延时 5ms
    Two_Wire_Send_Double(0x0001);
    Delay_ms(10); // 延时 10ms
    Two_Wire_Send_Double(0xFFF3);
    Delay_ms(5); // 延时 5ms
    Two_Wire_Send_Double(0x0002);
    Delay_ms(10); // 延时 10ms
}

// 播报时间
void speakTime()
{
    unsigned char hour = getCurrentTimeData(3);
    unsigned char minute = getCurrentTimeData(4);
    // 现在是
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(59);
    Delay_ms(5); // 延时 5ms

    // 北京时间
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(60);
    Delay_ms(5); // 延时 5ms

    if (hour >= 20)
    {
        // 二
        Two_Wire_Send_Byte(0xF3);
        Delay_ms(2); // 延时 2ms
        Two_Wire_Send_Byte(2);
        Delay_ms(5); // 延时 5ms
    }
    if (hour >= 10)
    {
        // 十
        Two_Wire_Send_Byte(0xF3);
        Delay_ms(2); // 延时 2ms
        Two_Wire_Send_Byte(10);
        Delay_ms(5); // 延时 5ms
    }

    // 个位
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(hour % 10);
    Delay_ms(5); // 延时 5ms

    // 点
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(65);
    Delay_ms(5); // 延时 5ms

    if (minute >= 20)
    {
        // 十位
        Two_Wire_Send_Byte(0xF3);
        Delay_ms(2); // 延时 2ms
        Two_Wire_Send_Byte(minute / 10);
        Delay_ms(5); // 延时 5ms
    }
    if (minute >= 10)
    {
        // 十
        Two_Wire_Send_Byte(0xF3);
        Delay_ms(2); // 延时 2ms
        Two_Wire_Send_Byte(10);
        Delay_ms(5); // 延时 5ms
    }
    if ((minute % 10) != 0)
    {
        // 个位
        Two_Wire_Send_Byte(0xF3);
        Delay_ms(2); // 延时 2ms
        Two_Wire_Send_Byte(minute % 10);
        Delay_ms(5); // 延时 5ms
    }

    // 分
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(66);
    Delay_ms(5); // 延时 5ms
}

// 播报温度
void speakTemperature()
{ // 温度
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(54);
    Delay_ms(5); // 延时 5ms

    // 3
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(3);
    Delay_ms(5); // 延时 5ms

    // 10
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(10);
    Delay_ms(5); // 延时 5ms

    // 2
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(2);
    Delay_ms(5); // 延时 5ms
}

// 播报湿度
void speakHumidity()
{
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); // 延时 2ms
    Two_Wire_Send_Byte(55);
    Delay_ms(5); // 延时 5ms
}
/*
//欢迎使用智能语音电子时钟
    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); //延时 2ms
    Two_Wire_Send_Byte(37);
    Delay_ms(5); //延时 5ms

    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); //延时 2ms
    Two_Wire_Send_Byte(38);
    Delay_ms(5); //延时 5ms

    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); //延时 2ms
    Two_Wire_Send_Byte(39);
    Delay_ms(5); //延时 5ms

    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); //延时 2ms
    Two_Wire_Send_Byte(40);
    Delay_ms(5); //延时 5ms

    Two_Wire_Send_Byte(0xF3);
    Delay_ms(2); //延时 2ms
    Two_Wire_Send_Byte(41);
    Delay_ms(5); //延时 5ms
*/
