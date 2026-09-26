#ifndef OLED_H
#define OLED_H

#include "main.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowString(uint8_t page, uint8_t col, const char *text);

#endif
