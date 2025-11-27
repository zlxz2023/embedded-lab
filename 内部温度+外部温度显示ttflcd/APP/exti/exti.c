#include "exti.h"
#include "SysTick.h"
#include "key.h"
#include "led.h"
#include "dht11.h"
#include "tftlcd.h"

u8 temp = 0;
u8 humi = 0;

void MY_EXTI_Init(){
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    
    // 输入时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    //设置IO口与中断线的映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    
    // NVIC 初始化
    // PA0 KEY_UP
    NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    // EXTI 初始化
    EXTI_InitStruct.EXTI_Line=EXTI_Line0;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
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
            char str[255];
            // 获取外部温度
            DHT11_Read_Data(&temp, &humi);
            sprintf(str, "%d ℃", temp);
            LCD_ShowFontHZ(10, 110,"外部温度");
            LCD_ShowString(130, 110 ,tftlcd_data.width,tftlcd_data.height,24,":");
            LCD_ShowString(150, 110 ,tftlcd_data.width,tftlcd_data.height,24,str);
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}
