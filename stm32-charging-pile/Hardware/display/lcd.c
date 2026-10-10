/**
 * @file    lcd.c
 * @brief   TFT-LCD 驱动 (FSMC 8080 并口)
 *          原理图: FSMC_NE4 (PG12), FSMC_A10 (PG0), FSMC_D0~D15
 *          支持 ILI9341/ILI9481/ST7789 等常见驱动芯片
 */
#include "lcd.h"
#include "pin_mapping.h"

LCD_Info_t g_lcd;

static FSMC_NORSRAMInitTypeDef  FSMC_InitStructure;
static FSMC_NORSRAMTimingInitTypeDef  FSMC_Timing;
static GPIO_InitTypeDef         GPIO_InitStructure;

static uint16_t LCD_Read_Reg(uint16_t reg)
{
    LCD_WR_REG(reg);
    return *(volatile uint16_t *)LCD_DATA_ADDR;
}

static uint16_t LCD_Read_ID(void)
{
    uint16_t id;
    LCD_WR_REG(0xD3);  /* ILI9341 读 ID 指令 */
    LCD_Read_Reg(0);
    LCD_Read_Reg(0);
    id = LCD_Read_Reg(0);  /* 实际: 返回 9341 */
    if(id != 0x9341) {
        LCD_WR_REG(0x04);
        id = LCD_Read_Reg(0);   /* 其他驱动芯片 */
    }
    return id;
}

void LCD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD
                         | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG
                         | RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    /* FSMC 数据线 PD0~PD15 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_8
                                  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11
                                  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
                                  | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* PD4(NOE), PD5(NWE), PD7(NE? 可选的), PG12(NE4) */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* FSMC 数据线 PE7~PE15 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9
                                  | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
                                  | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* PG0(A10/RS), PG12(NE4) */
    GPIO_InitStructure.GPIO_Pin   = LCD_RS_PIN;
    GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;
    GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

    /* 背光 PF6 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = LCD_BL_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_BL_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LCD_BL_PORT, LCD_BL_PIN);     /* 开背光 */

    /* FSMC 配置: NOR/PSRAM Bank1, NE4 */
    FSMC_Timing.FSMC_AddressSetupTime      = 2;
    FSMC_Timing.FSMC_AddressHoldTime        = 0;
    FSMC_Timing.FSMC_DataSetupTime          = 4;
    FSMC_Timing.FSMC_BusTurnaroundDuration  = 0;
    FSMC_Timing.FSMC_CLKDivision            = 0;
    FSMC_Timing.FSMC_DataLatency            = 0;
    FSMC_Timing.FSMC_AccessMode             = FSMC_AccessMode_A;

    FSMC_InitStructure.FSMC_Bank               = FSMC_Bank1_NORSRAM_4;  /* NE4 */
    FSMC_InitStructure.FSMC_DataAddressMux      = FSMC_DataAddressMux_Disable;
    FSMC_InitStructure.FSMC_MemoryType          = FSMC_MemoryType_SRAM;
    FSMC_InitStructure.FSMC_MemoryDataWidth     = FSMC_MemoryDataWidth_16b;
    FSMC_InitStructure.FSMC_BurstAccessMode     = FSMC_BurstAccessMode_Disable;
    FSMC_InitStructure.FSMC_WaitSignalPolarity   = FSMC_WaitSignalPolarity_Low;
    FSMC_InitStructure.FSMC_WrapMode             = FSMC_WrapMode_Disable;
    FSMC_InitStructure.FSMC_WaitSignalActive     = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_InitStructure.FSMC_WriteOperation       = FSMC_WriteOperation_Enable;
    FSMC_InitStructure.FSMC_WaitSignal           = FSMC_WaitSignal_Disable;
    FSMC_InitStructure.FSMC_ExtendedMode         = FSMC_ExtendedMode_Disable;
    FSMC_InitStructure.FSMC_WriteBurst          = FSMC_WriteBurst_Disable;
    FSMC_InitStructure.FSMC_ReadWriteTimingStruct  = &FSMC_Timing;
    FSMC_InitStructure.FSMC_WriteTimingStruct       = &FSMC_Timing;
    FSMC_NORSRAMInit(&FSMC_InitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM_4, ENABLE);

    /* 读取 LCD ID */
    g_lcd.id = LCD_Read_ID();
    g_lcd.width  = 320;
    g_lcd.height = 480;

    /* 初始化序列 (以 ILI9341 为例, 其他 LCD 驱动需调整) */
    LCD_WR_REG(0x01);  DELAY_MS(120);   /* SW reset */
    LCD_WR_REG(0x11);  DELAY_MS(120);   /* Sleep out */
    LCD_WR_REG(0x29);                    /* Display ON */

    LCD_Clear(WHITE);
}

