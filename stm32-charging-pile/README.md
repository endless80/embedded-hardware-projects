# 信盈达 STM32F103ZET6 充电桩远程控制系统

基于信盈达 STM32F103 开发板原理图的 Keil5 项目代码。

## 文件结构

```
STM32F103_ChargingPile/
- Hardware/
  - pin_mapping.h              # 中央引脚映射 (根据原理图定义)
  - sensor/                    # 传感器驱动
    - mq2.c/h                  # MQ-2 烟雾/可燃气体 (ADC, PC0)
    - dht11.c/h                # DHT11 温湿度 (单总线, PC2)
    - light_sensor.c/h         # 光敏电阻光照检测 (ADC, PC1)
    - ir_remote.c/h            # HS0038 红外接收 (NEC协议, PG15)
  - storage/                   # 存储驱动
    - w25q64.c/h               # W25Q64 SPI Flash (SPI1, PB3-CS)
    - at24c02.c/h              # AT24C02 EEPROM (I2C1, PB6/PB7)
    - sd_sdio.c/h              # SD卡 SDIO (PC8~PD2)
  - communication/             # 通信驱动
    - usart.c/h                # USART1/2/3 串口驱动+环形缓冲区
    - rs485.c/h                # SP3485 RS-485 (USART3 + PB12)
    - nrf24l01.c/h             # NRF24L01 2.4G (SPI2, PG6-CE)
    - esp8266.c/h              # ESP-12E WiFi (USART2, AT指令)
  - display/                   # 显示驱动
    - lcd.c/h                  # TFT-LCD (FSMC 8080, NE4)
    - touch.c/h                # 电阻触摸 XPT2046 (软SPI)
  - control/                   # 执行控制
    - relay.c/h                # 继电器 (PC5)
    - stepper.c/h              # ULN2003 步进电机 (PE0/1, PB4/5)
    - dc_motor.c/h             # L9110S 直流电机 (PB8/9, TIM4_PWM)
    - led.c/h                  # LED0-3 + RGB-LED (PF0~12)
  - buzzer/buzzer.c/h          # 有源蜂鸣器 (PF3)
  - key/key.c/h                # 4个按键 (PC13/14/15, PC3)
- User/
  - main.c                     # 主程序 (状态机 + 任务调度)
  - main.h                     # 公共头文件 + 函数声明
  - delay.c/h                  # 延时函数
  - stm32f10x_conf.h           # 外设库配置
  - stm32f10x_it.c/h           # 中断服务函数
- Startup/
  - startup_stm32f10x_hd.s     # 启动文件 (高密度, 144KB Flash)
- Libraries/
  - CMSIS/                     # ARM Cortex-M 内核文件
  - STM32F10x_StdPeriph_Driver/# STM32 标准外设库
- README.md
```

## 在 Keil5 中创建项目

### 1. 下载标准外设库
从 ST 官网下载 **STM32F10x Standard Peripheral Library**:
- 解压后将 `Libraries/CMSIS` 和 `Libraries/STM32F10x_StdPeriph_Driver` 复制到项目中

### 2. 创建 Keil 项目
1. 打开 Keil uVision5 -> **Project -> New uVision Project**
2. 选择芯片: **STM32F103ZE** (或 STM32F103ZET6)
3. 在 Manage Run-Time Environment 窗口点击 **OK** (不选择任何组件)
4. 在项目管理器 (Project窗口) 创建如下 Groups:
   - `Startup` -> 添加 `startup_stm32f10x_hd.s`
   - `User` -> 添加 `User/` 下所有 .c 文件
   - `Hardware/Sensor` -> 添加 `Hardware/sensor/*.c`
   - `Hardware/Storage` -> 添加 `Hardware/storage/*.c`
   - `Hardware/Comm` -> 添加 `Hardware/communication/*.c`
   - `Hardware/Display` -> 添加 `Hardware/display/*.c`
   - `Hardware/Control` -> 添加 `Hardware/control/*.c`
   - `Hardware/Other` -> 添加 `Hardware/buzzer/*.c`, `Hardware/key/*.c`
   - `SPL/src` -> 添加 `Libraries/STM32F10x_StdPeriph_Driver/src/` 下所有 .c 文件
     (至少包含: gpio, rcc, usart, spi, i2c, adc, tim, fsmc, sdio, exti, flash, dma)

