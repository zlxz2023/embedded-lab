#ifndef __MQ2FIRE_H__
#define __MQ2FIRE_H__

#include "system.h"

void MY_ADC_Init(void);
void GET_ADC_AVG_VALS(uint16_t *avg_val1, uint16_t *avg_val2, uint16_t times);
float Get_MQ135_PPM(uint16_t mq135);
float Get_PPM(uint16_t mq2);

#endif
