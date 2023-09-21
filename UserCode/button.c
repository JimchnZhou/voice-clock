//微信公众号：物联网零妖
//按键扫描
#include "CH554.H"
#include "button.h"
#include "hid.h"

sbit Button_1 = P1 ^ 5;
UINT8 Button_State_Flag = 1;
UINT8 Button_Down = 0;

//轮询按键状态
void Polling_Button_State(void)
{
    if(Button_1 != Button_State_Flag)		//如果按键实际状态与标记不符
    {
//        Button_Down ++;									//消抖
//        if(Button_Down > 1)
//        {
            if(Button_1 == 0)
            {
                HID_Button_Down();//按键按下
                Button_State_Flag = 0;
                Button_Down = 0;
            }
            else
            {
                HID_Button_Up(); //按键松开
                Button_State_Flag = 1;
                Button_Down = 0;
            }
//        }

    }
}

