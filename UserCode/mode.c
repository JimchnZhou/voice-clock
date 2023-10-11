#include "STC89.h"
#include "mode.h"

#define Debug_SendData Send_Data1

static unsigned char Mode = 0; // ��ͬģʽ 0������ʾ 1~3����ʱ���֣���

code unsigned char ModeColor[7][3] = {{0x01, 0x10, 0x20}, {0x00, 0xff, 0x00}, {0xff, 0x00, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0x00}, {0x00, 0xff, 0xff}, {0xff, 0xff, 0xff}};

static unsigned char Button_Value = 0; // �����Ӽ�֮�����ֵ

// ��ʼ��ҵ���߼�
void modeInit(void)
{
    Mode = 0;
    Button_Value = 0;
}

// �ı�����ģʽ
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
    if (Mode == 4) // У׼ʱ��
    {
    }
    if (Mode > 3)
        Mode = 0;
}

// ��ȡ��ǰ����ģʽ
unsigned char getRunningMode()
{
    return Mode;
}

// ����+�߼�
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

// ����-�߼�
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

// ��ͬģʽ����
void Mode_Loop(void)
{
    switch (Mode)
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
