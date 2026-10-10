#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f10x.h"

#define W25Q64_PAGE_SIZE    256
#define W25Q64_SECTOR_SIZE  4096
#define W25Q64_TOTAL_SIZE   (8 * 1024 * 1024)  /* 8MB */

void    W25Q64_Init(void);
uint16_t W25Q64_ReadID(void);
void    W25Q64_Read(uint32_t addr, uint8_t *buf, uint16_t len);
void    W25Q64_Write(uint32_t addr, const uint8_t *buf, uint16_t len);
void    W25Q64_Erase_Sector(uint32_t addr);
void    W25Q64_Erase_Chip(void);

#endif
