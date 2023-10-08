#include "STC89.h"
#include "gpio.h"
#include "delay.h"

sbit LED1 = P3 ^ 0;
sbit LED2 = P3 ^ 1;
sbit Beep = P1 ^ 0;

//点亮LED1
void LED1_ON(void)
{
    LED1 = 0;
}

//熄灭LED1
void LED1_OFF(void)
{
    LED1 = 1;
}

//点亮LED2
void LED2_ON(void)
{
    LED2 = 0;
}

//熄灭LED2
void LED2_OFF(void)
{
    LED2 = 1;
}

//打开蜂鸣器，响
void Beep_ON(void)
{
    Beep = 1;
}

//关闭蜂鸣器，不响
void Beep_OFF(void)
{
    Beep = 0;
}

//蜂鸣器响ms
void Beep_Once(unsigned int ms)
{
    Beep_ON();
    Delay_ms(ms);
    Beep_OFF();
}