/**
 * @file    pin_mapping.h
 * @brief   STM32F103ZET6 开发板引脚映射
 * @note    基于信盈达汽车充电桩原理图 (STM3F103开发板原理图.pdf)
 *          当引脚不确定时, 查看原理图对应网络标号确认
 */
#ifndef __PIN_MAPPING_H
#define __PIN_MAPPING_H

#include "stm32f10x.h"

/* ======================== 电源与系统 ======================== */
#define BOOT0_PIN                                             /* BOOT0 (连接CH340G一键下载) */

/* ======================== USART1 - 串口调试/CH340G ======================== */
#define USART1_TX_PIN              GPIO_Pin_9                 /* PA9 */
#define USART1_RX_PIN              GPIO_Pin_10                /* PA10 */
#define USART1_GPIO_PORT           GPIOA
#define USART1_GPIO_CLK            RCC_APB2Periph_GPIOA
#define USART1_APB                 RCC_APB2Periph_USART1

/* ======================== USART2 - ESP-12E WiFi 模块 ======================== */
#define USART2_TX_PIN              GPIO_Pin_2                 /* PA2 */
#define USART2_RX_PIN              GPIO_Pin_3                 /* PA3 */
#define USART2_GPIO_PORT           GPIOA
#define USART2_GPIO_CLK            RCC_APB2Periph_GPIOA
#define USART2_APB                 RCC_APB1Periph_USART2

#define ESP_EN_PIN                 GPIO_Pin_4                 /* PA4 - ESP8266 使能 */
#define ESP_EN_PORT                GPIOA

/* ======================== USART3 - RS485 / 通用 ======================== */
#define USART3_TX_PIN              GPIO_Pin_10                /* PB10 */
#define USART3_RX_PIN              GPIO_Pin_11                /* PB11 */
#define USART3_GPIO_PORT           GPIOB
#define USART3_GPIO_CLK            RCC_APB2Periph_GPIOB
#define USART3_APB                 RCC_APB1Periph_USART3

/* ======================== SPI1 - W25Q64 Flash ======================== */
#define SPI1_SCK_PIN               GPIO_Pin_5                 /* PA5 */
#define SPI1_MISO_PIN              GPIO_Pin_6                 /* PA6 */
#define SPI1_MOSI_PIN              GPIO_Pin_7                 /* PA7 */
#define SPI1_GPIO_PORT             GPIOA
#define SPI1_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI1_APB                   RCC_APB2Periph_SPI1

#define FLASH_CS_PIN               GPIO_Pin_3                 /* PB3 - W25Q64 片选 */
#define FLASH_CS_PORT              GPIOB
#define FLASH_CS_CLK               RCC_APB2Periph_GPIOB

/* ======================== SPI2 - NRF24L01 2.4G ======================== */
#define SPI2_SCK_PIN               GPIO_Pin_13                /* PB13 */
#define SPI2_MISO_PIN              GPIO_Pin_14                /* PB14 */
#define SPI2_MOSI_PIN              GPIO_Pin_15                /* PB15 */
#define SPI2_GPIO_PORT             GPIOB
#define SPI2_GPIO_CLK              RCC_APB2Periph_GPIOB
#define SPI2_APB                   RCC_APB1Periph_SPI2

#define NRF24L01_CE_PIN            GPIO_Pin_6                 /* PG6 */
#define NRF24L01_CSN_PIN           GPIO_Pin_7                 /* PG7 */
#define NRF24L01_IRQ_PIN           GPIO_Pin_8                 /* PG8 */
#define NRF24L01_PORT              GPIOG
#define NRF24L01_CLK               RCC_APB2Periph_GPIOG

/* ======================== I2C1 - AT24C02 EEPROM ======================== */
#define I2C1_SCL_PIN               GPIO_Pin_6                 /* PB6 */
#define I2C1_SDA_PIN               GPIO_Pin_7                 /* PB7 */
#define I2C1_GPIO_PORT             GPIOB
#define I2C1_GPIO_CLK              RCC_APB2Periph_GPIOB
#define I2C1_APB                   RCC_APB1Periph_I2C1

