/**
 * @file    system_stm32f10x.h
 * @brief   STM32F10x 系统头文件
 */
#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

#include <stdint.h>

extern uint32_t SystemCoreClock;     /* 系统时钟频率 (Hz) */

void SystemInit(void);
void SystemCoreClockUpdate(void);

#endif
