#include "CH554.H"
#include "timer.h"
#include "button.h"

void Timer0_Init(void)
{

//	AUXR |= 0x80;//定时器时钟1T模式 
    TMOD &= 0xF1;//设置定时器模式 1111 0001
    TL0 = 0x20;//设置定时初值
    TH0 = 0xD1;//设置定时初值
    TF0 = 0;//清除TF0标志
    TR0 = 1;//启动定时器
    ET0 = 1;//使能定时器中断

}

/*******************************************************************************
* Function Name  : mTimer0Interrupt()
* Description    : CH554定时计数器0定时计数器中断处理函数
*******************************************************************************/
void	mTimer0Interrupt(void) interrupt INT_NO_TMR0 using 1                  //timer0中断服务程序,使用寄存器组1
{
    Polling_Button_State();
	    TL0 = 0xF0;//设置定时初值
    TH0 = 0xD8;//设置定时初值
}


