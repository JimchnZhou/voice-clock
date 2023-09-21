#include "CH554.H"
#include "timer.h"
#include "button.h"

void Timer0_Init(void)
{

//	AUXR |= 0x80;//��ʱ��ʱ��1Tģʽ 
    TMOD &= 0xF1;//���ö�ʱ��ģʽ 1111 0001
    TL0 = 0x20;//���ö�ʱ��ֵ
    TH0 = 0xD1;//���ö�ʱ��ֵ
    TF0 = 0;//���TF0��־
    TR0 = 1;//������ʱ��
    ET0 = 1;//ʹ�ܶ�ʱ���ж�

}

/*******************************************************************************
* Function Name  : mTimer0Interrupt()
* Description    : CH554��ʱ������0��ʱ�������жϴ�����
*******************************************************************************/
void	mTimer0Interrupt(void) interrupt INT_NO_TMR0 using 1                  //timer0�жϷ������,ʹ�üĴ�����1
{
    Polling_Button_State();
	    TL0 = 0xF0;//���ö�ʱ��ֵ
    TH0 = 0xD8;//���ö�ʱ��ֵ
}


