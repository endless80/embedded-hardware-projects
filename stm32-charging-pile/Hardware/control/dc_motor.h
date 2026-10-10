#ifndef __DC_MOTOR_H
#define __DC_MOTOR_H

#include "stm32f10x.h"

#define MOTOR_FORWARD   0
#define MOTOR_BACKWARD  1

void DC_Motor_Init(void);
void DC_Motor_Run(uint8_t speed, uint8_t dir);  /* speed: 0-100 */
void DC_Motor_Stop(void);

#endif
