#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "wwdg.h"

int main(){
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    // 使用LED1来指示是否完成喂狗，如果喂狗成功则LED2闪烁， 否则LED1闪烁
    LED1=0;
    delay_ms(500);
    LED1=1;
    WWDG_Init();
    
	while(1){
		
    }
}
