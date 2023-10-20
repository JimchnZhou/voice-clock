//ϵͳ����������ײ��߼�  ΢�Ź��ںţ�����������
#ifndef __SYS_LOOP_H__
#define __SYS_LOOP_H__

unsigned char addEventToLoop(unsigned long Time, void (*Fun)(void));
unsigned char delEventFromLoop(void (*Fun)(void));
unsigned char addEventToDelay(unsigned long Time, void (*Fun)(void));
unsigned char addEventToMessages(unsigned char Message, void (*Fun)(void));
unsigned char delEventFromMessage(void (*Fun)(void));
unsigned char SYS_SendMessage(unsigned char Message);

void SYS_LOOP_Init(void);
void SYS_LOOP_While_APP(void);
void SYS_TimerInterrupt_App(void);


#endif
