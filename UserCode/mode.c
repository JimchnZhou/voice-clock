#include "STC89.h"
#include "mode.h"
#include "ds1302.h"
#include "lcd1602.h"

#define Debug_SendData Send_Data1

static unsigned char mode = 0; // 不同模式 0正常显示 1~3调整时，分，秒

// 运行模式初始化
void runningModeInit(void)
{
    mode = 0;
    // Button_Value = 0;
}

// 改变运行模式
void changeRunningMode(void)
{
    mode++;

    if (mode == 1)
    {
    }
    if (mode == 2)
    {
    }
    if (mode == 3)
    {
    }
    if (mode == 4) // 校准时钟
    {
    }
    if (mode > 3)
        mode = 0;
}

// 设置运行模式
unsigned char setRunningMode(unsigned char modeValue)
{
    unsigned char i;
    for (i = 0; i < 7; i++)
    {
        setNewTimeData(i,getCurrentTimeData());
    }
    LCD1602_Clear();
    mode = modeValue;
    return 0;
}

// 获取当前运行模式
unsigned char getRunningMode()
{
    return mode;
}

// 按键+逻辑
void Mode_Button_Up(void)
{
    if (mode == 0)
        return;
    //    Button_Value++;
    // if (mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if (mode == 1)
    {
        //        if (Button_Value > 23)
        //            Button_Value = 0;
        //        RTC_Timer.Hour = Button_Value;
    }
    if (mode == 2)
    {
        //        if (Button_Value > 59)
        //            Button_Value = 0;
        //        RTC_Timer.Minutes = Button_Value;
    }
    if (mode == 3)
    {
        //        if (Button_Value > 59)
        //            Button_Value = 0;
        //        RTC_Timer.Seconds = Button_Value;
    }
}

// 按键-逻辑
void Mode_Button_Down(void)
{
    if (mode == 0)
        return;
    //    Button_Value--;
    // if (mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if (mode == 1)
    {
        //        if (Button_Value > 23)
        //            Button_Value = 23;
        //        RTC_Timer.Hour = Button_Value;
    }
    if (mode == 8)
    {
        //        if (Button_Value > 59)
        //            Button_Value = 59;
        //        RTC_Timer.Minutes = Button_Value;
    }
    if (mode == 9)
    {
        //        if (Button_Value > 59)
        //            Button_Value = 59;
        //        RTC_Timer.Seconds = Button_Value;
    }
}

// 不同模式管理
void Mode_Loop(void)
{
    switch (mode)
    {
    case 0:
    {
        //		Get_Time(&RTC_Timer); // 获取时间
        // 获取温湿度
        // 显示

        break;
    }
    case 1:
    {
        // 调时
        break;
    }
    case 2:
    {
        // 调分
        break;
    }
    case 3:
    {
        // 调秒
        break;
    }
    }
}
//

//
