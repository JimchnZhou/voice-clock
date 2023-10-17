#include "stc89.h"
#include "dht11.h"
#include "delay.h"

sbit DHT11_PIN = P2 ^ 4;

#define DHT11_PIN_INPUT DHT11_PIN = 1 // DHT11_PIN 引脚设置为输入引脚
#define DHT11_PIN_CLR DHT11_PIN = 0   // DHT11_PIN 引脚拉低
#define DHT11_PIN_SET DHT11_PIN = 1   // DHT11_PIN 引脚拉高

unsigned char humiHighByte; // 湿度高8位 humiHighByte，湿度的整数
unsigned char humiLowByte;  // 湿度低8位 humiLowByte，湿度的小数
unsigned char tempHighByte; // 温度高8位 tempHighByte，温度的整数
unsigned char tempLowByte;  // 温度低8位 tempLowByte，温度的小数
unsigned char checkByte;    // 校验位8位 checkByte，湿度高 8 位 + 湿度低 8 位 + 温度高 8 位 + 温度低 8 位/* data */

// 获取 DHT11 引脚状态：高 低
unsigned char getPinStatusFromDHT11(void)
{
    if (DHT11_PIN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// 对 DHT11 发送起始信号，并检测 DHT11 的响应
// 主机起始信号：拉低总线 T (18ms < T <30ms)
// DHT11响应信号：83us低 + 87us高
unsigned char DHT11_Start(void)
{
    unsigned char retry = 0; // 定义变量，重试次数

    // 主机拉低(18ms<T<30ms)作为起始信号
    DHT11_PIN_CLR;   // 主机拉低总线
    delayMs(20);     // 数据手册典型值为 20ms
    DHT11_PIN_SET;   // 主机释放总线，总线由上拉电阻拉高，等待 DHT11 回答
    delayUs(20);     // 主机延时20us，等待响应稳定
    DHT11_PIN_INPUT; // 主机设为输入，判断从机响应信号

    // DHT11 在收到起始信号后，会回答 83us 低电平 + 87us 高电平
    while (getPinStatusFromDHT11() && (retry < 120)) // 检测 DHT11 的低电平响应
    {
        retry++;    // 重试次数 ++
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if (retry >= 120) // 判断是否超时
    {
        return -1; // 超时返回 -1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平，继续检测
    }
    while ((!getPinStatusFromDHT11() && (retry < 120))) // 检测 DHT11 的高电平响应
    {
        retry++;    // 重试次数 ++
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if (retry >= 120) // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        return 0; // DHT11 正确响应，DHT11 正常应答
    }
}

// 主机从 DHT11 读取 1bit 数据
// 数据0:54us低 + 23~27us高
// 数据1:54us低 + 68~74us高
static unsigned char read1BitFromDHT11(void)
{
    unsigned char retry = 0; // 定义变量，重试次数

    while (getPinStatusFromDHT11() && (retry < 120)) // 检测 DHT11 的低电平响应，有低电平代表开始输出数据
    {
        retry++;    // 重试次数
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    retry = 0;

    while ((!getPinStatusFromDHT11() && (retry < 120))) // 检测 DHT11 的高电平响应，等待 54us 低电平结束
    {
        retry++;    // 重试次数 ++
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    delayUs(45); // 延时45us，跳过数据0的高电平时间 45us > 23~27us

    if (getPinStatusFromDHT11()) // 45us 后，如果 DHT11_PIN 还为高说明是数据 1
    {
        return 1;
    }
    else // 45us 后，DHT11_PIN 不为高说明是数据 0
    {
        return 0;
    }
}

// 主机接受 DHT11 发送的数据_1Byte（高位先出）
static unsigned char read1ByteFromDHT11(void)
{
    unsigned char i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= read1BitFromDHT11();
    }
    return dat;
}

// 从 DHT11 读取数据
unsigned char readDataFromDht11(void)
{
    unsigned char retry = 0; // 定义变量，重试次数
    humiHighByte = read1ByteFromDHT11();
    humiLowByte = read1ByteFromDHT11();
    tempHighByte = read1ByteFromDHT11();
    tempLowByte = read1ByteFromDHT11();
    checkByte = read1ByteFromDHT11();

    // 结束信号，DHT11 输出 54us 低电平后转为高电平（输入状态）
    while (getPinStatusFromDHT11() && (retry < 120)) // 检测 DHT11 的低电平响应
    {
        retry++;    // 重试次数 ++
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }

    if (retry >= 120) // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平，继续检测
    }
    while ((!getPinStatusFromDHT11() && (retry < 120))) // 检测 DHT11 的高电平响应
    {
        retry++;    // 重试次数 ++
        delayUs(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if (retry >= 120) // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平
    }
    DHT11_PIN_SET; // 主机拉高释放总线

    if (checkByte == humiHighByte + humiLowByte + tempHighByte + tempLowByte)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

// 从DHT11获取湿度数据
unsigned char getHumiData(void)
{
    return humiHighByte + humiLowByte / 10;
}

// 从DHT11获取温度数据
unsigned char getTempData(void)
{
    return tempHighByte;
}
