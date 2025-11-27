#ifndef _input_H
#define _input_H

#include "system.h"

extern u8 TIM5_CH1_CAPTURE_STA;
extern u16 TIM5_CH1_CAPTURE_VAL;

void TIM5_CH1_INPUT_Init(u16 arr, u16 psc);

#endif
