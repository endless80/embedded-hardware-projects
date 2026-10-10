;******************************************************************************
; startup_stm32f10x_hd.s - STM32F103 高密度器件启动文件 (144KB+ Flash)
; 适用于: STM32F103ZET6 (512KB Flash, 64KB SRAM, LQFP144)
;******************************************************************************
;
; 向量表
;
                PRESERVE8
                THUMB

; 外部函数引用
                IMPORT  SystemInit
                IMPORT  __main

; 外部中断句柄引用
                IMPORT  NMI_Handler
                IMPORT  HardFault_Handler
                IMPORT  MemManage_Handler
                IMPORT  BusFault_Handler
                IMPORT  UsageFault_Handler
                IMPORT  SVC_Handler
                IMPORT  DebugMon_Handler
                IMPORT  PendSV_Handler
                IMPORT  SysTick_Handler

                IMPORT  WWDG_IRQHandler
                IMPORT  PVD_IRQHandler
                IMPORT  TAMPER_IRQHandler
                IMPORT  RTC_IRQHandler
                IMPORT  FLASH_IRQHandler
                IMPORT  RCC_IRQHandler
                IMPORT  EXTI0_IRQHandler
                IMPORT  EXTI1_IRQHandler
                IMPORT  EXTI2_IRQHandler
                IMPORT  EXTI3_IRQHandler
                IMPORT  EXTI4_IRQHandler
                IMPORT  DMA1_Channel1_IRQHandler
                IMPORT  DMA1_Channel2_IRQHandler
                IMPORT  DMA1_Channel3_IRQHandler
                IMPORT  DMA1_Channel4_IRQHandler
                IMPORT  DMA1_Channel5_IRQHandler
                IMPORT  DMA1_Channel6_IRQHandler
                IMPORT  DMA1_Channel7_IRQHandler
                IMPORT  ADC1_2_IRQHandler
                IMPORT  USB_HP_CAN1_TX_IRQHandler
                IMPORT  USB_LP_CAN1_RX0_IRQHandler
                IMPORT  CAN1_RX1_IRQHandler
                IMPORT  CAN1_SCE_IRQHandler
                IMPORT  EXTI9_5_IRQHandler
                IMPORT  TIM1_BRK_IRQHandler
                IMPORT  TIM1_UP_IRQHandler
                IMPORT  TIM1_TRG_COM_IRQHandler
                IMPORT  TIM1_CC_IRQHandler
                IMPORT  TIM2_IRQHandler
                IMPORT  TIM3_IRQHandler
                IMPORT  TIM4_IRQHandler
                IMPORT  I2C1_EV_IRQHandler
                IMPORT  I2C1_ER_IRQHandler
                IMPORT  I2C2_EV_IRQHandler
                IMPORT  I2C2_ER_IRQHandler
                IMPORT  SPI1_IRQHandler
                IMPORT  SPI2_IRQHandler
                IMPORT  USART1_IRQHandler
                IMPORT  USART2_IRQHandler
                IMPORT  USART3_IRQHandler
                IMPORT  EXTI15_10_IRQHandler
                IMPORT  RTCAlarm_IRQHandler
                IMPORT  USBWakeUp_IRQHandler
                IMPORT  TIM8_BRK_IRQHandler
                IMPORT  TIM8_UP_IRQHandler
                IMPORT  TIM8_TRG_COM_IRQHandler
                IMPORT  TIM8_CC_IRQHandler
                IMPORT  ADC3_IRQHandler
                IMPORT  FSMC_IRQHandler
                IMPORT  SDIO_IRQHandler
                IMPORT  TIM5_IRQHandler
                IMPORT  SPI3_IRQHandler
                IMPORT  UART4_IRQHandler
                IMPORT  UART5_IRQHandler
                IMPORT  TIM6_IRQHandler
                IMPORT  TIM7_IRQHandler
                IMPORT  DMA2_Channel1_IRQHandler
                IMPORT  DMA2_Channel2_IRQHandler
                IMPORT  DMA2_Channel3_IRQHandler
                IMPORT  DMA2_Channel4_IRQHandler
                IMPORT  DMA2_Channel5_IRQHandler

