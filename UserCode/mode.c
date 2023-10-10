#include "STC89.h"
#include "mode.h"

#define Debug_SendData Send_Data1

static unsigned char Mode = 0; // 不同模式 0正常显示 1~3调整时，分，秒

code unsigned char ModeColor[7][3] = {{0x01, 0x10, 0x20}, {0x00, 0xff, 0x00}, {0xff, 0x00, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0x00}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};

static unsigned char Button_Value = 0; // 按键加减之后的数值

// 初始化业务逻辑
void modeInit(void)
{
    Mode = 0;
    Button_Value = 0;
}

// 改变运行模式
void changeMode(void)
{
    Mode++;

    if (Mode == 1)
    {
    }
    if (Mode == 2)
    {
    }
    if (Mode == 3)
    {
    }
    if (Mode == 4) // 校准时钟
    {
    }
    if (Mode > 3)
        Mode = 0;
}

// 获取当前运行模式
unsigned char getRunningMode()
{
    return Mode;
}

// 按键+逻辑
void Mode_Button_Up(void)
{
    if (Mode == 0)
        return;
    Button_Value++;
    // if (Mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(Mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if (Mode == 1)
    {
        if (Button_Value > 23)
            Button_Value = 0;
        RTC_Timer.Hour = Button_Value;
    }
    if (Mode == 2)
    {
        if (Button_Value > 59)
            Button_Value = 0;
        RTC_Timer.Minutes = Button_Value;
    }
    if (Mode == 3)
    {
        if (Button_Value > 59)
            Button_Value = 0;
        RTC_Timer.Seconds = Button_Value;
    }
}

// 按键-逻辑
void Mode_Button_Down(void)
{
    if (Mode == 0)
        return;
    Button_Value--;
    // if (Mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(Mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if (Mode == 1)
    {
        if (Button_Value > 23)
            Button_Value = 23;
        RTC_Timer.Hour = Button_Value;
    }
    if (Mode == 8)
    {
        if (Button_Value > 59)
            Button_Value = 59;
        RTC_Timer.Minutes = Button_Value;
    }
    if (Mode == 9)
    {
        if (Button_Value > 59)
            Button_Value = 59;
        RTC_Timer.Seconds = Button_Value;
    }
}

// 不同模式管理
void Mode_Loop(void)
{
    switch (Mode)
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
