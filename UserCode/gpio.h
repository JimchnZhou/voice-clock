#ifndef __GPIO_H__
#define __GPIO_H__

//点亮LED1
void LED1_ON(void);

//熄灭LED1
void LED1_OFF(void);

//点亮LED2
void LED2_ON(void);

//熄灭LED2
void LED2_OFF(void);

//打开蜂鸣器，响
Void Beep_ON(void);

//关闭蜂鸣器，不响
void Beep_OFF(void);	

//蜂鸣器响 tms
void Beep_Once(unsigned int ms);


#endif