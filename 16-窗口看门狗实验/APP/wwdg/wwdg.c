#include "wwdg.h"
#include "led.h"
#include "SysTick.h"

void WWDG_Init(void){
    NVIC_InitTypeDef NVIC_InitStructure;
    // 使能窗口看门狗时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    
    // 设置上限值
    WWDG_SetWindowValue(0x5f);
    
    // 设置分频系数
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    
    // 开启中断并分组
    // NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 传递初始值
    WWDG_Enable(0x7f);
    // 清除中断状态标志
    WWDG_ClearFlag();
    // 开启WWDG中断 使其能在0x40时发生中断（WWDG_CFR的EWI赋值为1）
    WWDG_EnableIT();   
}

void WWDG_IRQHandler(void){
    // 如果延时超过一个计数周期则无法完成喂狗，系统复位，重新执行main函数
    //delay_ms(10);
    // 喂狗
    WWDG_SetCounter(0x7f);
    // 清除状态标志
    WWDG_ClearFlag();
    // LED2闪烁
    LED2=!LED2;
}
