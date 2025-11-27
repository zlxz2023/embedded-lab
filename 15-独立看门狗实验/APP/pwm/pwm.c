#include "pwm.h"

// 重载值，预分频值
void TIM3_CH2_PWM_Init(u16 per, u16 psc){
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 使能定时器、端口、时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置GPIO部分映射
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 定时器初始化
    TIM_TimeBaseInitStruct.TIM_Period=per;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    
    // 初始化PWM_CH2
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    
     // 使能重载值配置
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // 最大计数值配置
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    // 使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}
