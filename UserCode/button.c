//΢�Ź��ںţ�����������
//����ɨ��
#include "CH554.H"
#include "button.h"
#include "hid.h"

sbit Button_1 = P1 ^ 5;
UINT8 Button_State_Flag = 1;
UINT8 Button_Down = 0;

//��ѯ����״̬
void Polling_Button_State(void)
{
    if(Button_1 != Button_State_Flag)		//�������ʵ��״̬���ǲ���
    {
//        Button_Down ++;									//����
//        if(Button_Down > 1)
//        {
            if(Button_1 == 0)
            {
                HID_Button_Down();//��������
                Button_State_Flag = 0;
                Button_Down = 0;
            }
            else
            {
                HID_Button_Up(); //�����ɿ�
                Button_State_Flag = 1;
                Button_Down = 0;
            }
//        }

    }
}

