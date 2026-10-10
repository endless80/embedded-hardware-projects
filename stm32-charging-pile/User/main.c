/**
 * @file    main.c
 * @brief   STM32F103 充电桩远程控制系统 - 主程序
 *
 * 功能概要:
 *   - 系统初始化: 72MHz, 1ms SysTick
 *   - 传感器采集: MQ-2(烟雾), DHT11(温湿度), 光敏, 红外
 *   - 数据存储: W25Q64, AT24C02, SD卡
 *   - 通信: WiFi(ESP8266云上报), RS485(充电模块), NRF24L01(2.4G)
 *   - 人机交互: TFT-LCD(FSMC), 触摸屏, 按键
 *   - 控制: 继电器(通断), 步进电机(充电枪锁), 直流电机(风扇)
 *   - 状态指示: LED, RGB-LED, 蜂鸣器
 *
 * 状态机: IDLE → AUTH → CHARGING → DONE
 */
#include "main.h"
#include "delay.h"

#include "mq2.h"
#include "dht11.h"
#include "light_sensor.h"
#include "ir_remote.h"

#include "w25q64.h"
#include "at24c02.h"
#include "sd_sdio.h"

#include "usart.h"
#include "rs485.h"
#include "nrf24l01.h"
#include "esp8266.h"

#include "lcd.h"
#include "touch.h"

#include "relay.h"
#include "stepper.h"
#include "dc_motor.h"
#include "led.h"
#include "buzzer.h"
#include "key.h"

/* ======================== 全局变量 ======================== */
volatile uint32_t g_sys_tick = 0;          /* 系统滴答 1ms */
volatile uint8_t  g_wifi_connected = 0;     /* WiFi 连接状态 */
volatile uint32_t g_charge_energy = 0;      /* 累积充电电量 (Wh) */

/* 系统状态枚举 */
typedef enum {
    STATE_IDLE = 0,      /* 空闲/待机 */
    STATE_AUTH,          /* 用户认证 */
    STATE_CHARGING,      /* 充电中 */
    STATE_DONE,          /* 充电完成 */
    STATE_ALARM          /* 报警状态 */
} SysState_t;

static SysState_t g_state = STATE_IDLE;

/* 传感器数据缓存 */
typedef struct {
    uint16_t mq2_val;       /* MQ-2 ADC 原始值 */
    float    mq2_ppm;       /* 烟雾浓度估算 (需标定) */
    uint8_t  temperature;    /* 温度 ℃ */
    uint8_t  humidity;       /* 湿度 %RH */
    uint16_t light_val;      /* 光照 ADC 原始值 */
} SensorData_t;

static SensorData_t g_sensor;

/* 充电参数 */
typedef struct {
    uint16_t voltage_mv;     /* 充电电压 mV */
    uint16_t current_ma;     /* 充电电流 mA */
    uint32_t energy_wh;      /* 累计电量 Wh */
    uint32_t start_tick;     /* 开始充电时刻 */
} ChargeData_t;

static ChargeData_t g_charge;

/* ======================== 系统时钟配置 ======================== */
static void System_Clock_Config(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS) {
        /* 启用预取缓冲, 设置 2 个等待周期 */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);

        /* HCLK = SYSCLK = 72MHz */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        /* PCLK2 = HCLK = 72MHz */
        RCC_PCLK2Config(RCC_HCLK_Div1);
        /* PCLK1 = HCLK/2 = 36MHz */
        RCC_PCLK1Config(RCC_HCLK_Div2);

        /* PLL: 8MHz / 2 * 9 = 36MHz * 2 = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    } else {
        /* HSE 启动失败: 使用内部 HSI 8MHz */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    }
}

