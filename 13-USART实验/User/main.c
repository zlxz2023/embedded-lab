#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"

int main(){
    u8 i=0;
    u16 len=0;
    u16 j=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    
	while(1){
        if(USART1_RX_STATA & 0x8000){
            len=(USART1_RX_STATA & 0x3fff);
            for(j=0;j<len;j++){
                USART_SendData(USART1, USART1_RX_BUF[j]);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
            }
            USART1_RX_STATA=0;
        }
		i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
