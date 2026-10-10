/**
 * @file    nrf24l01.c
 * @brief   NRF24L01 2.4G 无线模块驱动
 *          原理图: SPI2 (PB13/14/15), CE=PG6, CSN=PG7, IRQ=PG8
 */
#include "nrf24l01.h"
#include "pin_mapping.h"

static SPI_InitTypeDef  SPI_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

static uint8_t NRF_SPI_RW(uint8_t byte)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, byte);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_I2S_ReceiveData(SPI2);
}

static uint8_t NRF_Read_Reg(uint8_t reg)
{
    uint8_t val;
    NRF24L01_CSN_L();
    NRF_SPI_RW(reg & 0x1F);
    val = NRF_SPI_RW(0xFF);
    NRF24L01_CSN_H();
    return val;
}

static void NRF_Write_Reg(uint8_t reg, uint8_t val)
{
    NRF24L01_CSN_L();
    NRF_SPI_RW((reg & 0x1F) | 0x20);
    NRF_SPI_RW(val);
    NRF24L01_CSN_H();
}

static void NRF_Write_Buf(uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    NRF24L01_CSN_L();
    NRF_SPI_RW(reg);
    for(i = 0; i < len; i++) NRF_SPI_RW(buf[i]);
    NRF24L01_CSN_H();
}

static void NRF_Read_Buf(uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    NRF24L01_CSN_L();
    NRF_SPI_RW(reg);
    for(i = 0; i < len; i++) buf[i] = NRF_SPI_RW(0xFF);
    NRF24L01_CSN_H();
}

void NRF24L01_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);

    /* SPI2 引脚 PB13(SCK) PB14(MISO) PB15(MOSI) */
    GPIO_InitStructure.GPIO_Pin   = SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_GPIO_PORT, &GPIO_InitStructure);

    /* CE, CSN, IRQ */
    GPIO_InitStructure.GPIO_Pin   = NRF24L01_CE_PIN | NRF24L01_CSN_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NRF24L01_PORT, &GPIO_InitStructure);
    NRF24L01_CE_L();
    NRF24L01_CSN_H();

    GPIO_InitStructure.GPIO_Pin   = NRF24L01_IRQ_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NRF24L01_PORT, &GPIO_InitStructure);

    /* SPI2 主机模式 */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);

    /* NRF24L01 初始化 */
    DELAY_MS(10);
    NRF_Write_Reg(NRF_CONFIG, 0x0E);           /* 上电, CRC 2字节 */
    NRF_Write_Reg(NRF_EN_AA, 0x01);            /* 自动ACK */
    NRF_Write_Reg(NRF_EN_RXADDR, 0x01);        /* 只开通道0 */
    NRF_Write_Reg(NRF_SETUP_AW, 0x03);         /* 地址宽度 5字节 */
    NRF_Write_Reg(NRF_SETUP_RETR, 0x1A);       /* 重发 500us, 10次 */
    NRF_Write_Reg(NRF_RF_CH, 40);              /* 频道 2.440GHz */
    NRF_Write_Reg(NRF_RF_SETUP, 0x06);         /* 1Mbps, 0dBm */
}

uint8_t NRF24L01_Check(void)
{
    uint8_t val = NRF_Read_Reg(NRF_CONFIG);
    return (val == 0x0E) ? 1 : 0;
}

void NRF24L01_Set_TX_Mode(void)
{
    NRF24L01_CE_L();
    NRF_Write_Reg(NRF_CONFIG, 0x0E);           /* PRIM=0 (TX) */
    DELAY_MS(1);
    NRF24L01_CE_H();
}

void NRF24L01_Set_RX_Mode(void)
{
    NRF24L01_CE_L();
    NRF_Write_Reg(NRF_CONFIG, 0x0F);           /* PRIM=1 (RX), 上电 */
    NRF_Write_Reg(NRF_STATUS, 0x70);           /* 清除状态 */
    NRF24L01_CE_H();
}

uint8_t NRF24L01_Send_Packet(uint8_t *data, uint8_t len)
{
    NRF24L01_Set_TX_Mode();
    NRF_Write_Buf(0xA0, data, len);            /* WR_TX_PAYLOAD */

    uint32_t timeout = GET_TICK();
    while(NRF_Read_Reg(NRF_STATUS) & 0x10 == 0) {  /* 等待 TX_DS */
        if((GET_TICK() - timeout) > 100) return 1;
    }
    NRF_Write_Reg(NRF_STATUS, 0x30);           /* 清除 TX_DS, MAX_RT */
    return 0;
}

uint8_t NRF24L01_Recv_Packet(uint8_t *data, uint8_t *len)
{
    if(NRF_Read_Reg(NRF_STATUS) & 0x40) {      /* RX_DR */
        NRF_Write_Reg(NRF_STATUS, 0x40);       /* 清除 RX_DR */
        *len = NRF_Read_Reg(0x60);              /* RD_RX_PL_WID */
        NRF_Read_Buf(0x61, data, *len);         /* RD_RX_PAYLOAD */
        return 0;
    }
    return 1;
}
