#ifndef _adc_H
#define _adc_H

#include "system.h"

extern float ppm;

void MY_ADC_Init(void);
float GET_ADC_VAL(u8 ch, u8 times);
float Get_PPM(float vol);

#endif
