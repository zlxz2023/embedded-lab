#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "iwdg.h"
#include "key.h"

int main(){
	u8 i=0;
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    KEY_Init();
    IWDG_Init(4, 1100); // —” ±(ms)£∫4*2^pre/40 * rlr
    
    LED2=1;
	while(1){
		if(KEY_Scan(1)==KEY_UP_PRESS){
            IWDG_ReloadCounter(); // Œππ∑
            LED2=0;
        }
        i++;
//        if(i%50==0){
//            LED1=!LED1;
//        }
		delay_ms(10);
	}
}
