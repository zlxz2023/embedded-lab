#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "pwm.h"

int main(){
	u16 i=0;
	u8 flag=0;
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    TIM3_CH2_PWM_Init(500, 72-1); // 0.5ms
	while(1){
		if(flag==0){
            i++;
            if(i==300){
                flag=1;
            }
        }else{
            i--;
            if(i==0){
                flag=0;
            }
        }
        TIM_SetCompare2(TIM3, i);
		delay_ms(10);
	}
}
