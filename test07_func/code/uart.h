#ifndef _uart_H
#define _uart_H
#include "stm32g4xx.h"                  // Device header

extern uint8_t rec_data;
extern char rec_buff[20];
extern char send_buff[20];
extern uint8_t buff_len;

void UART_Proc(uint8_t len);

#endif
