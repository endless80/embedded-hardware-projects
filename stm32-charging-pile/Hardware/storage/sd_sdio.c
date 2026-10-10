/**
 * @file    sd_sdio.c
 * @brief   SD 卡驱动 (SDIO 接口)
 *          原理图: PC8-D0, PC9-D1, PC10-D2, PC11-D3, PC12-CK, PD2-CMD
 *          注: 完整 SD 卡协议较复杂, 这里给出框架
 */
#include "sd_sdio.h"
#include "pin_mapping.h"

static SDIO_InitTypeDef  SDIO_InitStructure;
static GPIO_InitTypeDef  GPIO_InitStructure;

void SD_Init(void)
{
    RCC_APB2PeriphClockCmd(SDIO_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);

    /* SDIO 引脚配置: 复用推挽 */
    GPIO_InitStructure.GPIO_Pin   = SDIO_D0_PIN | SDIO_D1_PIN | SDIO_D2_PIN
                                  | SDIO_D3_PIN | SDIO_CK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SDIO_GPIO_PORT_C, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = SDIO_CMD_PIN;
    GPIO_Init(SDIO_GPIO_PORT_D, &GPIO_InitStructure);

    /* SDIO 外设初始化 (1-bit模式, 后续可切4-bit) */
    SDIO_DeInit();
    SDIO_InitStructure.SDIO_ClockEdge           = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass          = DISABLE;
    SDIO_InitStructure.SDIO_ClockPowerSave       = DISABLE;
    SDIO_InitStructure.SDIO_BusWide              = SDIO_BusWide_1b;
    SDIO_InitStructure.SDIO_HardwareFlowControl  = DISABLE;
    SDIO_InitStructure.SDIO_ClockDiv             = 0x02;
    SDIO_Init(&SDIO_InitStructure);
    SDIO_SetPowerState(SDIO_PowerState_ON);

    /* SD 卡初始化命令序列 (CMD0, CMD8, ACMD41, CMD2, CMD3 等) */
    /* 完整协议栈建议移植 FatFS + 底层驱动 */
}

uint8_t SD_Read_Block(uint32_t sector, uint8_t *buf)
{
    /* 发送 CMD17 (READ_SINGLE_BLOCK) */
    /* 轮询或DMA读取 512 字节 */
    /* 返回 0=成功 */
    return 0;
}

uint8_t SD_Write_Block(uint32_t sector, const uint8_t *buf)
{
    /* 发送 CMD24 (WRITE_BLOCK) */
    /* 返回 0=成功 */
    return 0;
}

void SDIO_IT_Handler(void)
{
    /* SDIO 中断处理 */
    if(SDIO_GetITStatus(SDIO_IT_DATAEND)) {
        SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
    }
    if(SDIO_GetITStatus(SDIO_IT_RXOVERR)) {
        SDIO_ClearITPendingBit(SDIO_IT_RXOVERR);
    }
}
