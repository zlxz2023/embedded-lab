#ifndef _lsens_H
#define _lsens_H

#include "system.h"

void Lsens_Init(void);
u16 GET_ADC_LIGHT_VAL(u8 ch, u8 times);
u8 GET_LSENS(void);
#endif
