#ifndef __BUTTON_H__
#define __BUTTON_H__

//初始化相关的变量
void buttonInit(void);

//给按钮添加处理函数，“C”为单击，“L”为长按、“O”为超时
void addActionToButton(unsigned char CH, unsigned char Type, void (*Fun)(void));

//检测按键业务逻辑，放到10ms循环中不断重复
void Button_Loop(void);

// 按钮1单击
void button1ClickAction();

//按钮2单击
void button2ClickAction();

//按钮1长按
void button1LongPressAction();

//按钮2长按
void button2LongPressAction();

#endif
