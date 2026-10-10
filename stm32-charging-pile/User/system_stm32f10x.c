/**
 * @file    system_stm32f10x.c
 * @brief   STM32F10x 系统初始化 - SystemInit()
 *          配置 72MHz 主频: 8MHz HSE x9 PLL = 72MHz
 */
#include "stm32f10x.h"

uint32_t SystemCoreClock = 72000000;    /* 初始默认 72MHz */

__IO uint32_t AHBPrescalerValue = 0;

void SystemInit(void)
{
    /* 复位 RCC 时钟配置 */
    RCC_DeInit();

    /* 启用外部 HSE 振荡器 */
    RCC_HSEConfig(RCC_HSE_ON);

    /* 等待 HSE 就绪 */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    /* 配置 Flash 预取缓冲和等待周期 */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLL: 8MHz x 9 = 72MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* 选择 PLL 输出作为系统时钟 */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);

    /* 更新 SystemCoreClock */
    SystemCoreClock = 72000000;

    /* 配置 SysTick */
    SysTick_Config(SystemCoreClock / 1000);
}

void SystemCoreClockUpdate(void)
{
    /* 根据时钟源和分频因子更新 SystemCoreClock */
    SystemCoreClock = 72000000;
}