; 默认中断句柄 (弱定义, 用户可覆盖)
                AREA    |.text|, CODE, READONLY

WWDG_IRQHandler             PROC  EXPORT  WWDG_IRQHandler           [WEAK]  B .  ENDP
PVD_IRQHandler              PROC  EXPORT  PVD_IRQHandler            [WEAK]  B .  ENDP
TAMPER_IRQHandler           PROC  EXPORT  TAMPER_IRQHandler         [WEAK]  B .  ENDP
RTC_IRQHandler              PROC  EXPORT  RTC_IRQHandler            [WEAK]  B .  ENDP
FLASH_IRQHandler            PROC  EXPORT  FLASH_IRQHandler          [WEAK]  B .  ENDP
RCC_IRQHandler              PROC  EXPORT  RCC_IRQHandler            [WEAK]  B .  ENDP
EXTI1_IRQHandler            PROC  EXPORT  EXTI1_IRQHandler          [WEAK]  B .  ENDP
EXTI2_IRQHandler            PROC  EXPORT  EXTI2_IRQHandler          [WEAK]  B .  ENDP
EXTI3_IRQHandler            PROC  EXPORT  EXTI3_IRQHandler          [WEAK]  B .  ENDP
EXTI4_IRQHandler            PROC  EXPORT  EXTI4_IRQHandler          [WEAK]  B .  ENDP
DMA1_Channel1_IRQHandler    PROC  EXPORT  DMA1_Channel1_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel2_IRQHandler    PROC  EXPORT  DMA1_Channel2_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel3_IRQHandler    PROC  EXPORT  DMA1_Channel3_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel4_IRQHandler    PROC  EXPORT  DMA1_Channel4_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel5_IRQHandler    PROC  EXPORT  DMA1_Channel5_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel6_IRQHandler    PROC  EXPORT  DMA1_Channel6_IRQHandler  [WEAK]  B .  ENDP
DMA1_Channel7_IRQHandler    PROC  EXPORT  DMA1_Channel7_IRQHandler  [WEAK]  B .  ENDP
ADC1_2_IRQHandler           PROC  EXPORT  ADC1_2_IRQHandler         [WEAK]  B .  ENDP
USB_HP_CAN1_TX_IRQHandler   PROC  EXPORT  USB_HP_CAN1_TX_IRQHandler [WEAK]  B .  ENDP
USB_LP_CAN1_RX0_IRQHandler  PROC  EXPORT  USB_LP_CAN1_RX0_IRQHandler[WEAK]  B .  ENDP
CAN1_RX1_IRQHandler         PROC  EXPORT  CAN1_RX1_IRQHandler       [WEAK]  B .  ENDP
CAN1_SCE_IRQHandler         PROC  EXPORT  CAN1_SCE_IRQHandler       [WEAK]  B .  ENDP
EXTI9_5_IRQHandler          PROC  EXPORT  EXTI9_5_IRQHandler        [WEAK]  B .  ENDP
TIM1_BRK_IRQHandler         PROC  EXPORT  TIM1_BRK_IRQHandler       [WEAK]  B .  ENDP
TIM1_UP_IRQHandler          PROC  EXPORT  TIM1_UP_IRQHandler        [WEAK]  B .  ENDP
TIM1_TRG_COM_IRQHandler     PROC  EXPORT  TIM1_TRG_COM_IRQHandler   [WEAK]  B .  ENDP
TIM1_CC_IRQHandler          PROC  EXPORT  TIM1_CC_IRQHandler        [WEAK]  B .  ENDP
TIM3_IRQHandler             PROC  EXPORT  TIM3_IRQHandler           [WEAK]  B .  ENDP
TIM4_IRQHandler             PROC  EXPORT  TIM4_IRQHandler           [WEAK]  B .  ENDP
I2C1_EV_IRQHandler          PROC  EXPORT  I2C1_EV_IRQHandler        [WEAK]  B .  ENDP
I2C1_ER_IRQHandler          PROC  EXPORT  I2C1_ER_IRQHandler        [WEAK]  B .  ENDP
I2C2_EV_IRQHandler          PROC  EXPORT  I2C2_EV_IRQHandler        [WEAK]  B .  ENDP
I2C2_ER_IRQHandler          PROC  EXPORT  I2C2_ER_IRQHandler        [WEAK]  B .  ENDP
SPI1_IRQHandler             PROC  EXPORT  SPI1_IRQHandler           [WEAK]  B .  ENDP
SPI2_IRQHandler             PROC  EXPORT  SPI2_IRQHandler           [WEAK]  B .  ENDP
EXTI15_10_IRQHandler        PROC  EXPORT  EXTI15_10_IRQHandler      [WEAK]  B .  ENDP
RTCAlarm_IRQHandler         PROC  EXPORT  RTCAlarm_IRQHandler       [WEAK]  B .  ENDP
USBWakeUp_IRQHandler        PROC  EXPORT  USBWakeUp_IRQHandler      [WEAK]  B .  ENDP
TIM8_BRK_IRQHandler         PROC  EXPORT  TIM8_BRK_IRQHandler       [WEAK]  B .  ENDP
TIM8_UP_IRQHandler          PROC  EXPORT  TIM8_UP_IRQHandler        [WEAK]  B .  ENDP
TIM8_TRG_COM_IRQHandler     PROC  EXPORT  TIM8_TRG_COM_IRQHandler   [WEAK]  B .  ENDP
TIM8_CC_IRQHandler          PROC  EXPORT  TIM8_CC_IRQHandler        [WEAK]  B .  ENDP
ADC3_IRQHandler             PROC  EXPORT  ADC3_IRQHandler           [WEAK]  B .  ENDP
FSMC_IRQHandler             PROC  EXPORT  FSMC_IRQHandler           [WEAK]  B .  ENDP
TIM5_IRQHandler             PROC  EXPORT  TIM5_IRQHandler           [WEAK]  B .  ENDP
SPI3_IRQHandler             PROC  EXPORT  SPI3_IRQHandler           [WEAK]  B .  ENDP
UART4_IRQHandler            PROC  EXPORT  UART4_IRQHandler          [WEAK]  B .  ENDP
UART5_IRQHandler            PROC  EXPORT  UART5_IRQHandler          [WEAK]  B .  ENDP
TIM6_IRQHandler             PROC  EXPORT  TIM6_IRQHandler           [WEAK]  B .  ENDP
TIM7_IRQHandler             PROC  EXPORT  TIM7_IRQHandler           [WEAK]  B .  ENDP
DMA2_Channel1_IRQHandler    PROC  EXPORT  DMA2_Channel1_IRQHandler  [WEAK]  B .  ENDP
DMA2_Channel2_IRQHandler    PROC  EXPORT  DMA2_Channel2_IRQHandler  [WEAK]  B .  ENDP
DMA2_Channel3_IRQHandler    PROC  EXPORT  DMA2_Channel3_IRQHandler  [WEAK]  B .  ENDP
DMA2_Channel4_IRQHandler    PROC  EXPORT  DMA2_Channel4_IRQHandler  [WEAK]  B .  ENDP
DMA2_Channel5_IRQHandler    PROC  EXPORT  DMA2_Channel5_IRQHandler  [WEAK]  B .  ENDP

