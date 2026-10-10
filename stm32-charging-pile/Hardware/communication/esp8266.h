#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"

void    ESP8266_Init(void);
void    ESP8266_Reset(void);
uint8_t ESP8266_Connect_AP(const char *ssid, const char *pwd);
uint8_t ESP8266_Send_Data(const char *host, uint16_t port, const char *data);
uint8_t ESP8266_Recv_Data(char *buf, uint16_t max_len);
uint8_t ESP8266_Send_AT(const char *cmd, uint16_t timeout_ms);

#endif
