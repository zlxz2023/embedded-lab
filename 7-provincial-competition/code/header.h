#ifndef _header_H
#define _header_H
#include "stm32g4xx.h"                  // Device header
#include "stdio.h"
#include "stdint.h"
#include "string.h"

#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include "lcd.h"
#include "key.h"
#include "i2c_hal.h"
#include "led.h"

extern uint8_t page, line;
extern uint8_t th1, th2, th3;
extern double height;
extern uint8_t level, level_temp;
extern uint32_t level_led, rec_led;

#endif
