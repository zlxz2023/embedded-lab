#ifndef _fun_H
#define _fun_H

#include "stm32g4xx.h"                  // Device header

void LCD_Show(void);
double GET_ADC_VOL(ADC_HandleTypeDef *hadc);

#endif
