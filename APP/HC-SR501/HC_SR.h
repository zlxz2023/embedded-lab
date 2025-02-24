#ifndef HC_SR
#define HC_SR

#include "system.h"
 
#define		BODY_HW_GPIO_CLK								RCC_APB2Periph_GPIOB
#define 	BODY_HW_GPIO_PORT								GPIOB
#define 	BODY_HW_GPIO_PIN								GPIO_Pin_7	

extern int exist_p;

void BODY_HW_Init(void);
uint16_t BODY_HW_GetData(void);

#endif
