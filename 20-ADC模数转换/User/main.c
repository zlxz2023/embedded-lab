#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"

int main(){
    u16 dat=0;
    u8 i=0;
    float vol=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    MY_ADC_Init();
    
	while(1){
		
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
        
        if(i%100==0){
            dat = GET_ADC_VAL(ADC_Channel_1, 20);
            printf("检测的AD值是 %d\n", dat);  // 0-4095
            vol = dat*3.3/4096;
            printf("检测的电压值为 %.2f V\n", vol);
        }
    }
}
