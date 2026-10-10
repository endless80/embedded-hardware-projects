/**
 * @file    stm32f10x_it.c
 * @brief   中断服务函数
 */
#include "stm32f10x_it.h"
#include "main.h"

void NMI_Handler(void)          {}
void HardFault_Handler(void)    { while(1); }
void MemManage_Handler(void)    { while(1); }
void BusFault_Handler(void)     { while(1); }
void UsageFault_Handler(void)   { while(1); }
void SVC_Handler(void)          {}
void DebugMon_Handler(void)     {}
void PendSV_Handler(void)       {}

/**
 * @brief  SysTick 中断 (1ms 系统滴答)
 */
extern volatile uint32_t g_sys_tick;
void SysTick_Handler(void)      { g_sys_tick++; }

/**
 * @brief  USART1 中断 (调试串口接收)
 */
extern void USART1_IRQ_Handler_user(void);
void USART1_IRQHandler(void)    { USART1_IRQ_Handler_user(); }

/**
 * @brief  USART2 中断 (ESP8266 WiFi 数据收发)
 */
extern void USART2_IRQ_Handler_user(void);
void USART2_IRQHandler(void)    { USART2_IRQ_Handler_user(); }

/**
 * @brief  USART3 中断 (RS485 通信)
 */
extern void USART3_IRQ_Handler_user(void);
void USART3_IRQHandler(void)    { USART3_IRQ_Handler_user(); }

/**
 * @brief  EXTI0 中断 (按键KEY1 / 外部触发)
 */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* 按键1按下处理, 在实际项目中在此添加 */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**
 * @brief  TIM2 中断 (定时任务调度)
 */
extern void TIM2_IRQ_Handler_user(void);
void TIM2_IRQHandler(void)      { TIM2_IRQ_Handler_user(); }

/**
 * @brief  SDIO 中断 (SD卡)
 */
void SDIO_IRQHandler(void)
{
    /* SDIO 中断, 在 sd_sdio.c 中处理 */
    SDIO_IT_Handler();
}
