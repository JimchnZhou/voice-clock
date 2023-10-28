//ϵͳ����������ײ��߼�  ΢�Ź��ںţ�����������
#ifndef __SYS_LOOP_H__
#define __SYS_LOOP_H__

unsigned char addEventToLoopList(unsigned long Time, void (*Fun)(void));
unsigned char delEventFromLoopList(void (*Fun)(void));
unsigned char addEventToDelayList(unsigned long Time, void (*Fun)(void));
unsigned char addEventToMessagesList(unsigned char Message, void (*Fun)(void));
unsigned char delEventFromMessageList(void (*Fun)(void));
unsigned char SYS_SendMessage(unsigned char Message);

void loopListInit(void);
void SYS_LOOP_While_APP(void);
void SYS_TimerInterrupt_App(void);


#endif
