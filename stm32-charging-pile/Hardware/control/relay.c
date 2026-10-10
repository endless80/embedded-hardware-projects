/**
 * @file    relay.c
 * @brief   继电器驱动
 *          原理图: PC5, NPN 三极管驱动
 */
#include "relay.h"
#include "pin_mapping.h"

void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RELAY_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = RELAY_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
    RELAY_OFF();
}

void Relay_Control(uint8_t on)
{
    if(on) RELAY_ON();
    else   RELAY_OFF();
}
