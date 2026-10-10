# Embedded Hardware Projects

嵌入式硬件与单片机项目集合，内容覆盖 STM32 固件、PCB 原理图设计、PCB 布局布线、打样验证和板级调试。

## 项目列表

### STM32F103 充电桩远程控制系统

目录：`stm32-charging-pile/`

- 基于 STM32F103ZET6 与 Keil MDK 的完整工程
- 充电流程包含待机、握手、充电、故障和结束 5 种状态
- 包含 OLED 菜单、UART 串口协议、上位机通信、看门狗与掉电保护
- 集成 ADC、DHT11、继电器、RS485、ESP8266、W25Q64、AT24C02 等驱动
- 项目说明：[stm32-charging-pile/README.md](stm32-charging-pile/README.md)

### 51 单片机核心板

目录：`pcb/51-mcu-core-board/`

- 使用嘉立创 EDA Pro 完成原理图和 PCB 设计
- 覆盖电源、晶振、复位、去耦和接口电路
- 工程文件：`51-mcu-core-board.epro`
- 说明：[pcb/51-mcu-core-board/README.md](pcb/51-mcu-core-board/README.md)

### USB 2.0 拓展坞

目录：`pcb/usb-2.0-hub/`

- 使用嘉立创 EDA Pro 完成 USB 2.0 拓展坞原理图与 PCB 设计
- 主控芯片为 SL2.1A，包含 USB Type-C 输入和 USB-A 输出接口
- 工程文件：`usb-2.0-hub.epro`
- 说明：[pcb/usb-2.0-hub/README.md](pcb/usb-2.0-hub/README.md)

### STM32 单片机开发板

目录：`pcb/stm32-dev-board/`

- 嘉立创 EDA Pro 工程，包含主控、电源、接口等原理图页
- 工程文件：`stm32-dev-board.epro`
- 说明：[pcb/stm32-dev-board/README.md](pcb/stm32-dev-board/README.md)

## 技术文档

- [充电桩系统设计说明](docs/charging-pile-system.md)
- [PCB 设计与调试记录](docs/pcb-design-debugging.md)
- [FPGA 北斗时间解析与显示项目说明](docs/fpga-time-display.md)

## 工具链

- 固件：Keil uVision 5、STM32F10x Standard Peripheral Library
- FPGA：Quartus II、ModelSim、Verilog HDL
- PCB：嘉立创 EDA Pro、Altium Designer
- 调试：串口调试助手、万用表、示波器、JTAG

## 说明

仓库用于展示个人学习和实践项目。第三方芯片厂商库、元器件符号和封装资源遵循其原始许可与使用条款。
