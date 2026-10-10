#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);
void LED_Control(uint8_t led_id, uint8_t on);
void LED_All_Off(void);

void RGB_Init(void);
void RGB_Control(uint8_t r, uint8_t g, uint8_t b);
void RGB_Off(void);

#endif
