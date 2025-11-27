#ifndef _pwm_H
#define _pwm_H

#include "system.h"

void PWM1(u16 arr,u16 psc,u16 ccr);
void PWM2(u16 arr,u16 psc,u16 ccr);
void PWM3(u16 arr,u16 psc,u16 ccr);
void PWM4(u32 arr,u32 psc,u32 ccr);
void TIM3_CH1_PWM_Init(u16 arr,u16 psc);
void TIM3_CH2_PWM_Init(u16 arr,u16 psc);
void TIM3_CH3_PWM_Init(u16 arr,u16 psc);
void TIM3_CH4_PWM_Init(u32 arr,u32 psc);

#endif
