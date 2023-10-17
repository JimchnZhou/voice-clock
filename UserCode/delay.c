#include <intrins.h>
#include "delay.h"

//微秒延时函数
void delayUs(unsigned int n)
{
    while(n--);
}

//毫秒延时函数
void delayMs(unsigned int n)
{
    unsigned char data i, j;
    while(n--)
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while(--j);
        }
        while(--i);
    }
}