/* ======================== SDIO - TF 卡 ======================== */
#define SDIO_D0_PIN                GPIO_Pin_8                 /* PC8 */
#define SDIO_D1_PIN                GPIO_Pin_9                 /* PC9 */
#define SDIO_D2_PIN                GPIO_Pin_10                /* PC10 */
#define SDIO_D3_PIN                GPIO_Pin_11                /* PC11 */
#define SDIO_CK_PIN                GPIO_Pin_12                /* PC12 */
#define SDIO_CMD_PIN               GPIO_Pin_2                 /* PD2 */
#define SDIO_GPIO_PORT_D           GPIOD
#define SDIO_GPIO_PORT_C           GPIOC
#define SDIO_GPIO_CLK              RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD

/* ======================== FSMC - TFT LCD ======================== */
#define LCD_CS_PIN                 GPIO_Pin_12                /* PG12 - FSMC_NE4 */
#define LCD_RS_PIN                 GPIO_Pin_0                 /* PG0  - FSMC_A10 */
#define LCD_CS_PORT                GPIOG
#define LCD_RS_PORT                GPIOG
#define LCD_BL_PIN                 GPIO_Pin_6                 /* PF6  - LCD背光 */
#define LCD_BL_PORT                GPIOF

#define FSMC_DATA_PORT             GPIOD                      /* D0-D15 通过 PD0-15, PE7-15 */
#define FSMC_DATA_PORT2            GPIOE
#define FSMC_CTRL_PORT             GPIOG

/* ======================== 触摸屏 (XPT2046) ======================== */
#define T_CS_PIN                   GPIO_Pin_9                 /* PF9 */
#define T_PEN_PIN                  GPIO_Pin_10                /* PF10 */
#define T_MOSI_PIN                 GPIO_Pin_8                 /* PF8 */
#define T_MISO_PIN                 GPIO_Pin_7                 /* PF7 */
#define T_SCK_PIN                  GPIO_Pin_6                 /* PF6 (注意: 与背光共用) */
#define T_PORT                     GPIOF
#define T_CLK                      RCC_APB2Periph_GPIOF

/* ======================== 传感器 ======================== */
#define MQ2_ADC_CHANNEL            ADC_Channel_10             /* PC0 */
#define MQ2_GPIO_PORT              GPIOC
#define MQ2_GPIO_PIN               GPIO_Pin_0

#define LIGHT_ADC_CHANNEL          ADC_Channel_11             /* PC1 */
#define LIGHT_GPIO_PORT            GPIOC
#define LIGHT_GPIO_PIN             GPIO_Pin_1

#define DHT11_PIN                  GPIO_Pin_2                 /* PC2 */
#define DHT11_PORT                 GPIOC
#define DHT11_CLK                  RCC_APB2Periph_GPIOC

#define IR_REMOTE_PIN              GPIO_Pin_15                /* PG15 */
#define IR_REMOTE_PORT             GPIOG
#define IR_REMOTE_CLK              RCC_APB2Periph_GPIOG

/* ======================== 控制 ======================== */
#define RELAY_PIN                  GPIO_Pin_5                 /* PC5 */
#define RELAY_PORT                 GPIOC
#define RELAY_CLK                  RCC_APB2Periph_GPIOC

#define BEEP_PIN                   GPIO_Pin_3                 /* PF3 */
#define BEEP_PORT                  GPIOF
#define BEEP_CLK                   RCC_APB2Periph_GPIOF

#define LED0_PIN                   GPIO_Pin_4                 /* PF4 */
#define LED1_PIN                   GPIO_Pin_5                 /* PF5 */
#define LED2_PIN                   GPIO_Pin_11                /* PF11 */
#define LED3_PIN                   GPIO_Pin_12                /* PF12 */
#define LED_PORT                   GPIOF
#define LED_CLK                    RCC_APB2Periph_GPIOF

#define RGB_R_PIN                  GPIO_Pin_0                 /* PF0 */
#define RGB_G_PIN                  GPIO_Pin_1                 /* PF1 */
#define RGB_B_PIN                  GPIO_Pin_2                 /* PF2 */
#define RGB_PORT                   GPIOF

#define KEY1_PIN                   GPIO_Pin_13                /* PC13 */
#define KEY2_PIN                   GPIO_Pin_14                /* PC14 */
#define KEY3_PIN                   GPIO_Pin_15                /* PC15 */
#define KEY4_PIN                   GPIO_Pin_3                 /* PC3 */
#define KEY_PORT                   GPIOC
#define KEY_CLK                    RCC_APB2Periph_GPIOC

