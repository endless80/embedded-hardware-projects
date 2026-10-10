#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdarg.h>

void Debug_USART_Init(uint32_t baud);
void Debug_Printf(const char *fmt, ...);
void USART1_IRQ_Handler_user(void);

void USART2_Init(uint32_t baud);
void USART2_Send(const uint8_t *buf, uint16_t len);
uint16_t USART2_Recv(uint8_t *buf, uint16_t max_len);
void USART2_IRQ_Handler_user(void);

void USART3_Init(uint32_t baud);
void USART3_Send(const uint8_t *buf, uint16_t len);
uint16_t USART3_Recv(uint8_t *buf, uint16_t max_len);
void USART3_IRQ_Handler_user(void);

#endif
