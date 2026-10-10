#ifndef __TOUCH_H
#define __TOUCH_H

#include "stm32f10x.h"

void    Touch_Init(void);
uint8_t Touch_Get_Point(uint16_t *x, uint16_t *y);

#endif
