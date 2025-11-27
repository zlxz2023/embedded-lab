#ifndef _uart_H
#define _uart_H
#include "stm32g4xx.h"                  // Device header

extern uint8_t rec_data, time;


void UART_Proc(void);

#endif
