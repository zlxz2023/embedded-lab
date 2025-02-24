#ifndef _step_motor_H
#define _step_motor_H

#include "system.h"

#define MOTOR_IN1_PORT      GPIOE
#define MOTOR_IN1_PIN       GPIO_Pin_1
#define MOTOR_IN1_PORT_RCC  RCC_APB2Periph_GPIOE

#define MOTOR_IN2_PORT      GPIOE
#define MOTOR_IN2_PIN       GPIO_Pin_2
#define MOTOR_IN2_PORT_RCC  RCC_APB2Periph_GPIOE

#define MOTOR_IN3_PORT      GPIOE
#define MOTOR_IN3_PIN       GPIO_Pin_3
#define MOTOR_IN3_PORT_RCC  RCC_APB2Periph_GPIOE

#define MOTOR_IN4_PORT      GPIOE
#define MOTOR_IN4_PIN       GPIO_Pin_4
#define MOTOR_IN4_PORT_RCC  RCC_APB2Periph_GPIOE

#define MOTOR_IN1 	PEout(1)
#define MOTOR_IN2 	PEout(2)
#define MOTOR_IN3 	PEout(3)
#define MOTOR_IN4 	PEout(4)

extern int curtain;
extern int stop_flag;

void STEP_Init(void);
void Step_Motor_Run(u8 dir,u8 speed,u8 sta);	

#endif
