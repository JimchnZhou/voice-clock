#ifndef __MODE_H__
#define __MODE_H__

// 改变运行模式
void changeMode(void)

void Mode_Init(void);
void Mode_Loop(void);
void Mode_ChangeMode(void);
void Mode_Button_Up(void);
void Mode_Button_Down(void);

// 获取当前运行模式
unsigned char getRunningMode(void);

#endif

