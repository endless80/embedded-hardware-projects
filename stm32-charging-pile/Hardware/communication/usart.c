/**
 * @file    usart.c
 * @brief   USART 串口驱动 (USART1/2/3)
 */
#include "usart.h"
#include "pin_mapping.h"

/* USART1 接收环形缓冲区 */
#define UART1_RX_BUF_SIZE   256
static uint8_t  uart1_rx_buf[UART1_RX_BUF_SIZE];
static uint16_t uart1_rx_head = 0, uart1_rx_tail = 0;

/* USART2 接收缓冲区 (ESP8266) */
#define UART2_RX_BUF_SIZE   512
static uint8_t  uart2_rx_buf[UART2_RX_BUF_SIZE];
static uint16_t uart2_rx_head = 0, uart2_rx_tail = 0;

/* USART3 接收缓冲区 (RS485) */
#define UART3_RX_BUF_SIZE   256
static uint8_t  uart3_rx_buf[UART3_RX_BUF_SIZE];
static uint16_t uart3_rx_head = 0, uart3_rx_tail = 0;

static void USART_GPIO_Init(USART_TypeDef *USARTx, GPIO_TypeDef *port,
                            uint16_t tx_pin, uint16_t rx_pin, uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    uint32_t gpio_clk = (port == GPIOA) ? RCC_APB2Periph_GPIOA
                       : (port == GPIOB) ? RCC_APB2Periph_GPIOB
                       : RCC_APB2Periph_GPIOC;

    RCC_APB2PeriphClockCmd(gpio_clk | RCC_APB2Periph_AFIO, ENABLE);

    /* TX: 复用推挽 */
    GPIO_InitStructure.GPIO_Pin   = tx_pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(port, &GPIO_InitStructure);

    /* RX: 浮空输入 */
    GPIO_InitStructure.GPIO_Pin   = rx_pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(port, &GPIO_InitStructure);

    /* USART 配置 */
    if(USARTx == USART1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(USARTx == USART2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    USART_InitStructure.USART_BaudRate            = baud;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits             = USART_StopBits_1;
    USART_InitStructure.USART_Parity               = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                 = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);
}

/* ======================== USART1 ======================== */
void Debug_USART_Init(uint32_t baud)
{
    USART_GPIO_Init(USART1, USART1_GPIO_PORT,
                    USART1_TX_PIN, USART1_RX_PIN, baud);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

void Debug_Printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    char *p = buf;
    while(*p) {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *p++);
    }
}

void USART1_IRQ_Handler_user(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE)) {
        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
        uart1_rx_buf[uart1_rx_head] = data;
        uart1_rx_head = (uart1_rx_head + 1) % UART1_RX_BUF_SIZE;
    }
}

/* ======================== USART2 (ESP8266) ======================== */
void USART2_Init(uint32_t baud)
{
    USART_GPIO_Init(USART2, USART2_GPIO_PORT,
                    USART2_TX_PIN, USART2_RX_PIN, baud);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_Send(const uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, buf[i]);
    }
}

uint16_t USART2_Recv(uint8_t *buf, uint16_t max_len)
{
    uint16_t cnt = 0;
    while(uart2_rx_head != uart2_rx_tail && cnt < max_len) {
        buf[cnt++] = uart2_rx_buf[uart2_rx_tail];
        uart2_rx_tail = (uart2_rx_tail + 1) % UART2_RX_BUF_SIZE;
    }
    return cnt;
}

void USART2_IRQ_Handler_user(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE)) {
        uint8_t data = (uint8_t)USART_ReceiveData(USART2);
        uart2_rx_buf[uart2_rx_head] = data;
        uart2_rx_head = (uart2_rx_head + 1) % UART2_RX_BUF_SIZE;
    }
}

/* ======================== USART3 (RS485) ======================== */
void USART3_Init(uint32_t baud)
{
    USART_GPIO_Init(USART3, USART3_GPIO_PORT,
                    USART3_TX_PIN, USART3_RX_PIN, baud);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART3_IRQn);
}

void USART3_Send(const uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++) {
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
        USART_SendData(USART3, buf[i]);
    }
}

uint16_t USART3_Recv(uint8_t *buf, uint16_t max_len)
{
    uint16_t cnt = 0;
    while(uart3_rx_head != uart3_rx_tail && cnt < max_len) {
        buf[cnt++] = uart3_rx_buf[uart3_rx_tail];
        uart3_rx_tail = (uart3_rx_tail + 1) % UART3_RX_BUF_SIZE;
    }
    return cnt;
}

void USART3_IRQ_Handler_user(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE)) {
        uint8_t data = (uint8_t)USART_ReceiveData(USART3);
        uart3_rx_buf[uart3_rx_head] = data;
        uart3_rx_head = (uart3_rx_head + 1) % UART3_RX_BUF_SIZE;
    }
}
