#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"

int main()
{
	u8 i=0;
	
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	BEEP_Init();
	KEY_Init();
	MYEXTI_Init();
    
	while(1){
        
	}
}
