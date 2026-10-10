#ifndef __IR_REMOTE_H
#define __IR_REMOTE_H

#include "stm32f10x.h"

void    IR_Remote_Init(void);
uint8_t IR_Remote_Scan(uint8_t *code);

#endif
