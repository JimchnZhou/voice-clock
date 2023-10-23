#include "STC89.h"

#include "lcd1602.h"
#include "ds1302.h"
#include "dht11.h"

#include "button.h"
#include "wt588.h"
#include "gpio.h"
#include "delay.h"

#include "timer.h"
#include "mode.h"
#include "task.h"

void main()
{

    lcd1602Init(); // LCD1602初始化
    ds1302Init();    // DS1302初始化
    buttonInit();    // 按钮初始化
    timer0Init();    // 10ms定时器初始化
    SYS_LOOP_Init(); // 系统调度初始化
    wt588fInit();    // WT588初始化

    // LED与蜂鸣器测试
    led1On();
    beepOnce(100);
    led1Off();

    delayMs(200);

    led2On();
    beepOnce(100);
    led2Off();

    // LCD1602测试

    // 语音测试

    // 绑定函数
    addEventToButton(0, 'C', button0OnClickEvent);   // 按键0添加单击函数：切换模式
    addEventToButton(1, 'C', button1OnClickEvent);   // 按键1添加单击函数
    addEventToButton(2, 'C', button2OnClickEvent);   // 按键2添加单击函数
    addEventToButton(0, 'L', button0LongPressEvent); // 按键0添加长按函数
    addEventToButton(1, 'L', button1LongPressEvent); // 按键1添加长按函数
    addEventToButton(2, 'L', button2LongPressEvent); // 按键2添加长按函数

    addEventToLoop(33, readTimeFromDs1302); // 添加读取时间到系统调度，每33*10ms读取一次时间
		addEventToLoop(50, readDataFromDht11); // 添加读取时间到系统调度，每50*10ms读取一次时间
    addEventToLoop(25, lcd1602Display);     // 添加显示到系统调度，每25*10ms刷新一次显示
    addEventToLoop(2, Button_Loop);         // 添加按键到系统调度，每2*10ms检测一次按键

    // List_2A_Play_WT588F();
    // Two_Wire_Send_Byte(0x01);

    EA = 1;
    SYS_LOOP_While_APP(); // 循环执行系统调度器
}
