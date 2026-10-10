#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"

/* LCD 颜色 */
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define GREEN          0x07E0
#define CYAN           0x07FF
#define MAGENTA        0xF81F
#define YELLOW         0xFFE0
#define GRAY           0x8430

/* LCD 指令 */
#define LCD_WR_REG(cmd)   { *(volatile uint16_t *)LCD_REG_ADDR  = (cmd); }
#define LCD_WR_DATA(data) { *(volatile uint16_t *)LCD_DATA_ADDR = (data); }

/* FSMC 地址: NE4 = 0x6C000000, A10 (RS) = 0x400 */
#define LCD_BASE        ((uint32_t)(0x6C000000 | 0x00000000))  /* NE4 */
#define LCD_REG_ADDR    (LCD_BASE + 0x400)                     /* RS=1: 命令 */
#define LCD_DATA_ADDR   (LCD_BASE + 0x000)                     /* RS=0: 数据 */

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t id;
    uint8_t  dir;
} LCD_Info_t;

extern LCD_Info_t g_lcd;

void    LCD_Init(void);
void    LCD_Clear(uint16_t color);
void    LCD_Set_Window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void    LCD_Set_Cursor(uint16_t x, uint16_t y);
void    LCD_Draw_Point(uint16_t x, uint16_t y, uint16_t color);
void    LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void    LCD_Display_On(void);
void    LCD_Display_Off(void);

void    LCD_Draw_Char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void    LCD_Draw_String(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);
void    LCD_Draw_Num(uint16_t x, uint16_t y, uint32_t num, uint16_t color, uint16_t bg);
void    LCD_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void    LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

#endif
