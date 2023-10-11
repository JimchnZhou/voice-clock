#ifndef __MODE_H__
#define __MODE_H__

//运行模式初始化
void runningModeInit(void);

// 改变运行模式
void changeRunningMode(void);

// 设置运行模式
unsigned char setRunningMode(unsigned char modeValue);


void Mode_Loop(void);
void Mode_ChangeMode(void);
void Mode_Button_Up(void);
void Mode_Button_Down(void);

// 获取当前运行模式
unsigned char getRunningMode(void);

#endif

