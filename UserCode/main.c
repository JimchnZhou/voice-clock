#include "STC89.h"
#include "button.h"
#include "gpio.h"
#include "delay.h"

void main()
{
	DS1302_Init();	//DS1302初始化
	LCD1602_Init();		//LCD1602初始化
	Button_Init();	//按钮程序初始化
	Timer0_Init();	//定时器初始化
	
	LED1_ON();
	Beep_Once(300);	//蜂鸣器测试
	LED1_OFF();
	//按键1短按绑定函数：语音播报
	//按键1长按绑定函数：切换模式
	
	//按键2短按绑定函数
	//按键2长按绑定函数
	
	//定时器绑定函数
	//主循环开始
    
	LCD1602_Write_String(0,0,"Welcome!");

    while(1)
    {
        ;
    }

}