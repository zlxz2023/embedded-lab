#ifndef _beep_H
#define _beep_H

#include "system.h"

#define BEEP_PORT_RCC   RCC_APB2Periph_GPIOB
#define BEEP_PIN        GPIO_Pin_8
#define BEEP_PORT       GPIOB
#define BEEP            PBout(8)

void BEEP_Init();

#endif
