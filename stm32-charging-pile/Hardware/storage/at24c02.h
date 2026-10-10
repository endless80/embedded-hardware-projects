#ifndef __AT24C02_H
#define __AT24C02_H

#include "stm32f10x.h"

#define AT24C02_ADDR    0xA0

void    AT24C02_Init(void);
uint8_t AT24C02_Write_Byte(uint16_t addr, uint8_t data);
uint8_t AT24C02_Read_Byte(uint16_t addr);
void    AT24C02_Write_Bytes(uint16_t addr, uint8_t *buf, uint16_t len);
void    AT24C02_Read_Bytes(uint16_t addr, uint8_t *buf, uint16_t len);

#endif
