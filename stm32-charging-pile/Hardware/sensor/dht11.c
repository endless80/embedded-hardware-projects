/**
 * @file    dht11.c
 * @brief   DHT11 温湿度传感器驱动 (单总线)
 *          原理图: PC2
 */
#include "dht11.h"
#include "pin_mapping.h"

static void DHT11_Delay_us(uint32_t us)
{
    uint32_t i;
    for(i = 0; i < us * 8; i++) { __NOP(); }
}

static void DHT11_Set_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

static void DHT11_Set_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE);
    DHT11_Set_Output();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

static uint8_t DHT11_Read_Byte(void)
{
    uint8_t val = 0, i;
    for(i = 0; i < 8; i++) {
        while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_RESET);  /* 等待低电平结束 */
        DHT11_Delay_us(40);
        if(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET) {     /* 高电平 > 40us = 1 */
            val |= (0x80 >> i);
        }
        while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET);   /* 等待高电平结束 */
    }
    return val;
}

uint8_t DHT11_Read_Temperature_Humidity(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5] = {0};

    /* 主机发起通信 */
    DHT11_Set_Output();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); /* 拉低 ≥ 18ms */
    DHT11_Delay_us(20000);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   /* 拉高 20-40us */
    DHT11_Delay_us(30);

    DHT11_Set_Input();

    /* 等待 DHT11 响应低电平 (80us) */
    if(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET) return 1;
    while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_RESET);
    /* 等待响应高电平 (80us) */
    while(GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET);

    /* 读取 40 位数据: humi_int, humi_dec, temp_int, temp_dec, checksum */
    buf[0] = DHT11_Read_Byte();
    buf[1] = DHT11_Read_Byte();
    buf[2] = DHT11_Read_Byte();
    buf[3] = DHT11_Read_Byte();
    buf[4] = DHT11_Read_Byte();

    DHT11_Set_Output();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);

    /* 校验 */
    if((buf[0] + buf[1] + buf[2] + buf[3]) != buf[4]) return 2;

    *humi = buf[0];
    *temp = buf[2];
    return 0;
}
