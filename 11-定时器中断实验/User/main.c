#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "time.h"

int main(){
	u8 i=0;
	
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    TIM4_Init(1000, 3600-1); // 72MHz
	while(1){
		i++;
		if(i%20==0){
			LED1=!LED1;      //LED1×´Ì¬È¡·´
		}
		delay_ms(10);
	}
}