/* ======================== 外设初始化 ======================== */
static void Peripherals_Init(void)
{
    /* --- 通信 --- */
    Debug_USART_Init(115200);          /* 调试串口 */
    Debug_Printf("\r\n======== 充电桩控制系统启动 ========\r\n");

    RS485_Init(9600);                  /* RS485 连接充电模块 */
    NRF24L01_Init();                   /* 2.4G 无线 */

    ESP8266_Init();                    /* WiFi 初始化 */
    Debug_Printf("[WiFi] ESP8266 初始化完成\r\n");

    /* --- 传感器 --- */
    MQ2_Init();
    LightSensor_Init();
    DHT11_Init();
    IR_Remote_Init();
    Debug_Printf("[Sensor] 传感器初始化完成\r\n");

    /* --- 存储 --- */
    W25Q64_Init();
    AT24C02_Init();
    SD_Init();
    {
        uint16_t flash_id = W25Q64_ReadID();
        Debug_Printf("[Flash] W25Q64 ID: 0x%04X\r\n", flash_id);
    }

    /* --- 显示 --- */
    LCD_Init();
    Touch_Init();
    LCD_Clear(WHITE);
    LCD_Draw_String(10, 10, "Charging Pile System v1.0", BLUE, WHITE);
    Debug_Printf("[LCD] TFT 初始化完成\r\n");

    /* --- 控制 --- */
    LED_Init();
    RGB_Init();
    Buzzer_Init();
    Relay_Init();
    Stepper_Init();
    DC_Motor_Init();
    Key_Init();

    Buzzer_Beep(200);   /* 上电提示音 */
}

/* ======================== 传感器更新 ======================== */
static void Sensor_Update(void)
{
    /* MQ-2 烟雾 */
    g_sensor.mq2_val = MQ2_Read();
    /* 粗略估算: ADC值 → ppm, 需实际标定 */
    g_sensor.mq2_ppm = (float)g_sensor.mq2_val * 100.0f / 4096.0f;

    /* 光照 */
    g_sensor.light_val = LightSensor_Read();

    /* DHT11 */
    DHT11_Read_Temperature_Humidity(&g_sensor.temperature, &g_sensor.humidity);
}

/* ======================== 显示更新 ======================== */
static void Display_Update(void)
{
    char buf[64];
    uint16_t y = 40;

    LCD_Fill(0, 40, 319, 479, WHITE);   /* 清除数据显示区 */

    /* 状态 */
    snprintf(buf, sizeof(buf), "State: ");
    LCD_Draw_String(10, y, buf, BLACK, WHITE);
    switch(g_state) {
        case STATE_IDLE:     LCD_Draw_String(70, y, "IDLE", GRAY, WHITE);   break;
        case STATE_AUTH:     LCD_Draw_String(70, y, "AUTH", BLUE, WHITE);   break;
        case STATE_CHARGING: LCD_Draw_String(70, y, "CHARGING", GREEN, WHITE); break;
        case STATE_DONE:     LCD_Draw_String(70, y, "DONE", CYAN, WHITE);   break;
        case STATE_ALARM:    LCD_Draw_String(70, y, "ALARM!", RED, WHITE);  break;
    }
    y += 20;

    /* 传感器数据 */
    snprintf(buf, sizeof(buf), "Temp: %dC  Humi: %d%%", g_sensor.temperature, g_sensor.humidity);
    LCD_Draw_String(10, y, buf, BLACK, WHITE); y += 20;

    snprintf(buf, sizeof(buf), "Smoke: %d (%d.%d%%)", g_sensor.mq2_val,
             (int)g_sensor.mq2_ppm, (int)(g_sensor.mq2_ppm * 10) % 10);
    LCD_Draw_String(10, y, buf, BLACK, WHITE); y += 20;

    snprintf(buf, sizeof(buf), "Light: %d", g_sensor.light_val);
    LCD_Draw_String(10, y, buf, BLACK, WHITE); y += 20;

    /* 充电数据 */
    snprintf(buf, sizeof(buf), "Voltage: %dmV  Current: %dmA",
             g_charge.voltage_mv, g_charge.current_ma);
    LCD_Draw_String(10, y, buf, BLACK, WHITE); y += 20;

    snprintf(buf, sizeof(buf), "Energy: %lu Wh", g_charge.energy_wh);
    LCD_Draw_String(10, y, buf, BLACK, WHITE); y += 20;

    /* WiFi 状态 */
    LCD_Draw_String(10, y, "WiFi: ", BLACK, WHITE);
    LCD_Draw_String(50, y, g_wifi_connected ? "Connected" : "Disconnected",
                    g_wifi_connected ? GREEN : RED, WHITE);
    y += 20;

    /* 底部提示 */
    LCD_Draw_String(10, 460, "KEY1:Start  KEY2:Stop  Key3:Reset", GRAY, WHITE);
}

