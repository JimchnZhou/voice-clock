#ifndef __GPIO_H__
#define __GPIO_H__

//点亮LED1
void led1On(void);

//熄灭LED1
void led1Off(void);

//点亮LED2
void led2On(void);

//熄灭LED2
void led2Off(void);

//打开蜂鸣器，响
void beepOn(void);

//关闭蜂鸣器，不响
void beepOff(void);

//蜂鸣器响 ms
void beepOnce(unsigned int ms);

//蜂鸣器响 300ms
void beep300ms(void);

#endif
