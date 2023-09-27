#include "STC89.h"
#include "WT588.h"
#include "delay.h"

sbit Busy = P3 ^ 5;
sbit One_Line_Data = P3 ^ 6;
sbit Two_line_SCL = P3 ^ 7;

#define Two_line_SCL One_Line_SDA	//一线控制时的数据 是 二线控制时的时钟

//WT588F一线串口单字节控制程序
void Line_1A_WT588F(unsigned char Byte_Value)
{
    unsigned char S_DATA, i;
    S_DATA = Byte_Value;
    One_Line_Data = 0;
    Delay_ms(5); //延时 5ms
    for(i = 0; i < 8; i++)
    {
        if((S_DATA & 0X01) == 1)
        {
            One_Line_Data = 1;
            Delay_us(600); //延时 600us
            One_Line_Data = 0;
            Delay_us(200); //延时 200us
        }
        else
        {
            One_Line_Data = 1;
            Delay_us(200); //延时 200us
            One_Line_Data = 0;
            Delay_us(600); //延时 600us
        }
        S_DATA = S_DATA >> 1;
    }
    One_Line_Data = 1;
}

//一线串口双字节控制程序
/*--------------------------------------
;模块名称:Line_1A_WT588F_Couple(UI16 USER_DATA)
;功 能:实现一线串口通信函数
;入 参:USER_DATA
;出 参:
;one_line_DATA 控制 IO
;-------------------------------------*/

void Line_1A_WT588F_Couple(unsigned int Double_Value)
{
    unsigned char i, num_temp;
    unsigned char B_DATA;
    unsigned int ddata_temp, pdata_temp;
    ddata_temp = Double_Value;
    pdata_temp = ddata_temp & 0X00FF; //移位前准备
    pdata_temp <<= 8; //低字节移位到高字节
    ddata_temp >>= 8; //高字节移位到低字节
    ddata_temp |= pdata_temp; //移位后重新合并
    num_temp = 16;
    One_Line_Data = 0; //拉低数据线
    Delay_ms(5); //延时 5MS
    B_DATA = ddata_temp & 0X0001;
    for(i = 0; i < num_temp; i++)
    {
        if(i == 8)
        {
            One_Line_Data = 1;
            Delay_ms(2); //延时 2MS
            One_Line_Data = 0;
            Delay_ms(5); //延时 5MS
        }
        One_Line_Data = 1; //拉高数据传输线 ,准备传输数据
        if(B_DATA == 0)
        {
            /*表示逻辑电平 0*/
            Delay_us(200); // 延时 200us
            One_Line_Data = 0;
            Delay_us(600); // 延时 600us
        }
        else
        {
            /*表示逻辑电平 1*/
            Delay_us(600); // 延时 600us
            One_Line_Data = 0;
            Delay_us(200); // 延时 200us
        }
        ddata_temp = ddata_temp >> 1;
        B_DATA = ddata_temp & 0x0001;
    }
    One_Line_Data = 1;
}

//一线串口单字节连码示例(F3 + 01 + F3 + 02)
Void List_1A_Play_WT588F(void)
{
    Line_1A_WT588F(0xF3);
    Delay_ms(2); //延时 2ms
    Line_1A_WT588F(0x01);
    Delay_ms(5); //延时 5ms
    Line_1A_WT588F(0xF3);
    Delay_ms(2); //延时 2ms
    Line_1A_WT588F(0x02);
    Delay_ms(5); //延时 5ms
}

//一线串口双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
Void List_1A_Play_WT588F_Couple(void)
{
    Line_1A_WT588F_Couple(0xFFF3);
    Delay_ms(5); //延时 5ms
    Line_1A_WT588F_Couple(0x0001);
    Delay_ms(10); //延时 10ms
    Line_1A_WT588F_Couple(0xFFF3);
    Delay_ms(5); //延时 5ms
    Line_1A_WT588F_Couple(0x0001);
    Delay_ms(10); //延时 10ms
}

