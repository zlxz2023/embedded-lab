#include "exti.h"
#include "SysTick.h"
#include "key.h"
#include "led.h"
#include "beep.h"

void MY_EXTI_Init(){
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    
    // 输入时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    //设置IO口与中断线的映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
    
    // NVIC 初始化
    // PA0 KEY_UP
    NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    //PE2 KEY2
    NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // PE3 KEY1
    NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // PE4 KEY0
    NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    // EXTI 初始化
    EXTI_InitStruct.EXTI_Line=EXTI_Line0;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    
    EXTI_InitStruct.EXTI_Line=EXTI_Line4 | EXTI_Line2 | EXTI_Line3;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruct);
}

// 中断服务程序
// KEY_UP
void EXTI0_IRQHandler(){
    if(EXTI_GetITStatus(EXTI_Line0)==SET){
        delay_ms(10); // 消抖
        if(KEY_UP==1){
            LED2=0;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}
// KEY2
void EXTI2_IRQHandler(){
    if(EXTI_GetITStatus(EXTI_Line2)==SET){
        delay_ms(10); // 消抖
        if(KEY2==1){
            BEEP=1;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line2);
}
// KEY1
void EXTI3_IRQHandler(){
    if(EXTI_GetITStatus(EXTI_Line3)==SET){
        delay_ms(10); // 消抖
        if(KEY_UP==1){
            LED2=1;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
}
// KEY0
void EXTI4_IRQHandler(){
    if(EXTI_GetITStatus(EXTI_Line4)==SET){
        delay_ms(10); // 消抖
        if(KEY_UP==1){
            BEEP=0;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
}
