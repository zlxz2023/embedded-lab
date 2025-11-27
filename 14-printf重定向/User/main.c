#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"

int main(){
    u8 i=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    
	while(1){
		i++;
        if(i%50==0){
            LED1=!LED1;
            printf("%d", 123);
        }
        delay_ms(10);
    }
}
