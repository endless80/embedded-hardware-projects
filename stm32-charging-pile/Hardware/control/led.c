/**
 * @file    led.c
 * @brief   LED 指示灯 + RGB-LED 驱动
 *          原理图: PF4/5/11/12 (LED0-3, 低电平亮)
 *                  PF0/1/2 (RGB R/G/B, 高电平亮)
 */
#include "led.h"
#include "pin_mapping.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LED0_PIN | LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    LED_All_Off();
}

void LED_Control(uint8_t led_id, uint8_t on)
{
    switch(led_id) {
        case 0: if(on) LED0_ON(); else LED0_OFF(); break;
        case 1: if(on) LED1_ON(); else LED1_OFF(); break;
        case 2: if(on) LED2_ON(); else LED2_OFF(); break;
        case 3: if(on) LED3_ON(); else LED3_OFF(); break;
        default: break;
    }
}

void LED_All_Off(void)
{
    LED0_OFF(); LED1_OFF(); LED2_OFF(); LED3_OFF();
}

void RGB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = RGB_R_PIN | RGB_G_PIN | RGB_B_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RGB_PORT, &GPIO_InitStructure);
    RGB_Off();
}

void RGB_Control(uint8_t r, uint8_t g, uint8_t b)
{
    if(r) RGB_R_ON(); else RGB_R_OFF();
    if(g) RGB_G_ON(); else RGB_G_OFF();
    if(b) RGB_B_ON(); else RGB_B_OFF();
}

void RGB_Off(void)
{
    RGB_R_OFF(); RGB_G_OFF(); RGB_B_OFF();
}
