#ifndef __SD_SDIO_H
#define __SD_SDIO_H

#include "stm32f10x.h"

#define SD_BLOCK_SIZE   512

void    SD_Init(void);
uint8_t SD_Read_Block(uint32_t sector, uint8_t *buf);
uint8_t SD_Write_Block(uint32_t sector, const uint8_t *buf);
void    SDIO_IT_Handler(void);

#endif
