#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "dac.h"

int main(){
    u8 i=0;
    u8 key=0;
    u16 dat=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    KEY_Init();
    USART1_Init(115200);
    DAC1_Init();
    
	while(1){
        key=KEY_Scan(1);
        if(key==KEY_UP_PRESS){
            dat+=100;
            if(dat>=4000){
                dat=4095;
            }
            DAC_SetChannel1Data(DAC_Align_12b_R, dat);
        }else if(key==KEY1_PRESS){
            dat-=100;
            if(dat<0){
                dat=0;
            }
            DAC_SetChannel1Data(DAC_Align_12b_R, dat);
        }
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
        
        if(i%100==0){
            printf("实际输出的DAC电压: %.2f V\n", 3.3*dat/4095);
            printf("理论输出的DAC电压: %.2f V\n", 3.3*DAC_GetDataOutputValue(DAC_Channel_1)/4095);
        }
    }
}
