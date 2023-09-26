#include "delay.h"

//微秒延时函数
void	Delay_us(unsigned int n)
{
    while(n--);
}

//毫秒延时函数
void	Delay_ms(unsigned int n)
{
    while(n)
    {
        Delay_us(1000);
        -- n;
    }
}


