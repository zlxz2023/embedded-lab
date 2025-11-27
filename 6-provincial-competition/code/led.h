#ifndef _led_H
#define _led_H
#include "stm32g4xx.h"                  // Device header

void LED_Show(uint8_t led, uint8_t led_mode);
void LED_Proc(void);

extern uint32_t led_count;
extern uint8_t led_flag;
#endif
