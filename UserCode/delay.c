#include <intrins.h>
#include "delay.h"

//微秒延时函数
void Delay_us(unsigned int n)
{
    while(n--);
}

//毫秒延时函数
void Delay_ms(unsigned int n)
{
    unsigned char data i, j;

    _nop_();
    i = 2;
    j = 199;
    do
    {
        while(--j);
    }
    while(--i);
}


