#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f10x.h"

/* NRF24L01 寄存器 */
#define NRF_CONFIG       0x00
#define NRF_EN_AA        0x01
#define NRF_EN_RXADDR    0x02
#define NRF_SETUP_AW     0x03
#define NRF_SETUP_RETR   0x04
#define NRF_RF_CH        0x05
#define NRF_RF_SETUP     0x06
#define NRF_STATUS       0x07
#define NRF_RX_PW_P0     0x11
#define NRF_TX_ADDR      0x10
#define NRF_RX_ADDR_P0   0x0A
#define NRF_FIFO_STATUS  0x17

void    NRF24L01_Init(void);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_Send_Packet(uint8_t *data, uint8_t len);
uint8_t NRF24L01_Recv_Packet(uint8_t *data, uint8_t *len);
void    NRF24L01_Set_RX_Mode(void);
void    NRF24L01_Set_TX_Mode(void);

#endif
