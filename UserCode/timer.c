#include "Timer0.h"
#include "sys_loop.h"
#include "Button.h"

//��ʼ����ʱ��0��10ms
void Timer0_Init(void)
{
    AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
    TMOD &= 0xF0;		//���ö�ʱ��ģʽ
    TL0 = 0x00;		//���ö�ʱ��ֵ
    TH0 = 0xDC;		//���ö�ʱ��ֵ
    TF0 = 0;		//���TF0��־
    TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0 = 1;//ʹ�ܶ�ʱ���ж�
}

//��ʱ��0�жϺ���
void Timer0_Interrupt(void) interrupt 1
{
    SYS_TimerInterrupt_App();
}

