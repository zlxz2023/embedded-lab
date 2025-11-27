#include "time.h"
#include "led.h"

void TIM4_Init(u16 per, u16 psc){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    // 定时器初始化
    TIM_TimeBaseInitStruct.TIM_Period=per;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
    // 定时器中断配置
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    // 配置NVIC
    NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 定时器使能
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET){
        LED2=!LED2;
    }
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
