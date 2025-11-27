#ifndef _exti_H
#define _exti_H

#include "system.h"

void MY_EXTI_Init(void);
void EXTI4_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI0_IRQHandler(void);

#endif
