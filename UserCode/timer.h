//Timer0 22.1184MHZ
#ifndef __TIMER0_H__
#define __TIMER0_H__
#include "STC89.h"

//初始化定时器0
void Timer0_Init(void);

//定时器0中断函数
void Timer0_Interrupt(void);

#endif