//二线串口单字节控制程序
/*--------------------------------------
;模块名称:Line_2A_WT588F(UC8 DDATA)
;功 能:实现二线串口通信函数
;入 参:DDATA 为发送数据
;出 参:
;CLK_2A 时钟线
;P_DATA_2A 数据线
;-------------------------------------*/

#define CLK_2A P01
#define P_DATA_2A P00
void Line_2A_WT588F(unsigned char DDATA)
{
    unsigned char S_DATA, j;
    unsigned char B_DATA;
    CLK_2A = 1; //拉高 CLK
    P_DATA_2A = 1; //拉高 DATA
    S_DATA = DDATA;
    CLK_2A = 0; //拉低 CLK
    Delay_ms(5); //延时 5ms
    B_DATA = S_DATA & 0X01;
    for(j = 0; j < 8; j++)
    {
        CLK_2A = 0; //拉低 CLK
        P_DATA_2A = B_DATA; //传输数据一位
        Delay_us(300); //延时 300us
        CLK_2A = 1; //拉高 CLK
        Delay_us(300); //延时 300us
        S_DATA = S_DATA >> 1;
        B_DATA = S_DATA & 0X01;
    }
    P_DATA_2A = 1;
    CLK_2A = 1;
}

//二线串口双字节控制程序
/*--------------------------------------
;模块名称:Line_2A_WT588F(UI16 USER_DATA)
;功 能:实现二线串口通信函数
;入 参:USER_DATA 为发送数据
;出 参:
;CLK_2A 时钟线
;DATA_2A 数据线
;-------------------------------------*/

#define CLK_2A P01
#define DATA_2A P02
void Line_2A_WT588F_Couple(unsigned int USER_DATA)
{
    unsigned char i, num_temp;
    unsigned int ddata_temp, pdata_temp;
    ddata_temp = USER_DATA;
    pdata_temp = ddata_temp & 0X00FF;
    ddata_temp >>= 8;
    pdata_temp <<= 8;
    ddata_temp |= pdata_temp;
    num_temp = 16;
    CLK_2A = 0; //拉低 CLK
    Delay_10us(500); //延时 5ms
    for(j = 0; j < num_temp; j++)
    {
        if(j == 8)
        {
            CLK_2A = 1;
            DATA_2A = 1;
            Delay_N10us(200); //延时 2ms
            CLK_2A = 0;
            Delay_N10us(500); //延时 5ms
        }
        CLK_2A = 0;
        DATA_2A = ddata_temp & 0X0001;
        Delay_N10us(20); //延时 200us
        CLK_2A = 1;
        Delay_N10us(20); //延时 200us
        ddata_temp = ddata_temp >> 1;
    }
    CLK_2A = 1;
    DATA_2A = 1;
}

//二线串口单字节连码示例(F3 + 01 + F3 + 02)
void List_2A_Play_WT588F(void)
{
    Line_2A_WT588F(0xF3);
    Delay_N10us(200); //延时 2ms
    Line_2A_WT588F(0x01);
    Delay_N10us(500); //延时 5ms
    Line_2A_WT588F(0xF3);
    Delay_N10us(200); //延时 2ms
    Line_2A_WT588F(0x02);
    Delay_N10us(500); //延时 5ms
}

//二线串口双字节连码示例(FFF3 + 0001 + FFF3 + 0002)
void List_2A_Play_WT588F_Couple(void)
{
    Line_2A_WT588F_Couple(0xFFF3);
    Delay_ms(5); //延时 5ms
    Line_2A_WT588F_Couple(0x0001);
    Delay_ms(10); //延时 10ms
    Line_2A_WT588F_Couple(0xFFF3);
    Delay_ms(5); //延时 5ms
    Line_2A_WT588F_Couple(0x0002);
    Delay_ms(10); //延时 10ms
}