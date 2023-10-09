// 微信公众号：物联网零妖
// 业务逻辑
#include "STC89.h"
#include "mode.h"

// #include <Uart_1.h>
// #include <intrins.h>
// #include <PCF8563T.h>
// #include "Button.h"
// #include <WS2812.h>
// #include <IAP_EEPROM.h>

#define Debug_SendData Send_Data1

//定义一个时钟寄存器的结构体
struct RTC
{
    unsigned char	Seconds;//秒
    unsigned char	Minutes;//分钟
    unsigned char	Hour;//小时
    unsigned char	Week;//星期
    unsigned char	Date;//日
    unsigned char	Month;//月
    unsigned char	Year;//年
    unsigned char NC;//预留一个空间。
};

code unsigned char ModeColor[7][3] = {{0x01, 0x10, 0x20}, {0x00, 0xff, 0x00}, {0xff, 0x00, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0x00}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};
static struct RTC RTC_Timer;		   // 实时读取的RTC数值
static unsigned char Mode = 0;		   // 不同模式 0正常显示 1~3调整时，分，秒
static unsigned char Button_Value = 0; // 按键加减之后的数值

// 初始化业务逻辑
void Mode_Init(void)
{
    Mode = 0;
    Button_Value = 0;
}

// 切换模式
void Mode_ChangeMode(void)
{
    Mode++;
    // if (Mode < 7)
    // {
    // 	Button_Value = 0;
    // }
    if(Mode == 1)
    {
        // EEPROM_SaveColor2();
        Button_Value = RTC_Timer.Hour;
        // WS2812_SetColor(0, 255, 255, 255);
        // WS2812_SetColor(1, 255, 255, 255);
        // WS2812_SetColor(2, 0, 0, 0);
        // WS2812_SetColor(3, 0, 0, 0);
        // WS2812_SetColor(4, 0, 0, 0);
        // WS2812_SetColor(5, 0, 0, 0);
    }
    if(Mode == 2)
    {
        Button_Value = RTC_Timer.Minutes;
        // WS2812_SetColor(2, 255, 255, 255);
        // WS2812_SetColor(3, 255, 255, 255);
        // WS2812_SetColor(0, 0, 0, 0);
        // WS2812_SetColor(1, 0, 0, 0);
        // WS2812_SetColor(4, 0, 0, 0);
        // WS2812_SetColor(5, 0, 0, 0);
    }
    if(Mode == 3)
    {
        Button_Value = RTC_Timer.Seconds;
        // WS2812_SetColor(4, 255, 255, 255);
        // WS2812_SetColor(5, 255, 255, 255);
        // WS2812_SetColor(2, 0, 0, 0);
        // WS2812_SetColor(3, 0, 0, 0);
        // WS2812_SetColor(0, 0, 0, 0);
        // WS2812_SetColor(1, 0, 0, 0);
    }
    if(Mode == 4)  // 校准时钟
    {
//		Check_Time(&RTC_Timer);
        // EEPROM_ReadColor2();
    }
    if(Mode > 3)
        Mode = 0;
}

// 按键+逻辑
void Mode_Button_Up(void)
{
    if(Mode == 0)
        return;
    Button_Value++;
    // if (Mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(Mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if(Mode == 1)
    {
        if(Button_Value > 23)
            Button_Value = 0;
        RTC_Timer.Hour = Button_Value;
    }
    if(Mode == 2)
    {
        if(Button_Value > 59)
            Button_Value = 0;
        RTC_Timer.Minutes = Button_Value;
    }
    if(Mode == 3)
    {
        if(Button_Value > 59)
            Button_Value = 0;
        RTC_Timer.Seconds = Button_Value;
    }
}

// 按键-逻辑
void Mode_Button_Down(void)
{
    if(Mode == 0)
        return;
    Button_Value--;
    // if (Mode < 7)
    // {
    // 	if (Button_Value > 6)
    // 		Button_Value = 0;
    // 	WS2812_SetColor(Mode - 1, ModeColor[Button_Value][0], ModeColor[Button_Value][1], ModeColor[Button_Value][2]);
    // }
    if(Mode == 1)
    {
        if(Button_Value > 23)
            Button_Value = 23;
        RTC_Timer.Hour = Button_Value;
    }
    if(Mode == 8)
    {
        if(Button_Value > 59)
            Button_Value = 59;
        RTC_Timer.Minutes = Button_Value;
    }
    if(Mode == 9)
    {
        if(Button_Value > 59)
            Button_Value = 59;
        RTC_Timer.Seconds = Button_Value;
    }
}

// 不同模式管理
void Mode_Loop(void)
{
    switch(Mode)
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
