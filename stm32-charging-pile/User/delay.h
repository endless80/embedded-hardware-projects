/**
 * @file    delay.h
 * @brief   延时函数
 */
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void delay_us(uint32_t us);

/* 简单阻塞式微秒延时 (72MHz 下约 8 个周期/us) */
static inline void delay_us_loop(uint32_t us)
{
    uint32_t i;
    for(i = 0; i < us * 8; i++) {
        __NOP();
    }
}

#endif
