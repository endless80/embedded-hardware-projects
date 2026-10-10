/**
 * @file    dc_motor.c
 * @brief   直流电机驱动 (L9110S)
 *          原理图: PB8(IA) PB9(IB) → L9110S → 130电机
 *          IA/IB 输出 PWM 控制速度和方向
 */
#include "dc_motor.h"
#include "pin_mapping.h"

static TIM_TimeBaseInitTypeDef  TIM_BaseStructure;
static TIM_OCInitTypeDef        TIM_OCStructure;
static GPIO_InitTypeDef         GPIO_InitStructure;

void DC_Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(MOTOR_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* PB8 (TIM4_CH3), PB9 (TIM4_CH4) 复用推挽 */
    GPIO_InitStructure.GPIO_Pin   = MOTOR_IN_A_PIN | MOTOR_IN_B_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

    /* TIM4 配置, 72MHz / 72 = 1MHz, 自动重装 1000 = 1KHz PWM */
    TIM_BaseStructure.TIM_Prescaler         = 72 - 1;
    TIM_BaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_BaseStructure.TIM_Period            = 1000 - 1;
    TIM_BaseStructure.TIM_ClockDivision     = 0;
    TIM_BaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_BaseStructure);

    /* CH3 (PB8) - IA */
    TIM_OCStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCStructure.TIM_Pulse        = 0;
    TIM_OCStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OC3Init(TIM4, &TIM_OCStructure);

    /* CH4 (PB9) - IB */
    TIM_OC4Init(TIM4, &TIM_OCStructure);

    TIM_Cmd(TIM4, ENABLE);
}

void DC_Motor_Run(uint8_t speed, uint8_t dir)
{
    uint16_t pulse = (uint16_t)((uint32_t)speed * 1000 / 100);
    if(pulse > 1000) pulse = 1000;

    if(dir == MOTOR_FORWARD) {
        TIM_SetCompare3(TIM4, pulse);   /* IA = PWM */
        TIM_SetCompare4(TIM4, 0);       /* IB = 0 */
    } else {
        TIM_SetCompare3(TIM4, 0);
        TIM_SetCompare4(TIM4, pulse);   /* IB = PWM */
    }
}

void DC_Motor_Stop(void)
{
    TIM_SetCompare3(TIM4, 0);
    TIM_SetCompare4(TIM4, 0);
}