void LCD_Set_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_WR_REG(0x2A);   /* COLADDR */
    LCD_WR_DATA(x1 >> 8);  LCD_WR_DATA(x1 & 0xFF);
    LCD_WR_DATA(x2 >> 8);  LCD_WR_DATA(x2 & 0xFF);
    LCD_WR_REG(0x2B);   /* PAGEADDR */
    LCD_WR_DATA(y1 >> 8);  LCD_WR_DATA(y1 & 0xFF);
    LCD_WR_DATA(y2 >> 8);  LCD_WR_DATA(y2 & 0xFF);
    LCD_WR_REG(0x2C);   /* RAMWR */
}

void LCD_Set_Cursor(uint16_t x, uint16_t y)
{
    LCD_Set_Window(x, y, x, y);
}

void LCD_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Set_Cursor(x, y);
    LCD_WR_DATA(color);
}

void LCD_Clear(uint16_t color)
{
    uint32_t i, total = g_lcd.width * g_lcd.height;
    LCD_Set_Window(0, 0, g_lcd.width - 1, g_lcd.height - 1);
    for(i = 0; i < total; i++) {
        LCD_WR_DATA(color);
    }
}

void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint32_t i, total = (x2 - x1 + 1) * (y2 - y1 + 1);
    LCD_Set_Window(x1, y1, x2, y2);
    for(i = 0; i < total; i++) {
        LCD_WR_DATA(color);
    }
}

void LCD_Display_On(void)
{
    LCD_WR_REG(0x29);
    GPIO_SetBits(LCD_BL_PORT, LCD_BL_PIN);
}

void LCD_Display_Off(void)
{
    LCD_WR_REG(0x28);
    GPIO_ResetBits(LCD_BL_PORT, LCD_BL_PIN);
}

/* ============ 简单字符显示 (6x8 字体) ============ */
static const uint8_t ascii_6x8[][6] = {
  {0x00,0x00,0x00,0x00,0x00,0x00},  /* SPACE */
  {0x00,0x00,0x5F,0x00,0x00,0x00},  /* !    */
  {0x00,0x07,0x00,0x07,0x00,0x00},  /* " */
  /* ... 完整 ASCII 字库在项目中自行添加 ... */
};

void LCD_Draw_Char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg)
{
    uint8_t i, j;
    uint8_t *p = (uint8_t *)ascii_6x8[c - 0x20];
    for(i = 0; i < 6; i++) {
        uint8_t line = p[i];
        for(j = 0; j < 8; j++) {
            if(line & (0x80 >> j))
                LCD_Draw_Point(x + i, y + j, color);
            else
                LCD_Draw_Point(x + i, y + j, bg);
        }
    }
}

void LCD_Draw_String(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg)
{
    while(*str) {
        LCD_Draw_Char(x, y, *str++, color, bg);
        x += 6;
        if(x > g_lcd.width - 6) { x = 0; y += 8; }
    }
}

void LCD_Draw_Num(uint16_t x, uint16_t y, uint32_t num, uint16_t color, uint16_t bg)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%lu", num);
    LCD_Draw_String(x, y, buf, color, bg);
}

void LCD_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    /* Bresenham 画线算法 */
    int16_t dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int16_t dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;

    while(1) {
        LCD_Draw_Point(x1, y1, color);
        if(x1 == x2 && y1 == y2) break;
        int16_t e2 = 2 * err;
        if(e2 > -dy) { err -= dy; x1 += sx; }
        if(e2 <  dx) { err += dx; y1 += sy; }
    }
}

void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t x = 0, y = r, d = 3 - 2 * r;
    while(x <= y) {
        LCD_Draw_Point(x0 + x, y0 + y, color);
        LCD_Draw_Point(x0 - x, y0 + y, color);
        LCD_Draw_Point(x0 + x, y0 - y, color);
        LCD_Draw_Point(x0 - x, y0 - y, color);
        LCD_Draw_Point(x0 + y, y0 + x, color);
        LCD_Draw_Point(x0 - y, y0 + x, color);
        LCD_Draw_Point(x0 + y, y0 - x, color);
        LCD_Draw_Point(x0 - y, y0 - x, color);
        if(d < 0) d += 4 * x + 6;
        else { d += 4 * (x - y) + 10; y--; }
        x++;
    }
}
