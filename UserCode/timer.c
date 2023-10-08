#include "STC89.h"
#include "timer.h"
//#include "Uart_1.h"

#define	DebugSendByte	Send_Data1

#define	FOSC	22118400
#define	Timer_0_List_Count	4

struct
{
    unsigned char Flag[Timer_0_List_Count];
    void (*Fun_Point_List[Timer_0_List_Count])(void);
    unsigned int Counter[Timer_0_List_Count];
    unsigned int Timer[Timer_0_List_Count];
} Timer0_Struct;

unsigned char Timer0_Handler_Flag = 0;

//初始化定时器0
void Timer0_Init(void)
{
    unsigned char i = 0;

    for(i = 0; i < Timer_0_List_Count; i++)
    {
        Timer0_Struct.Flag[i] = 0;
        Timer0_Struct.Counter[i] = 0;
    }

    AUXR |= 0x80;//定时器时钟1T模式
    TMOD &= 0xF0;//设置定时器模式
    TL0 = 0x9A;//设置定时初值
    TH0 = 0xA9;//设置定时初值
    TF0 = 0;//清除TF0标志
    TR0 = 1;//启动定时器
    ET0 = 1;//使能定时器中断

}

//添加一个函数到定时器中
unsigned char Timer_0_Add_Fun(unsigned int Time, void (*Fun)(void))
{
    unsigned char i = 0;
    for(i = 0; i < Timer_0_List_Count; i++)
    {
        if(Timer0_Struct.Flag[i] == 0)
        {
            Timer0_Struct.Flag[i] = 1;
            Timer0_Struct.Counter[i] = 0;
            Timer0_Struct.Fun_Point_List[i] = Fun;
            Timer0_Struct.Timer[i] = Time - 1;
            return 1;
        }
    }
    return 0;
}

//系统定时器中断里面执行的函数
void Timer0_SYS_APP_LOOP(void)
{
    unsigned char i = 0, j = 0;
    if(Timer0_Handler_Flag == 0)
        return;
    Timer0_Handler_Flag = 0;
    for(i = 0; i < Timer_0_List_Count; i++) //循环定时的函数
    {
        if(Timer0_Struct.Flag[i])
        {
            Timer0_Struct.Counter[i] ++ ;
            if(Timer0_Struct.Counter[i] > Timer0_Struct.Timer[i])
            {
                Timer0_Struct.Counter[i] = 0;
                Timer0_Struct.Fun_Point_List[i]();
            }
        }
    }


}
//被链接到了定时器0.
void Timer0_Interrupt(void) interrupt 1
{
    Timer0_Handler_Flag = 1;
}
//


