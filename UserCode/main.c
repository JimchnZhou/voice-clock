#include "STC89.h"
#include "ds1302.h"
#include "dht11.h"
#include "lcd1602.h"
#include "button.h"
#include "timer.h"
#include "gpio.h"
#include "delay.h"

void main()
{
	DS1302_Init();	//DS1302初始化
	DHT11_Init()；	//DHT11初始化
	LCD1602_Init();	//LCD1602初始化
	Button_Init();	//按钮程序初始化
	Timer0_Init();	//定时器初始化

	//按键1短按绑定函数：语音播报
	//按键1长按绑定函数：切换模式
	
	//按键2短按绑定函数
	//按键2长按绑定函数
	
	//LED与蜂鸣器测试
	LED1_ON();
	Beep_Once(300);	
	LED1_OFF();
	
	delay(500);
	
	LED2_ON();
	Beep_Once(300);	
	LED2_OFF();
	
	//LCD1602测试
	LCD1602_Write_String(0,0,"1970-01-01");
	LCD1602_Write_String(0,1,"Thursday");
	
	//语音测试

	
	//定时器绑定函数
	
    
	//主循环开始
    while(1)
    {
        ;
    }

}