#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc_temp.h"

int main(){
    u8 i=0;
    float temp=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    ADC_TEMP_Init();
    
	while(1){
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
        
        if(i%100==0){
            temp=GET_TEMPERTURE();
//            if(temp<0){
//                printf("内部温度检测值为 -");
//            }else{
//                printf("内部温度检测值为 +");
//            }
            printf("%.2f ℃\n", temp);
        }
    }
}
