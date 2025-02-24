#include "time.h"
#include "led.h"
#include "dht11.h"
#include "wifi_function.h"
#include "led.h"
#include "beep.h"

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
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 定时器使能
    TIM_Cmd(TIM4, ENABLE);
}

// 存储温度和湿度
u8 temp;  	    
u8 humi;
char buf[200];
char mqtt_cmd[100]; // 用于存放完整的 AT 指令
u8 send_data_flag=0;

void TIM4_IRQHandler(void){
    if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET){
        DHT11_Read_Data(&temp,&humi);
        // 格式化为 JSON 字符串
        sprintf(buf, "{\"temp\": %d, \"humi\": %d, \"led\": %d, \"beep\": %d}", temp, humi, (int)!LED1, (int)BEEP);
        // 设置标志，指示主循环发送数据
        send_data_flag = true;
    }
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
