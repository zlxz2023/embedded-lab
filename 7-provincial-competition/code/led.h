#ifndef _led_H
#define _led_H
#include "stm32g4xx.h"                  // Device header

extern uint8_t rec_data, time;
extern uint8_t rec_flag;

void LED_Show(uint8_t led, uint8_t led_mode);
void UART_Proc(void);

#endif
