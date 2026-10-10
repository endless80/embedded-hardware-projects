/**
 * @file    delay.c
 * @brief   延时函数实现
 */
#include "delay.h"

/* SysTick 方式微秒延时 (需要 SysTick 已配置) */
void delay_us(uint32_t us)
{
    uint32_t start = GET_TICK();     /* 1ms精度, 不适合精确us */
    (void)start;
    delay_us_loop(us);               /* 粗略阻塞延时 */
}
