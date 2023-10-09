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
    //	Ds1302_Init();	//DS1302初始化
    //	DHT11_Init()；	//DHT11初始化
    LCD1602_Init(); // LCD1602初始化
    //	Button_Init();	//按钮程序初始化
    //	Timer0_Init();	//定时器初始化

    Button_Set_Fun(0, 'C', Mode_ChangeMode());  // 按键1短按绑定函数：切换模式
    Button_Set_Fun(1, 'C', Mode_Button_Up());   // 按键2短按绑定函数
    Button_Set_Fun(2, 'C', Mode_Button_Down()); // 按键3短按绑定函数

    // LED与蜂鸣器测试
    LED1_ON();
    Beep_Once(300);
    LED1_OFF();

    Delay_ms(500);

    LED2_ON();
    Beep_Once(300);
    LED2_OFF();

    // LCD1602测试
    LCD1602_Write_String(0, 0, "1970-01-01 23:59:59");
    LCD1602_Write_String(0, 1, "T:36.5 RH:55%");

    // 语音测试

    // 定时器绑定函数

    // 主循环开始
    while (1)
    {
        ;
    }
}
