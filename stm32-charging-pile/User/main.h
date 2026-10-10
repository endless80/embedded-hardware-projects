/**
 * @file    main.h
 * @brief   主程序公共头文件
 */
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "pin_mapping.h"

/* 系统滴答 (由 SysTick_Handler 每1ms递增) */
extern volatile uint32_t g_sys_tick;
#define GET_TICK()  (g_sys_tick)
#define DELAY_MS(ms)  { uint32_t start = GET_TICK(); while((GET_TICK() - start) < (ms)); }

/* ========== 传感器初始化/读取函数 ========== */
void MQ2_Init(void);
uint16_t MQ2_Read(void);
void LightSensor_Init(void);
uint16_t LightSensor_Read(void);

void DHT11_Init(void);
uint8_t DHT11_Read_Temperature_Humidity(uint8_t *temp, uint8_t *humi);

void IR_Remote_Init(void);
uint8_t IR_Remote_Scan(uint8_t *code);

/* ========== 存储 ========== */
void W25Q64_Init(void);
uint8_t W25Q64_ReadID(void);
void W25Q64_Read(uint32_t addr, uint8_t *buf, uint16_t len);
void W25Q64_Write(uint32_t addr, const uint8_t *buf, uint16_t len);
void W25Q64_Erase_Sector(uint32_t sector_addr);

void AT24C02_Init(void);
uint8_t AT24C02_Write_Byte(uint16_t addr, uint8_t data);
uint8_t AT24C02_Read_Byte(uint16_t addr);
void AT24C02_Write_Bytes(uint16_t addr, uint8_t *buf, uint16_t len);
void AT24C02_Read_Bytes(uint16_t addr, uint8_t *buf, uint16_t len);

void SD_Init(void);
uint8_t SD_Read_Block(uint32_t sector, uint8_t *buf);
uint8_t SD_Write_Block(uint32_t sector, const uint8_t *buf);

/* ========== 通信 ========== */
void Debug_USART_Init(uint32_t baud);
void Debug_Printf(const char *fmt, ...);
void USART1_IRQ_Handler_user(void);

void ESP8266_Init(void);
void ESP8266_Reset(void);
uint8_t ESP8266_Connect_AP(const char *ssid, const char *pwd);
uint8_t ESP8266_Send_Data(const char *host, uint16_t port, const char *data);
uint8_t ESP8266_Recv_Data(char *buf, uint16_t max_len);
void USART2_IRQ_Handler_user(void);

void RS485_Init(uint32_t baud);
void RS485_Send(uint8_t *buf, uint16_t len);
uint8_t RS485_Recv(uint8_t *buf, uint16_t max_len, uint32_t timeout_ms);
void USART3_IRQ_Handler_user(void);

void NRF24L01_Init(void);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_Send_Packet(uint8_t *data, uint8_t len);
uint8_t NRF24L01_Recv_Packet(uint8_t *data, uint8_t *len);

/* ========== 显示 ========== */
void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_Draw_String(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);
void LCD_Draw_Num(uint16_t x, uint16_t y, uint32_t num, uint16_t color, uint16_t bg);
void LCD_Show_Chinese(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);

void Touch_Init(void);
uint8_t Touch_Get_Point(uint16_t *x, uint16_t *y);

/* ========== 控制 ========== */
void Relay_Init(void);
void Relay_Control(uint8_t on);

void Buzzer_Init(void);
void Buzzer_Beep(uint16_t duration_ms);

void LED_Init(void);
void LED_Control(uint8_t led_id, uint8_t on);
void RGB_Control(uint8_t r, uint8_t g, uint8_t b);

void Key_Init(void);
uint8_t Key_Scan(void);

void Stepper_Init(void);
void Stepper_Run(uint16_t steps, uint8_t dir);

void DC_Motor_Init(void);
void DC_Motor_Run(uint8_t speed, uint8_t dir);
void DC_Motor_Stop(void);

/* ========== 全局状态 ========== */
extern volatile uint8_t  g_wifi_connected;
extern volatile uint32_t g_charge_energy;

#endif /* __MAIN_H */