; 堆栈配置
Stack_Size      EQU     0x00001000
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

Heap_Size       EQU     0x00000200
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

; 复位向量表
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                ; 0 堆栈指针
                DCD     Reset_Handler               ; 1 复位
                DCD     NMI_Handler                 ; 2 NMI
                DCD     HardFault_Handler            ; 3 Hard Fault
                DCD     MemManage_Handler            ; 4 MemManage
                DCD     BusFault_Handler             ; 5 Bus Fault
                DCD     UsageFault_Handler           ; 6 Usage Fault
                DCD     0                           ; 7 保留
                DCD     0                           ; 8 保留
                DCD     0                           ; 9 保留
                DCD     0                           ; 10 保留
                DCD     SVC_Handler                 ; 11 SVCall
                DCD     DebugMon_Handler             ; 12 Debug Monitor
                DCD     0                           ; 13 保留
                DCD     PendSV_Handler               ; 14 PendSV
                DCD     SysTick_Handler              ; 15 SysTick
; 外部中断
                DCD     WWDG_IRQHandler              ; 16
                DCD     PVD_IRQHandler               ; 17
                DCD     TAMPER_IRQHandler            ; 18
                DCD     RTC_IRQHandler               ; 19
                DCD     FLASH_IRQHandler             ; 20
                DCD     RCC_IRQHandler               ; 21
                DCD     EXTI0_IRQHandler             ; 22
                DCD     EXTI1_IRQHandler             ; 23
                DCD     EXTI2_IRQHandler             ; 24
                DCD     EXTI3_IRQHandler             ; 25
                DCD     EXTI4_IRQHandler             ; 26
                DCD     DMA1_Channel1_IRQHandler     ; 27
                DCD     DMA1_Channel2_IRQHandler     ; 28
                DCD     DMA1_Channel3_IRQHandler     ; 29
                DCD     DMA1_Channel4_IRQHandler     ; 30
                DCD     DMA1_Channel5_IRQHandler     ; 31
                DCD     DMA1_Channel6_IRQHandler     ; 32
                DCD     DMA1_Channel7_IRQHandler     ; 33
                DCD     ADC1_2_IRQHandler             ; 34
                DCD     USB_HP_CAN1_TX_IRQHandler    ; 35
                DCD     USB_LP_CAN1_RX0_IRQHandler   ; 36
                DCD     CAN1_RX1_IRQHandler          ; 37
                DCD     CAN1_SCE_IRQHandler          ; 38
                DCD     EXTI9_5_IRQHandler           ; 39
                DCD     TIM1_BRK_IRQHandler          ; 40
                DCD     TIM1_UP_IRQHandler           ; 41
                DCD     TIM1_TRG_COM_IRQHandler      ; 42
                DCD     TIM1_CC_IRQHandler           ; 43
                DCD     TIM2_IRQHandler              ; 44
                DCD     TIM3_IRQHandler              ; 45
                DCD     TIM4_IRQHandler              ; 46
                DCD     I2C1_EV_IRQHandler           ; 47
                DCD     I2C1_ER_IRQHandler           ; 48
                DCD     I2C2_EV_IRQHandler           ; 49
                DCD     I2C2_ER_IRQHandler           ; 50
                DCD     SPI1_IRQHandler              ; 51
                DCD     SPI2_IRQHandler              ; 52
                DCD     USART1_IRQHandler            ; 53
                DCD     USART2_IRQHandler            ; 54
                DCD     USART3_IRQHandler            ; 55
                DCD     EXTI15_10_IRQHandler         ; 56
                DCD     RTCAlarm_IRQHandler          ; 57
                DCD     USBWakeUp_IRQHandler         ; 58
                DCD     TIM8_BRK_IRQHandler          ; 59
                DCD     TIM8_UP_IRQHandler           ; 60
                DCD     TIM8_TRG_COM_IRQHandler      ; 61
                DCD     TIM8_CC_IRQHandler           ; 62
                DCD     ADC3_IRQHandler              ; 63
                DCD     FSMC_IRQHandler              ; 64
                DCD     SDIO_IRQHandler              ; 65
                DCD     TIM5_IRQHandler              ; 66
                DCD     SPI3_IRQHandler              ; 67
                DCD     UART4_IRQHandler             ; 68
                DCD     UART5_IRQHandler             ; 69
                DCD     TIM6_IRQHandler              ; 70
                DCD     TIM7_IRQHandler              ; 71
                DCD     DMA2_Channel1_IRQHandler     ; 72
                DCD     DMA2_Channel2_IRQHandler     ; 73
                DCD     DMA2_Channel3_IRQHandler     ; 74
                DCD     DMA2_Channel4_IRQHandler     ; 75
                DCD     DMA2_Channel5_IRQHandler     ; 76
__Vectors_End
__Vectors_Size  EQU     __Vectors_End - __Vectors

; 复位处理
                AREA    |.text|, CODE, READONLY
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; 用户堆栈初始化 (用于 C 库)
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap
                LDR     R0, =Heap_Mem
                LDR     R1, =Stack_Mem
                LDR     R2, =__heap_limit
                LDR     R3, =__initial_sp
                BX      LR

                ALIGN
                END
