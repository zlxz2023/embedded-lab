#include "step_motor.h"
#include "SysTick.h"

void STEP_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(MOTOR_IN1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_IN2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_IN3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_IN4_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR_IN1_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_IN1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR_IN2_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_IN1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR_IN3_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_IN1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR_IN4_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_IN1_PORT,&GPIO_InitStructure);
	
	
}

void Step_Motor_Run(u8 dir,u8 speed,u8 sta){
	u8 i=0;
	
	if(sta==1){
        if(dir==0){//À≥ ±’Î
			for(i=0;i<8;i++){
				switch(i){
					case 0: MOTOR_IN1=1;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=0;break;
					case 1: MOTOR_IN1=1;MOTOR_IN2=1;MOTOR_IN3=0;MOTOR_IN4=0;break;
					case 2: MOTOR_IN1=0;MOTOR_IN2=1;MOTOR_IN3=0;MOTOR_IN4=0;break;
					case 3: MOTOR_IN1=0;MOTOR_IN2=1;MOTOR_IN3=1;MOTOR_IN4=0;break;
					case 4: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=1;MOTOR_IN4=0;break;
					case 5: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=1;MOTOR_IN4=1;break;
					case 6: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=1;break;
					case 7: MOTOR_IN1=1;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=1;break;
				}
				delay_ms(speed);
			}
		}else{  //ƒÊ ±’Î
			for(i=0;i<8;i++){
				switch(i){
					case 0: MOTOR_IN1=1;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=1;break;
					case 1: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=1;break;
					case 2: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=1;MOTOR_IN4=1;break;
					case 3: MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=1;MOTOR_IN4=0;break;
					case 4: MOTOR_IN1=0;MOTOR_IN2=1;MOTOR_IN3=1;MOTOR_IN4=0;break;
					case 5: MOTOR_IN1=0;MOTOR_IN2=1;MOTOR_IN3=0;MOTOR_IN4=0;break;
					case 6: MOTOR_IN1=1;MOTOR_IN2=1;MOTOR_IN3=0;MOTOR_IN4=0;break;
					case 7: MOTOR_IN1=1;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=0;break;
				}
				delay_ms(speed);
			}
		}
	}else{
		 MOTOR_IN1=0;MOTOR_IN2=0;MOTOR_IN3=0;MOTOR_IN4=0;
	}
}
