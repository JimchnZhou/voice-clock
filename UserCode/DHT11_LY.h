//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef __DHT11_H__
#define __DHT11_H__

#include<STC15W.h>


void Read_DHT11(void);

//Humi 三个字节    Temp 六个字节 
void Read_DHT11_Str(unsigned char *Temp,unsigned char *Humi);



#endif