// ΢�Ź��ںţ�����������
// ҵ���߼�
#include "STC89.h"
#include "mode.h"

// #include <Uart_1.h>
// #include <intrins.h>
// #include <PCF8563T.h>
// #include "Button.h"
// #include <WS2812.h>
// #include <IAP_EEPROM.h>

#define Debug_SendData Send_Data1

//����һ��ʱ�ӼĴ����Ľṹ��
struct RTC
{
    unsigned char	Seconds;//��
    unsigned char	Minutes;//����
    unsigned char	Hour;//Сʱ
    unsigned char	Week;//����
    unsigned char	Date;//��
    unsigned char	Month;//��
    unsigned char	Year;//��
    unsigned char NC;//Ԥ��һ���ռ䡣
};

code unsigned char ModeColor[7][3] = {{0x01, 0x10, 0x20}, {0x00, 0xff, 0x00}, {0xff, 0x00, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0x00}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};
static struct RTC RTC_Timer;		   // ʵʱ��ȡ��RTC��ֵ
static unsigned char Mode = 0;		   // ��ͬģʽ 0������ʾ 1~3����ʱ���֣���
static unsigned char Button_Value = 0; // �����Ӽ�֮�����ֵ

// ��ʼ��ҵ���߼�
void Mode_Init(void)
{
    Mode = 0;
    Button_Value = 0;
}

// �л�ģʽ
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
    if(Mode == 4)  // У׼ʱ��
    {
//		Check_Time(&RTC_Timer);
        // EEPROM_ReadColor2();
    }
    if(Mode > 3)
        Mode = 0;
}

// ����+�߼�
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

// ����-�߼�
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

// ��ͬģʽ����
void Mode_Loop(void)
{
    switch(Mode)
    {
    case 0:
    {
//		Get_Time(&RTC_Timer); // ��ȡʱ��
        // ��ȡ��ʪ��
        // ��ʾ

        break;
    }
    case 1:
    {
        // ��ʱ
        break;
    }
    case 2:
    {
        // ����
        break;
    }
    case 3:
    {
        // ����
        break;
    }
    }
}
//

//
