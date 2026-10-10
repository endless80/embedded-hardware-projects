#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_NONE    0
#define KEY1        1
#define KEY2        2
#define KEY3        3
#define KEY4        4

void    Key_Init(void);
uint8_t Key_Scan(void);          /* 返回按键编号, 无按键返回 KEY_NONE */
uint8_t Key_Scan_Long(void);     /* 长按检测 */

#endif
