#include "timer.h"
#include "sys_loop.h"
#include "button.h"

//初始化定时器0，10ms
void Timer0_Init(void)
{
    AUXR &= 0x7F;		//定时器时钟12T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0x00;		//设置定时初值
    TH0 = 0xDC;		//设置定时初值
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时
    ET0 = 1;//使能定时器中断
}

//定时器0中断函数
void Timer0_Interrupt(void) interrupt 1
{
    SYS_TimerInterrupt_App();
}

