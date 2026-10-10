/**
 * @file    at24c02.c
 * @brief   AT24C02 EEPROM 驱动 (I2C1)
 *          原理图: PB6(SCL) PB7(SDA)
 */
#include "at24c02.h"
#include "pin_mapping.h"

static I2C_InitTypeDef  I2C_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

void AT24C02_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* PB6(SCL) PB7(SDA) 开漏输出 */
    GPIO_InitStructure.GPIO_Pin   = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);

    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode               = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1         = 0x00;
    I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed          = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

uint8_t AT24C02_Write_Byte(uint16_t addr, uint8_t data)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, AT24C02_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, (uint8_t)(addr & 0xFF));
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
    return 0;
}

uint8_t AT24C02_Read_Byte(uint16_t addr)
{
    uint8_t data = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    /* 伪写操作设置地址 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, AT24C02_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, (uint8_t)(addr & 0xFF));
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* 重复起始 + 读 */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, AT24C02_ADDR, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C1);

    I2C_GenerateSTOP(I2C1, ENABLE);
    return data;
}

void AT24C02_Write_Bytes(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++) {
        AT24C02_Write_Byte(addr + i, buf[i]);
        DELAY_MS(10);
    }
}

void AT24C02_Read_Bytes(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++) {
        buf[i] = AT24C02_Read_Byte(addr + i);
    }
}
