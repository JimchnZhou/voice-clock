#ifndef __BUTTON_H__
#define __BUTTON_H__

//初始化相关的变量
void buttonInit(void);

//给按钮添加处理函数，“C”为单击，“L”为长按、“O”为超时
void addEventToButton(unsigned char CH, unsigned char Type, void (*Fun)(void));

//检测按键业务逻辑，放到10ms循环中不断重复
void Button_Loop(void);

// 按钮 0 单击事件
void button0OnClickEvent(void);

// 按钮 1 单击事件
void button1OnClickEvent(void);

//按钮 2 单击事件
void button2OnClickEvent(void);

//按钮 1 长按事件
void button0LongPressEvent(void);

//按钮 1 长按事件
void button1LongPressEvent(void);

//按钮 2 长按事件
void button2LongPressEvent(void);

#endif
