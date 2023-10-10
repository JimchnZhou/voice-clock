#include "STC89.h"

#include "ds1302.h"
#include "dht11.h"
#include "lcd1602.h"
#include "button.h"
#include "wt588.h"
#include "gpio.h"
#include "delay.h"

#include "timer.h"
#include "mode.h"
#include "sys_loop.h"

void main()
{

    LCD1602_Init(); // LCD1602初始化
    Ds1302_Init();	//DS1302初始化
    Button_Init();	//按钮初始化
    Timer0_Init();	//10ms定时器初始化
    SYS_LOOP_Init();//系统调度初始化

    // LED与蜂鸣器测试
    LED1_ON();
    Beep_Once(100);
    LED1_OFF();

    Delay_ms(200);

    LED2_ON();
    Beep_Once(100);
    LED2_OFF();

// LCD1602测试


    // 语音测试

    // 绑定函数
    Button_Set_Fun(0, 'C', Beep_300ms);  // 按键1短按绑定函数：切换模式
    Button_Set_Fun(1, 'C', Beep_ON);   // 按键2短按绑定函数
    Button_Set_Fun(2, 'C', Beep_OFF); // 按键3短按绑定函数


    SYS_New_LoopFun(25, Ds1302_Read_Time); //添加读取时间到系统调度，每25*10ms读取一次时间
    SYS_New_LoopFun(25, LCD1602_Loop); //添加显示到系统调度，每25*10ms刷新一次显示
    SYS_New_LoopFun(2, Button_Loop); //添加按键到系统调度，每2*10ms检测一次按键

List_2A_Play_WT588F();
//Two_Wire_Send_Byte(0x01);

    EA = 1;
    SYS_LOOP_While_APP();//循环执行系统调度器
}
