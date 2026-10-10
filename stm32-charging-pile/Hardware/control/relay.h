#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

void Relay_Init(void);
void Relay_Control(uint8_t on);    /* 1=闭合, 0=断开 */

#endif
