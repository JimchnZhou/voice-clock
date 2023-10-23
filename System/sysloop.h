//系统程序调度器底层逻辑  微信公众号：物联网零妖
#ifndef __SYS_LOOP_H__
#define __SYS_LOOP_H__

unsigned char SYS_New_LoopFun(unsigned long Time, void (*Fun)(void));
unsigned char SYS_Del_LoopFun(void (*Fun)(void));
unsigned char SYS_New_DelayFun(unsigned long Time, void (*Fun)(void));
unsigned char SYS_New_MessageFun(unsigned char Message, void (*Fun)(void));
unsigned char SYS_Del_MessageFun(void (*Fun)(void));
unsigned char SYS_SendMessage(unsigned char Message);

void SYS_LOOP_Init(void);
void SYS_LOOP_While_APP(void);
void SYS_TimerInterrupt_App(void);


#endif