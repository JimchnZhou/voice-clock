#ifndef __IIC_H__
#define __IIC_H__

void IIC_Start(void);

void IIC_Stop(void);

void SEND0(void);

void SEND1(void);

bit Check_Ack(void);
void Write_byte(unsigned char dat);

unsigned char Read_byte(void);

#endif
