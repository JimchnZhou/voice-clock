#ifndef __DHT11_H__
#define __DHT11_H__

// 从 DHT11 读取数据
unsigned char readDataFromDht11(void);

// 从DHT11获取湿度数据
unsigned char getHumiData(void);

// 从DHT11获取温度数据
unsigned char getTempData(void);

#endif
