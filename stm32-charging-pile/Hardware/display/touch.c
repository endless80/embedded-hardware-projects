/**
 * @file    touch.c
 * @brief   电阻触摸屏驱动 (XPT2046 / TSC2046)
 *          原理图: PF9(CS), PF10(PENIRQ), PF8(MOSI), PF7(MISO), PF6(SCK)
 *          注: 触摸控制器使用软件 SPI 通信
 */
#include "touch.h"
#include "pin_mapping.h"

static GPIO_InitTypeDef GPIO_InitStructure;

/* 触摸 SPI 软实现 */
static void T_SPI_CS(uint8_t level)
{
    if(level) GPIO_SetBits(T_PORT, T_CS_PIN);
    else      GPIO_ResetBits(T_PORT, T_CS_PIN);
}

static void T_SPI_SCK(uint8_t level)
{
    if(level) GPIO_SetBits(T_PORT, T_SCK_PIN);
    else      GPIO_ResetBits(T_PORT, T_SCK_PIN);
}

static void T_SPI_MOSI(uint8_t level)
{
    if(level) GPIO_SetBits(T_PORT, T_MOSI_PIN);
    else      GPIO_ResetBits(T_PORT, T_MOSI_PIN);
}

static uint8_t T_SPI_MISO(void)
{
    return GPIO_ReadInputDataBit(T_PORT, T_MISO_PIN);
}

static uint16_t Touch_Read_ADC(uint8_t cmd)
{
    uint8_t i;
    uint16_t val = 0;

    T_SPI_CS(0);
    for(i = 0; i < 8; i++) {
        T_SPI_SCK(0);
        if(cmd & 0x80) T_SPI_MOSI(1); else T_SPI_MOSI(0);
        cmd <<= 1;
        T_SPI_SCK(1);
        val <<= 1;
        if(T_SPI_MISO()) val |= 0x01;
    }
    /* 读取 12 位 ADC 结果 */
    for(i = 0; i < 12; i++) {
        T_SPI_SCK(0);
        T_SPI_SCK(1);
        val <<= 1;
        if(T_SPI_MISO()) val |= 0x01;
    }
    T_SPI_CS(1);
    return (val >> 3) & 0xFFF;   /* 12位有效 */
}

void Touch_Init(void)
{
    RCC_APB2PeriphClockCmd(T_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = T_CS_PIN | T_SCK_PIN | T_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(T_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = T_MISO_PIN | T_PEN_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(T_PORT, &GPIO_InitStructure);

    T_SPI_CS(1);
    T_SPI_SCK(0);
}

uint8_t Touch_Get_Point(uint16_t *x, uint16_t *y)
{
    uint16_t raw_x, raw_y;

    /* 等待触摸 */
    if(T_PEN_READ() != Bit_RESET) return 1;  /* 未按下 */

    raw_x = Touch_Read_ADC(0xD0);   /* X 坐标 */
    raw_y = Touch_Read_ADC(0x90);   /* Y 坐标 */

    /* 简单的坐标转换 (需根据实际屏幕校准) */
    *x = (uint16_t)((uint32_t)raw_x * 320 / 4096);
    *y = (uint16_t)((uint32_t)raw_y * 480 / 4096);

    return 0;
}
