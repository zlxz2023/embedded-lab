#ifndef _key_H
#define _key_H
#include "stm32g4xx.h"                  // Device header

#define B1_PRESS 1
#define B2_PRESS 2
#define B3_PRESS 3
#define B4_PRESS 4

uint8_t KEY_Scan(void);
void KEY_Detection(void);
#endif