### 3. 编译选项配置
**Target Options:**
- **Target -> Xtal**: `8.0 MHz` (外部晶振)
- **C/C++ -> Define**: `USE_STDPERIPH_DRIVER`, `STM32F10X_HD`
- **C/C++ -> Include Paths**: 添加如下路径:
  ```
  .\User
  .\Hardware
  .\Hardware\sensor
  .\Hardware\storage
  .\Hardware\communication
  .\Hardware\display
  .\Hardware\control
  .\Hardware\buzzer
  .\Hardware\key
  .\Libraries\CMSIS
  .\Libraries\CMSIS\startup
  .\Libraries\STM32F10x_StdPeriph_Driver\inc
  ```
- **Debug -> Settings -> Flash Download -> Reset and Run**: 勾选

## 引脚说明

| 模块 | 引脚 | 复用功能 |
|------|------|---------|
| 调试串口 | PA9(TX), PA10(RX) | USART1 -> CH340G |
| WiFi | PA2(TX), PA3(RX), PA4(EN) | USART2 -> ESP-12E |
| RS485 | PB10(TX), PB11(RX), PB12(RE/DE) | USART3 -> SP3485 |
| SPI Flash | PA5(SCK), PA6(MISO), PA7(MOSI), PB3(CS) | SPI1 -> W25Q64 |
| 2.4G无线 | PB13(SCK), PB14(MISO), PB15(MOSI), PG6(CE), PG7(CSN) | SPI2 -> NRF24L01 |
| EEPROM | PB6(SCL), PB7(SDA) | I2C1 -> AT24C02 |
| SD卡 | PC8~12, PD2 | SDIO -> TF卡座 |
| TFT-LCD | PG12(NE4), PG0(A10/RS), PD0~15, PE7~15 | FSMC 16位并口 |
| 触摸屏 | PF9(CS), PF10(PEN), PF8(MOSI), PF7(MISO), PF6(SCK) | 软件SPI -> XPT2046 |
| MQ-2 | PC0 | ADC1_IN10 |
| 光敏 | PC1 | ADC1_IN11 |
| DHT11 | PC2 | GPIO 单总线 |
| 红外接收 | PG15 | GPIO 外部中断 |
| 继电器 | PC5 | GPIO |
| 蜂鸣器 | PF3 | GPIO |
| LED 0-3 | PF4/5/11/12 | GPIO (低电平亮) |
| RGB-LED | PF0/1/2 | GPIO (高电平亮) |
| 按键1~4 | PC13/14/15, PC3 | GPIO 内部上拉 |
| 直流电机 | PB8, PB9 | TIM4_CH3/CH4 (PWM) |
| 步进电机 | PE0/1, PB4/5 | GPIO |

> 注意: 引脚映射定义在 `Hardware/pin_mapping.h` 中，所有宏以原理图网络标号为基准。
> 如果实物与原理图不一致，修改 `pin_mapping.h` 即可。

## 主程序功能

`main.c` 实现了充电桩控制系统的状态机:

| 状态 | 说明 | RGB颜色 | 继电器 |
|------|------|---------|--------|
| IDLE | 待机, 等待按键 | 绿灯 | 断开 |
| AUTH | 用户认证中 | 黄灯 | 断开 |
| CHARGING | 充电中, 采集数据 | 红灯 | 闭合 |
| DONE | 充电完成 | 青色 | 断开 |
| ALARM | 报警(烟雾超标) | 紫色闪烁 | 断开 |

**任务调度** (主循环轮询):
- 传感器采集: 每 500ms
- 显示刷新: 每 200ms
- WiFi 数据上报: 每 5s
- RS485 读取充电模块: 每 1s
- 报警检测: 每循环
- 按键扫描: 每循环

## 调试输出

USART1 (PA9/PA10) 通过 CH340G 连接到 USB，波特率 115200。
开启 Keil 的 MicroLIB 后，`printf()` 直接输出到调试串口。

## 需要注意的点

1. **SD卡驱动**: `sd_sdio.c` 中给出了 SDIO 框架，完整 SD 协议建议配合 FatFS 文件系统一起移植
2. **LCD初始化序列**: `lcd.c` 中的初始化序列以 ILI9341 为例，实际 LCD 驱动芯片（如 ILI9481/ST7789/RM68090）需更改初始化命令
3. **触摸校准**: `touch.c` 中的坐标转换是线性的，实际使用需要做三点或五点校准
4. **MQ-2标定**: ppm 转换是估算值，需要根据实际传感器手册做标定
5. **ESP8266参数**: 修改 `main.c` 中 `Communication_Task()` 里的 SSID、密码和服务器地址
6. **RS485协议**: 示例用的是 Modbus 03 功能码读取寄存器，实际充电模块协议需调整

## License

本项目代码基于学习目的开放，可自由修改使用。
