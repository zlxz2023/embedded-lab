#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "input.h"

int main(){
    u32 dat=0;
    u8 i=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    TIM5_CH1_INPUT_Init(0xffff, 72-1);
	while(1){
		if(TIM5_CH1_CAPTURE_STA & 0x80){
            dat = (TIM5_CH1_CAPTURE_STA & 0x3f)*0xffff + TIM5_CH1_CAPTURE_VAL;
            printf("高电平持续时间：%d us\n", dat);
            TIM5_CH1_CAPTURE_STA=0;
        }
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
