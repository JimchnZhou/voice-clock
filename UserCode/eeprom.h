#ifndef __EEPROM_H__
#define __EEPROM_H__

unsigned char rdeeprom(unsigned char addr);
void wrteeprom(unsigned char addr,unsigned char dat);
void  COM(void);

#endif

