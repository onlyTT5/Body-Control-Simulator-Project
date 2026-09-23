#ifndef APP_MAIN_H
#define APP_MAIN_H
#include "main.h"

void AppMain_Init(void);
void AppMain_Run(void);
static void AppMain_FlashLed(uint8_t count, uint32_t delay_ms);
static uint8_t AppMain_CanProtocolSelfTest(void);
#endif
