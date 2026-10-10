/**
 * @file    w25q64.c
 * @brief   W25Q64 SPI Flash 驱动, 通过 SPI1 通信
 *          原理图: SPI1 (PA5/PA6/PA7), CS = PB3
 */
#include "w25q64.h"
#include "pin_mapping.h"

/* SPI 指令 */
#define W25X_WriteEnable      0x06
#define W25X_WriteDisable     0x04
#define W25X_ReadStatusReg    0x05
#define W25X_WriteStatusReg   0x01
#define W25X_ReadData         0x03
#define W25X_FastReadData     0x0B
#define W25X_PageProgram      0x02
#define W25X_SectorErase      0x20
#define W25X_ChipErase        0xC7
#define W25X_ReadID           0x9F

static SPI_InitTypeDef  SPI_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

static void SPI1_Write_Byte(uint8_t byte)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, byte);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI1);
}

static uint8_t SPI1_Read_Byte(void)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0xFF);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

static void W25Q64_Write_Enable(void)
{
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_WriteEnable);
    FLASH_CS_H();
}

static uint8_t W25Q64_Read_Status_Reg(void)
{
    uint8_t status;
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_ReadStatusReg);
    status = SPI1_Read_Byte();
    FLASH_CS_H();
    return status;
}

static void W25Q64_Wait_Busy(void)
{
    while(W25Q64_Read_Status_Reg() & 0x01);
}

void W25Q64_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | FLASH_CS_CLK | RCC_APB2Periph_SPI1, ENABLE);

    /* PA5(SCK) PA6(MISO) PA7(MOSI) 复用推挽 */
    GPIO_InitStructure.GPIO_Pin   = SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);

    /* PB3 CS */
    GPIO_InitStructure.GPIO_Pin   = FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FLASH_CS_PORT, &GPIO_InitStructure);
    FLASH_CS_H();

    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}

uint16_t W25Q64_ReadID(void)
{
    uint16_t id = 0;
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_ReadID);
    id  = (uint16_t)SPI1_Read_Byte() << 8;
    id |= (uint16_t)SPI1_Read_Byte();
    FLASH_CS_H();
    return id;
}

void W25Q64_Read(uint32_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_ReadData);
    SPI1_Write_Byte((uint8_t)(addr >> 16));
    SPI1_Write_Byte((uint8_t)(addr >> 8));
    SPI1_Write_Byte((uint8_t)(addr));
    for(i = 0; i < len; i++) {
        buf[i] = SPI1_Read_Byte();
    }
    FLASH_CS_H();
}

void W25Q64_Write(uint32_t addr, const uint8_t *buf, uint16_t len)
{
    uint16_t i;
    W25Q64_Write_Enable();
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_PageProgram);
    SPI1_Write_Byte((uint8_t)(addr >> 16));
    SPI1_Write_Byte((uint8_t)(addr >> 8));
    SPI1_Write_Byte((uint8_t)(addr));
    for(i = 0; i < len; i++) {
        SPI1_Write_Byte(buf[i]);
    }
    FLASH_CS_H();
    W25Q64_Wait_Busy();
}

void W25Q64_Erase_Sector(uint32_t addr)
{
    W25Q64_Write_Enable();
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_SectorErase);
    SPI1_Write_Byte((uint8_t)(addr >> 16));
    SPI1_Write_Byte((uint8_t)(addr >> 8));
    SPI1_Write_Byte((uint8_t)(addr));
    FLASH_CS_H();
    W25Q64_Wait_Busy();
}

void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_Enable();
    FLASH_CS_L();
    SPI1_Write_Byte(W25X_ChipErase);
    FLASH_CS_H();
    W25Q64_Wait_Busy();
}
