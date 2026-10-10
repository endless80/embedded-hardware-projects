#ifndef __RS485_H
#define __RS485_H

#include "stm32f10x.h"

/* RS485 收发控制 (RE/DE 共用) */
#define RS485_RE_PIN        GPIO_Pin_12       /* PB12 */
#define RS485_RE_PORT       GPIOB

#define RS485_SET_TX()      GPIO_SetBits(RS485_RE_PORT, RS485_RE_PIN)   /* DE=1 发送 */
#define RS485_SET_RX()      GPIO_ResetBits(RS485_RE_PORT, RS485_RE_PIN) /* RE=0 接收 */

void    RS485_Init(uint32_t baud);
void    RS485_Send(uint8_t *buf, uint16_t len);
uint8_t RS485_Recv(uint8_t *buf, uint16_t max_len, uint32_t timeout_ms);

#endif
