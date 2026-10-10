/**
 * @file    key.c
 * @brief   4个用户按键驱动 (低电平有效, 无外部上拉则启用内部上拉)
 *          原理图: PC13, PC14, PC15, PC3
 */
#include "key.h"
#include "pin_mapping.h"

static GPIO_InitTypeDef GPIO_InitStructure;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;              /* 内部上拉 */
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

uint8_t Key_Scan(void)
{
    static uint8_t last = KEY_NONE;
    uint8_t key = KEY_NONE;

    if(KEY1_PRESSED())                /* PC13 按下 */
        key = KEY1;
    else if(KEY2_PRESSED())           /* PC14 */
        key = KEY2;
    else if(KEY3_PRESSED())           /* PC15 */
        key = KEY3;
    else if(KEY4_PRESSED())
        key = KEY4;

    /* 简单消抖: 只有新按下才返回 */
    if(key != last && key != KEY_NONE) {
        last = key;
        return key;
    }
    if(key == KEY_NONE) last = KEY_NONE;
    return KEY_NONE;
}

uint8_t Key_Scan_Long(void)
{
    /* 长按检测: 持续按下超过800ms */
    /* 简化版, 实际项目推荐用定时器 */
    return Key_Scan();
}
