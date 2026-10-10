#ifndef __STEPPER_H
#define __STEPPER_H

#include "stm32f10x.h"

#define STEPPER_FULL_STEP   0
#define STEPPER_HALF_STEP   1

void Stepper_Init(void);
void Stepper_Run(uint16_t steps, uint8_t dir);
void Stepper_Stop(void);
void Stepper_Set_Speed(uint16_t step_delay_us);

#endif
