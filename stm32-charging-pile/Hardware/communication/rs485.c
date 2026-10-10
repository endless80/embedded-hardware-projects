/**
 * @file    rs485.c
 * @brief   RS-485 通信驱动 (SP3485)
 *          原理图: USART3 (PB10/PB11) + PB12(RE/DE)
 */
#include "rs485.h"
#include "usart.h"
#include "pin_mapping.h"

void RS485_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* RE/DE 控制引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = RS485_RE_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_RE_PORT, &GPIO_InitStructure);
    RS485_SET_RX();

    /* USART3 初始化 */
    USART3_Init(baud);
}

void RS485_Send(uint8_t *buf, uint16_t len)
{
    RS485_SET_TX();
    DELAY_MS(1);
    USART3_Send(buf, len);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    DELAY_MS(1);
    RS485_SET_RX();
}

uint8_t RS485_Recv(uint8_t *buf, uint16_t max_len, uint32_t timeout_ms)
{
    uint16_t len = 0;
    uint32_t start = GET_TICK();

    while((GET_TICK() - start) < timeout_ms) {
        len = USART3_Recv(buf, max_len);
        if(len > 0) break;
    }
    return (uint8_t)len;
}
