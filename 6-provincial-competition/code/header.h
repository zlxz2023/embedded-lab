#ifndef _header_H
#define _header_H
#include "stm32g4xx.h"                  // Device header
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

#include "lcd.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "i2c_hal.h"

#define VDD 3.3

extern uint8_t send_flag, modify_flag, k_modify;
extern double V1, k;
extern uint8_t hours, minutes, seconds, page, hcount;
#endif