/* ======================== 状态机 ======================== */
static void StateMachine_Run(void)
{
    switch(g_state) {
    case STATE_IDLE:
        RGB_Control(0, 1, 0);       /* 绿灯 - 待机 */

        /* 按键1: 开始充电 */
        if(Key_Scan() == KEY1) {
            g_state = STATE_AUTH;
            Debug_Printf("[State] AUTH\r\n");
        }

        /* 烟雾超标 → 报警 */
        if(g_sensor.mq2_ppm > 80.0f) {
            g_state = STATE_ALARM;
            Debug_Printf("[State] ALARM (Smoke!)\r\n");
        }
        break;

    case STATE_AUTH:
        RGB_Control(1, 1, 0);       /* 黄灯 - 等待认证 */

        /* 模拟认证通过 (实际项目读取 RFID/刷卡/手机扫码) */
        if(Key_Scan() == KEY1) {
            g_state = STATE_CHARGING;
            g_charge.start_tick = GET_TICK();
            g_charge.energy_wh = 0;
            Relay_Control(1);        /* 闭合继电器, 开始充电 */
            Buzzer_Beep(500);
            Debug_Printf("[State] CHARGING started\r\n");
        }
        break;

    case STATE_CHARGING:
        RGB_Control(1, 0, 0);       /* 红灯 - 充电中 */

        /* 模拟充电电流/电压 (实际通过 RS485 读取充电模块) */
        g_charge.voltage_mv = 220000;    /* 220V */
        g_charge.current_ma = 10000;     /* 10A */

        /* 累计电量 */
        uint32_t elapsed_ms = GET_TICK() - g_charge.start_tick;
        if(elapsed_ms > 0) {
            /* Wh = V * A * h */
            g_charge.energy_wh = (uint32_t)((uint32_t)g_charge.voltage_mv
                               * (uint32_t)g_charge.current_ma
                               * elapsed_ms) / (3600 * 1000 * 1000);
        }

        /* 按键2: 停止充电 */
        if(Key_Scan() == KEY2) {
            g_state = STATE_DONE;
            Relay_Control(0);          /* 断开继电器 */
            Debug_Printf("[State] DONE (user stop)\r\n");
        }
        break;

    case STATE_DONE:
        RGB_Control(0, 1, 1);       /* 青色 - 充电完成 */
        g_charge.current_ma = 0;

        /* 按键3: 复位到空闲 */
        if(Key_Scan() == KEY3) {
            g_state = STATE_IDLE;
            g_charge.energy_wh = 0;
            Debug_Printf("[State] IDLE (reset)\r\n");
        }
        break;

    case STATE_ALARM:
        RGB_Control(1, 0, 1);       /* 紫色闪烁 */
        Relay_Control(0);            /* 强制断电 */
        Buzzer_Beep(1000);

        /* 蜂鸣器每2秒蜂鸣一次 */
        static uint32_t last_alarm = 0;
        if(GET_TICK() - last_alarm > 2000) {
            Buzzer_Beep(300);
            last_alarm = GET_TICK();
        }

        /* 按键3: 解除报警 */
        if(Key_Scan() == KEY3) {
            g_state = STATE_IDLE;
            Buzzer_Off();
            Debug_Printf("[State] IDLE (alarm clear)\r\n");
        }
        break;
    }
}

/* ======================== 通信任务 ======================== */
static void Communication_Task(void)
{
    /* 每 5 秒更新一次 WiFi 和数据上报 */
    static uint32_t last_report = 0;

    if(GET_TICK() - last_report < 5000) return;
    last_report = GET_TICK();

    /* 尝试连接 WiFi (可用标志控制) */
    if(!g_wifi_connected) {
        if(ESP8266_Connect_AP("YourSSID", "YourPassword") == 0) {
            g_wifi_connected = 1;
            Debug_Printf("[WiFi] Connected\r\n");
            LED_Control(0, 1);     /* LED0 亮表示联网 */
        }
    }

    /* 上报数据到云端 */
    if(g_wifi_connected) {
        char json[256];
        snprintf(json, sizeof(json),
            "{\"temp\":%d,\"humi\":%d,\"smoke\":%d.%d,"
            "\"voltage\":%d,\"current\":%d,\"energy\":%lu,"
            "\"state\":%d}",
            g_sensor.temperature, g_sensor.humidity,
            (int)g_sensor.mq2_ppm, (int)(g_sensor.mq2_ppm * 10) % 10,
            g_charge.voltage_mv, g_charge.current_ma,
            g_charge.energy_wh, (int)g_state);

        ESP8266_Send_Data("api.charging-pile.com", 80, json);
        Debug_Printf("[Cloud] Data reported\r\n");
    }
}

