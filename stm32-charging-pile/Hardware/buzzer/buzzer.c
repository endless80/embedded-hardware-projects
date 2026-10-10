/**
 * @file    buzzer.c
 * @brief   蜂鸣器驱动 (有源蜂鸣器, 高电平驱动)
 *          原理图: PF3 → S8050 → BEEP
 */
#include "buzzer.h"
#include "pin_mapping.h"

static GPIO_InitTypeDef GPIO_InitStructure;

void Buzzer_Init(void)
{
    RCC_APB2PeriphClockCmd(BEEP_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
    Buzzer_Off();
}

void Buzzer_On(void)    { BEEP_ON(); }
void Buzzer_Off(void)   { BEEP_OFF(); }

void Buzzer_Beep(uint16_t duration_ms)
{
    Buzzer_On();
    DELAY_MS(duration_ms);
    Buzzer_Off();
}
