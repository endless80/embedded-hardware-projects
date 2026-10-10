/**
 * @file    stepper.c
 * @brief   5线4相步进电机驱动 (ULN2003)
 *          原理图: PE0/PE1/PB4/PB5 → ULN2003
 *          4相驱动: A(PE0)-B(PE1)-C(PB4)-D(PB5)
 */
#include "stepper.h"
#include "pin_mapping.h"
#include "delay.h"

static uint16_t g_step_delay = 2000;  /* us */

/* 4相8拍 (半步) 时序表 */
static const uint8_t step_seq[8][4] = {
    {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
    {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};

static void Stepper_Set_Phase(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    if(a) GPIO_SetBits(STEPPER_PORT_A, STEPPER_IN1_PIN);
    else  GPIO_ResetBits(STEPPER_PORT_A, STEPPER_IN1_PIN);
    if(b) GPIO_SetBits(STEPPER_PORT_A, STEPPER_IN2_PIN);
    else  GPIO_ResetBits(STEPPER_PORT_A, STEPPER_IN2_PIN);
    if(c) GPIO_SetBits(STEPPER_PORT_B, STEPPER_IN3_PIN);
    else  GPIO_ResetBits(STEPPER_PORT_B, STEPPER_IN3_PIN);
    if(d) GPIO_SetBits(STEPPER_PORT_B, STEPPER_IN4_PIN);
    else  GPIO_ResetBits(STEPPER_PORT_B, STEPPER_IN4_PIN);
}

void Stepper_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(STEPPER_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = STEPPER_IN1_PIN | STEPPER_IN2_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STEPPER_PORT_A, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = STEPPER_IN3_PIN | STEPPER_IN4_PIN;
    GPIO_Init(STEPPER_PORT_B, &GPIO_InitStructure);

    Stepper_Stop();
}

void Stepper_Run(uint16_t steps, uint8_t dir)
{
    uint16_t i;
    for(i = 0; i < steps; i++) {
        uint8_t idx = (dir == 0) ? (i & 0x07) : (7 - (i & 0x07));
        Stepper_Set_Phase(step_seq[idx][0], step_seq[idx][1],
                          step_seq[idx][2], step_seq[idx][3]);
        delay_us_loop(g_step_delay);   /* 微秒延时 */
    }
    Stepper_Stop();
}

void Stepper_Stop(void)
{
    Stepper_Set_Phase(0, 0, 0, 0);
}

void Stepper_Set_Speed(uint16_t step_delay_us)
{
    g_step_delay = step_delay_us;
}
