/**
 * @file    esp8266.c
 * @brief   ESP-12E (ESP8266) WiFi 模块驱动
 *          原理图: USART2 (PA2/PA3), ESP_EN = PA4
 *          AT 指令通信
 */
#include "esp8266.h"
#include "usart.h"
#include "pin_mapping.h"

static char resp_buf[512];

void ESP8266_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* ESP_EN 输出 */
    GPIO_InitStructure.GPIO_Pin   = ESP_EN_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP_EN_PORT, &GPIO_InitStructure);

    USART2_Init(115200);
    ESP8266_Reset();
}

void ESP8266_Reset(void)
{
    GPIO_ResetBits(ESP_EN_PORT, ESP_EN_PIN);   /* RST */
    DELAY_MS(10);
    GPIO_SetBits(ESP_EN_PORT, ESP_EN_PIN);     /* 上电 */
    DELAY_MS(1000);
}

uint8_t ESP8266_Send_AT(const char *cmd, uint16_t timeout_ms)
{
    USART2_Send((const uint8_t *)cmd, strlen(cmd));
    USART2_Send((const uint8_t *)"\r\n", 2);

    uint32_t start = GET_TICK();
    uint16_t pos = 0;
    memset(resp_buf, 0, sizeof(resp_buf));

    while((GET_TICK() - start) < timeout_ms) {
        uint16_t len = USART2_Recv((uint8_t *)&resp_buf[pos], 
                                     sizeof(resp_buf) - pos - 1);
        pos += len;
        if(strstr(resp_buf, "OK"))    return 0;   /* 成功 */        
        if(strstr(resp_buf, "ERROR")) return 1;   /* 失败 */
        if(strstr(resp_buf, "FAIL"))  return 2;
    }
    return 0xFF;   /* 超时 */
}

uint8_t ESP8266_Connect_AP(const char *ssid, const char *pwd)
{
    char cmd[128];

    /* 设置 Station 模式 */
    ESP8266_Send_AT("AT+CWMODE=1", 2000);
    DELAY_MS(500);

    /* 连接 WiFi */
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
    if(ESP8266_Send_AT(cmd, 10000) == 0) {
        return 0;  /* 连接成功 */
    }
    return 1;
}

uint8_t ESP8266_Send_Data(const char *host, uint16_t port, const char *data)
{
    char cmd[128];

    /* TCP 连接 */
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
    if(ESP8266_Send_AT(cmd, 5000) != 0) return 1;

    /* 发送数据长度 */
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d", (int)strlen(data));
    if(ESP8266_Send_AT(cmd, 3000) != 0) return 2;

    /* 发送数据内容 */
    USART2_Send((const uint8_t *)data, strlen(data));
    DELAY_MS(500);

    /* 关闭连接 */
    ESP8266_Send_AT("AT+CIPCLOSE", 2000);
    return 0;
}

uint8_t ESP8266_Recv_Data(char *buf, uint16_t max_len)
{
    uint16_t len = USART2_Recv((uint8_t *)buf, max_len - 1);
    if(len > 0) {
        buf[len] = '\0';
        return 0;
    }
    return 1;
}