#define MOTOR_IN_A_PIN             GPIO_Pin_8                 /* PB8 */
#define MOTOR_IN_B_PIN             GPIO_Pin_9                 /* PB9 */
#define MOTOR_PORT                 GPIOB
#define MOTOR_CLK                  RCC_APB2Periph_GPIOB

#define STEPPER_IN1_PIN            GPIO_Pin_0                 /* PE0 */
#define STEPPER_IN2_PIN            GPIO_Pin_1                 /* PE1 */
#define STEPPER_IN3_PIN            GPIO_Pin_4                 /* PB4 */
#define STEPPER_IN4_PIN            GPIO_Pin_5                 /* PB5 */
#define STEPPER_PORT_A             GPIOE
#define STEPPER_PORT_B             GPIOB
#define STEPPER_CLK                RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB

/* ======================== 宏操作 ======================== */
#define NRF24L01_CE_L()            GPIO_ResetBits(NRF24L01_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CE_H()            GPIO_SetBits(NRF24L01_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CSN_L()           GPIO_ResetBits(NRF24L01_PORT, NRF24L01_CSN_PIN)
#define NRF24L01_CSN_H()           GPIO_SetBits(NRF24L01_PORT, NRF24L01_CSN_PIN)

#define RELAY_ON()                 GPIO_SetBits(RELAY_PORT, RELAY_PIN)
#define RELAY_OFF()                GPIO_ResetBits(RELAY_PORT, RELAY_PIN)

#define BEEP_ON()                  GPIO_SetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF()                 GPIO_ResetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_TOGGLE()              GPIO_WriteBit(BEEP_PORT, BEEP_PIN, \
                                      (BitAction)(1 - GPIO_ReadOutputDataBit(BEEP_PORT, BEEP_PIN)))

#define LED0_ON()                  GPIO_ResetBits(LED_PORT, LED0_PIN)
#define LED0_OFF()                 GPIO_SetBits(LED_PORT, LED0_PIN)
#define LED1_ON()                  GPIO_ResetBits(LED_PORT, LED1_PIN)
#define LED1_OFF()                 GPIO_SetBits(LED_PORT, LED1_PIN)
#define LED2_ON()                  GPIO_ResetBits(LED_PORT, LED2_PIN)
#define LED2_OFF()                 GPIO_SetBits(LED_PORT, LED2_PIN)
#define LED3_ON()                  GPIO_ResetBits(LED_PORT, LED3_PIN)
#define LED3_OFF()                 GPIO_SetBits(LED_PORT, LED3_PIN)

#define RGB_R_ON()                 GPIO_SetBits(RGB_PORT, RGB_R_PIN)
#define RGB_G_ON()                 GPIO_SetBits(RGB_PORT, RGB_G_PIN)
#define RGB_B_ON()                 GPIO_SetBits(RGB_PORT, RGB_B_PIN)
#define RGB_R_OFF()                GPIO_ResetBits(RGB_PORT, RGB_R_PIN)
#define RGB_G_OFF()                GPIO_ResetBits(RGB_PORT, RGB_G_PIN)
#define RGB_B_OFF()                GPIO_ResetBits(RGB_PORT, RGB_B_PIN)

#define KEY1_PRESSED()             (GPIO_ReadInputDataBit(KEY_PORT, KEY1_PIN) == Bit_RESET)
#define KEY2_PRESSED()             (GPIO_ReadInputDataBit(KEY_PORT, KEY2_PIN) == Bit_RESET)
#define KEY3_PRESSED()             (GPIO_ReadInputDataBit(KEY_PORT, KEY3_PIN) == Bit_RESET)
#define KEY4_PRESSED()             (GPIO_ReadInputDataBit(KEY_PORT, KEY4_PIN) == Bit_RESET)

#define FLASH_CS_L()               GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN)
#define FLASH_CS_H()               GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN)

#define T_CS_L()                   GPIO_ResetBits(T_PORT, T_CS_PIN)
#define T_CS_H()                   GPIO_SetBits(T_PORT, T_CS_PIN)
#define T_PEN_READ()               GPIO_ReadInputDataBit(T_PORT, T_PEN_PIN)

#endif /* __PIN_MAPPING_H */
