#include "input.h"

u8 TIM5_CH1_CAPTURE_STA=0; //输入捕获状态
u16 TIM5_CH1_CAPTURE_VAL=0; //输入捕获值

void TIM5_CH1_INPUT_Init(u16 arr, u16 psc){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    
    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 初始化PA0
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	 //设置下拉输入模式
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */
    // 定时器初始化
    TIM_TimeBaseInitStruct.TIM_Period=arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
    // 输入捕获参数配置
    TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
    TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM_ICInitStruct.TIM_ICFilter=0;
    TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICInit(TIM5, &TIM_ICInitStruct);
    // 开启捕获和定时器更新（溢出）中断
    TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
    // 配置NVIC
    NVIC_InitStruct.NVIC_IRQChannel=TIM5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 定时器使能
    TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void){
    if((TIM5_CH1_CAPTURE_STA & 0x80)==0){
        // 产生溢出中断
        if(TIM_GetITStatus(TIM5, TIM_IT_Update)){
            if(TIM5_CH1_CAPTURE_STA & 0x40){// 捕获到上升沿
                if((TIM5_CH1_CAPTURE_STA & 0x3f)==0x3f){
                    TIM5_CH1_CAPTURE_STA |= 0x80;
                    TIM5_CH1_CAPTURE_VAL=0xffff; // 此时溢出次数超出范围，强制置为0xffff
                }else{
                    TIM5_CH1_CAPTURE_STA++;
                }
            }
        }
        // 产生捕获中断
        if(TIM_GetITStatus(TIM5, TIM_IT_CC1)){
            if(TIM5_CH1_CAPTURE_STA & 0x40){
                TIM5_CH1_CAPTURE_STA |= 0x80;
                TIM5_CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
                TIM_OC1NPolarityConfig(TIM5, TIM_ICPolarity_Rising);
            }else{
                TIM5_CH1_CAPTURE_STA=0;
                TIM5_CH1_CAPTURE_VAL=0;
                TIM5_CH1_CAPTURE_STA |= 0x40;
                TIM_Cmd(TIM5, DISABLE);
                TIM_SetCounter(TIM5, 0);
                TIM_OC1NPolarityConfig(TIM5, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM5, ENABLE);
            }
        }
    }
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);
}
