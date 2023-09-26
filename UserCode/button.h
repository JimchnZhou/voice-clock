#ifndef __BUTTON_H__
#define __BUTTON_H__

//初始化相关的变量
void Button_Init(void);

//给按钮添加处理函数，“C”为单击，“L”为长按、“O”为超时
void Button_Set_Fun(unsigned char CH, unsigned char Type, void (*Fun)(void));

//检测按键业务逻辑，放到10ms循环中不断重复
void Button_Loop(void);

#endif