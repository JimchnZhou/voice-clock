#include "stc89.h"
#include "gpio.h"
#include "delay.h"

sbit Led1 = P3 ^ 0;
sbit Led2 = P3 ^ 1;
sbit Beep = P1 ^ 0;

//点亮LED1
void led1On(void)
{
    Led1 = 0;
}

//熄灭LED1
void led1Off(void)
{
    Led1 = 1;
}

//点亮LED2
void led2On(void)
{
    Led2 = 0;
}

//熄灭LED2
void led2Off(void)
{
    Led2 = 1;
}

//打开蜂鸣器，响
void beepOn(void)
{
    Beep = 0;
}

//关闭蜂鸣器，不响
void beepOff(void)
{
    Beep = 1;
}

//蜂鸣器响ms
void beepOnce(unsigned int ms)
{
    beepOn();
    delayMs(ms);
    beepOff();
}

//蜂鸣器响ms
void beep300ms(void)
{
    beepOn();
    delayMs(300);
    beepOff();
}
