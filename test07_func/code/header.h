#ifndef _header_H
#define _header_H

#include "stm32g4xx.h"                  // Device header
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "main.h"
#include "gpio.h"
#include "lcd.h"
#include "usart.h"
#include "adc.h"

#include "led.h"
#include "key.h"
#include "uart.h"

extern uint8_t page;
extern uint8_t line;
extern uint8_t count1,count2,count3;
extern uint8_t time;
#endif
