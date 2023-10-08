#include "STC89.h"
#include "dht11.h"
#include "delay.h"
#include "iic.h"
#include "eeprom.h"

sbit DHT11_DATA = P2 ^ 4;

#define DHT11_DATA_INPUT DHT11_DATA = 1 // DHT11_DATA 引脚设置为输入引脚
#define DHT11_DATA_CLR DHT11_DATA = 0   // DHT11_DATA 引脚拉低
#define DHT11_DATA_SET DHT11_DATA = 1   // DHT11_DATA 引脚拉高

// unsigned char U8FLAG, U8temp, U8comdata;

// unsigned char U8RH_data_H_temp; // 湿度高8位缓存
// unsigned char U8RH_data_L_temp; // 湿度低8位缓存
// unsigned char U8T_data_H_temp;  // 温度高8位缓存
// unsigned char U8T_data_L_temp;  // 温度低8位缓存
// unsigned char U8checkdata_temp; // 校验位8位缓存

typedef struct
{
    unsigned char U8RH_data_H; // 湿度高8位 U8RH_data_H，湿度的整数
    unsigned char U8RH_data_L; // 湿度低8位 U8RH_data_L，湿度的小数
    unsigned char U8T_data_H;  // 温度高8位 U8T_data_H，温度的整数
    unsigned char U8T_data_L;  // 温度低8位 U8T_data_L，温度的小数
    unsigned char U8checkdata; // 校验位8位 U8checkdata，湿度高 8 位 + 湿度低 8 位 + 温度高 8 位 + 温度低 8 位/* data */
} DHT11_Data;

// // DHT11初始化
// DHT11_Init()
// {
// }

// 读取 DHT11_DATA 数据
unsigned char DHT11_Read(void)
{
    if(DHT11_DATA)
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
    DHT11_DATA_CLR;                       // 主机拉低总线
    Delay_ms(24);                         // 取 18ms 与 30ms 的中间值 24ms
    DHT11_DATA_SET;                       // 主机释放总线，总线由上拉电阻拉高
    Delay_us(20);                         // 主机延时20us，等待响应稳定
    DHT11_DATA_INPUT;                     // 主机设为输入，判断从机响应信号
    while(DHT11_Read() && (retry < 120))  // 检测 DHT11 的低电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if(retry >= 120)  // 判断是否超时
    {
        return -1; // 超时返回 -1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平，继续检测
    }
    while((!DHT11_Read() && (retry < 120)))  // 检测 DHT11 的高电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if(retry >= 120)  // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        return 0; // DHT11 正确响应
    }
}

// 主机接受 DHT11 发送的数据_1bit
// 数据0:54us低 + 23~27us高
// 数据1:54us低 + 68~74us高
static unsigned char DHT11_Read_Bit(void)
{
    unsigned char retry = 0; // 定义变量，重试次数

    while(DHT11_Read() && (retry < 120))  // 检测 DHT11 的低电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    retry = 0;

    while((!DHT11_Read() && (retry < 120)))  // 检测 DHT11 的高电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    Delay_us(45); // 延时45us，跳过数据0的高电平时间

    if(DHT11_Read())  // 45us 后，如果 DHT11_DATA 还为高说明是数据 1
    {
        return 1;
    }
    else // 45us 后，DHT11_DATA 不为高说明是数据 0
    {
        return 0;
    }
}

// 主机接受 DHT11 发送的数据_1Byte（高位先出）
static unsigned char DHT11_Read_Byte(void)
{
    unsigned char i, dat;
    dat = 0;
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}

unsigned char DHT11_Read_Data(DHT11_Data* Data)
{
    unsigned char retry = 0; // 定义变量，重试次数
    Data->U8RH_data_H = DHT11_Read_Byte();
    Data->U8RH_data_L = DHT11_Read_Byte();
    Data->U8T_data_H = DHT11_Read_Byte();
    Data->U8T_data_L = DHT11_Read_Byte();
    Data->U8checkdata = DHT11_Read_Byte();

    while(DHT11_Read() && (retry < 120))  // 检测 DHT11 的低电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }

    if(retry >= 120)  // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平，继续检测
    }
    while((!DHT11_Read() && (retry < 120)))  // 检测 DHT11 的高电平响应
    {
        retry++;     // 重试次数 ++
        Delay_us(1); // 一次重试延时 1us，重试次数相当于延时时长
    }
    if(retry >= 120)  // 判断是否超时
    {
        return -1; // 超时返回 1，DHT11 无响应
    }
    else
    {
        retry = 0; // 未超时代表 DHT11 正确拉低电平，继续检测
    }

    DHT11_DATA_SET;
    if(Data->U8checkdata == Data->U8RH_data_H + Data->U8RH_data_L + Data->U8T_data_H + Data->U8T_data_L)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

// void COM(void)
// {
//     unsigned char i;
//     for (i = 0; i < 8; i++)
//     {
//         U8FLAG = 2;
//         while ((!DHT11_DATA) && U8FLAG++)
//             ;
//         Delay_us(20);
//         U8temp = 0;
//         if (DHT11_DATA)
//             U8temp = 1;
//         U8FLAG = 2;
//         while ((DHT11_DATA) && U8FLAG++)
//             ;
//         // 超时则跳出for循环
//         if (U8FLAG == 1)
//             break;
//         // 判断数据位是0还是1

//         // 如果高电平高过预定0高电平值则数据位为 1

//         U8comdata <<= 1;
//         U8comdata |= U8temp; // 0
//     }                        // rof
// }

// unsigned char RH(void)
// {
//     // 主机拉低(18ms<T<30ms)作为起始信号
//     DHT11_DATA_CLR; // 主机拉低总线
//     Delay_ms(24);   // 取 18ms 与 30ms 的中间值 24ms
//     DHT11_DATA_SET; // 主机释放总线，总线由上拉电阻拉高
//     Delay_us(20);   // 主机延时20us

//     DHT11_DATA_INPUT; // 主机设为输入 判断从机响应信号

//     // 判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
//     if (!DHT11_DATA) // T !
//     {
//         U8FLAG = 2;
//         while ((!DHT11_DATA) && U8FLAG++)
//             ; // 判断从机是否发出 80us 的低电平响应信号是否结束
//         U8FLAG = 2;
//         while ((DHT11_DATA) && U8FLAG++)
//             ; // 判断从机是否发出 80us 的高电平，如发出则进入数据接收状态

//         // 数据接收状态
//         COM();
//         U8RH_data_H_temp = U8comdata;
//         COM();
//         U8RH_data_L_temp = U8comdata;
//         COM();
//         U8T_data_H_temp = U8comdata;
//         COM();
//         U8T_data_L_temp = U8comdata;
//         COM();
//         U8checkdata_temp = U8comdata;

//         DHT11_DATA = 1;
//         // 数据校验
//         U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8RH_data_H_temp + U8RH_data_L_temp);
//         if (U8temp == U8checkdata_temp) // 数据校验通过，将缓存数据写入
//         {
//             Data->U8RH_data_H = U8RH_data_H_temp;
//             U8RH_data_L = U8RH_data_L_temp;
//             U8T_data_H = U8T_data_H_temp;
//             U8T_data_L = U8T_data_L_temp;
//             U8checkdata = U8checkdata_temp;
//         }
//         return 1; // 数据校验不通过，返回 1
//     }
//     else // 传感器不响应
//     {
//         return 0;
//     }
// }