/* ======================== RS485 充电模块读取 ======================== */
static void RS485_Task(void)
{
    static uint8_t cmd_read[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    uint8_t resp[32];

    RS485_Send(cmd_read, sizeof(cmd_read));
    uint8_t len = RS485_Recv(resp, sizeof(resp), 100);

    if(len > 0) {
        /* 解析 Modbus 响应, 更新电压电流 */
        /* resp[0]=地址, resp[1]=功能码, resp[2]=字节数, resp[3-4]=数据 */
        if(len >= 5 && resp[1] == 0x03) {
            g_charge.voltage_mv = ((uint16_t)resp[3] << 8) | resp[4];
        }
    }
}

/* ======================== 报警检测 ======================== */
static void Alarm_Check(void)
{
    /* 烟雾浓度超过阈值 */
    if(g_sensor.mq2_ppm > 80.0f && g_state != STATE_ALARM) {
        g_state = STATE_ALARM;
        Debug_Printf("[ALARM] 烟雾浓度超标! %.1f%%\r\n", g_sensor.mq2_ppm);
    }
}

/* ======================== 主函数 ======================== */
int main(void)
{
    /* 系统初始化 */
    System_Clock_Config();

    /* SysTick: 1ms 中断 */
    if(SysTick_Config(SystemCoreClock / 1000)) {
        while(1);
    }

    /* 外设初始化 */
    Peripherals_Init();

    Debug_Printf("系统启动成功, 72MHz, 进入主循环\r\n");

#if 0  /* 可选: NRF24L01 自检 */
    if(NRF24L01_Check()) {
        Debug_Printf("[NRF24L01] OK\r\n");
    } else {
        Debug_Printf("[NRF24L01] Not found\r\n");
    }
#endif

    /* ======================== 主循环 ======================== */
    while(1) {
        /* 1. 传感器采集 (每 500ms) */
        static uint32_t last_sensor = 0;
        if(GET_TICK() - last_sensor >= 500) {
            Sensor_Update();
            last_sensor = GET_TICK();
        }

        /* 2. 状态机运行 */
        StateMachine_Run();

        /* 3. 显示刷新 (每 200ms) */
        static uint32_t last_display = 0;
        if(GET_TICK() - last_display >= 200) {
            Display_Update();
            last_display = GET_TICK();
        }

        /* 4. 通信任务 (WiFi 数据上报) */
        Communication_Task();

        /* 5. RS485 读取充电模块 */
        static uint32_t last_rs485 = 0;
        if(GET_TICK() - last_rs485 >= 1000) {
            RS485_Task();
            last_rs485 = GET_TICK();
        }

        /* 6. 报警检测 */
        Alarm_Check();

        /* 7. 按键扫描 */
        Key_Scan();      /* 状态机中已处理, 这里保持调用 */

        /* 8. 系统空闲 - 可进入低功耗模式 */
        __NOP();
    }
}

/* ======================== TIM2 中断用户处理 ======================== */
void TIM2_IRQ_Handler_user(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        /* 周期性任务, 如 LED 闪烁等 */
    }
}

/* ======================== NRF24L01 测试 (外部调用) ======================== */
#if 0
void NRF24L01_Test(void)
{
    uint8_t tx_data[] = "Hello";
    uint8_t rx_data[32];
    uint8_t rx_len;

    NRF24L01_Set_TX_Mode();
    NRF24L01_Send_Packet(tx_data, 5);

    NRF24L01_Set_RX_Mode();
    if(NRF24L01_Recv_Packet(rx_data, &rx_len) == 0) {
        Debug_Printf("NRF24L01 recv: %s\r\n", rx_data);
    }
}
#endif

/**
 * @brief  C 库 printf 重定向 (Keil的 Use MicroLIB + 重写 fputc)
 */
#ifdef __GNUC__
#else
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}
#endif

/**
 * @brief  默认 fputc 用于 printf
 *         在 Keil 中开启 Use MicroLIB 即可用 printf 输出到调试串口
 */
#if 1
/* 标准库 printf 重定向 */
struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;
#endif
