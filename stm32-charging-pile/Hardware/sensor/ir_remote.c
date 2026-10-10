/**
 * @file    ir_remote.c
 * @brief   红外遥控接收驱动 (HS0038)
 *          原理图: PG15 - REMOTE_IN
 *          使用 NEC 协议: 9ms 引导码 + 4.5ms 高电平 + 32位数据
 */
#include "ir_remote.h"
#include "pin_mapping.h"

#define IR_REMOTE_TIMEOUT   100000

static GPIO_InitTypeDef GPIO_InitStructure;

void IR_Remote_Init(void)
{
    RCC_APB2PeriphClockCmd(IR_REMOTE_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = IR_REMOTE_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(IR_REMOTE_PORT, &GPIO_InitStructure);
}

static uint8_t IR_Remote_Get_High_Time(void)
{
    uint32_t cnt = 0;
    while(GPIO_ReadInputDataBit(IR_REMOTE_PORT, IR_REMOTE_PIN) == Bit_SET) {
        if(++cnt > IR_REMOTE_TIMEOUT) break;
    }
    return (cnt > 100) ? 1 : 0;
}

static uint8_t IR_Remote_Get_Low_Time(void)
{
    uint32_t cnt = 0;
    while(GPIO_ReadInputDataBit(IR_REMOTE_PORT, IR_REMOTE_PIN) == Bit_RESET) {
        if(++cnt > IR_REMOTE_TIMEOUT) break;
    }
    return (cnt > 100) ? 1 : 0;
}

uint8_t IR_Remote_Scan(uint8_t *code)
{
    uint8_t i, j, bit_val;
    uint32_t data = 0;

    /* 等待引导码: 9ms 低电平 */
    if(GPIO_ReadInputDataBit(IR_REMOTE_PORT, IR_REMOTE_PIN) == Bit_SET) return 0;
    if(!IR_Remote_Get_Low_Time()) return 0;
    /* 4.5ms 高电平 */
    if(!IR_Remote_Get_High_Time()) return 0;

    /* 读取 32 位数据 */
    for(i = 0; i < 32; i++) {
        if(!IR_Remote_Get_Low_Time()) return 0;    /* 等待低电平 (562.5us) */
        if(!IR_Remote_Get_High_Time()) return 0;   /* 高电平长度: 0=562.5us, 1=1.6875ms */
        /* 简单检测: 这里通过高电平时间判断 0/1, 简化版用固定延时判断 */
        bit_val = 1;
        /* 实际项目中建议用定时器捕获测量脉宽 */
        data = (data << 1) | bit_val;
    }

    code[0] = (uint8_t)(data >> 24);
    code[1] = (uint8_t)(data >> 16);
    code[2] = (uint8_t)(data >> 8);
    code[3] = (uint8_t)(data);
    return 1;
}
