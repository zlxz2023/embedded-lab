#ifndef __LDR_H
#define	__LDR_H

#include "system.h"

//#define		IR_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
//#define 	IR_DO_GPIO_PORT								GPIOA
//#define 	IR_DO_GPIO_PIN								GPIO_Pin_1	



#define IR_READ_TIMES	10

#define		IR_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	IR_AO_GPIO_PORT								GPIOA
#define 	IR_AO_GPIO_PIN								GPIO_Pin_0
#define     ADC_CHANNEL               		            ADC_Channel_0


void IR_Init(void);
uint16_t IR_FireData(void);

#endif