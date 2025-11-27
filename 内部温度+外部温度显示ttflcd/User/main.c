#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc_temp.h"
#include "dht11.h"
#include "tftlcd.h"
#include "exti.h"
#include "key.h"

char str[255];

int main(){
    u8 i=0;
    float temp1=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    KEY_Init();
    USART1_Init(115200);
    ADC_TEMP_Init();
    DHT11_Init();
    TFTLCD_Init();
    MY_EXTI_Init();
    
    FRONT_COLOR=BLACK;
    
	while(1){
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
        
        if(i%1000==0){ // 1000*10=10s
            temp1=GET_TEMPERTURE();
            if(temp1<0){
                printf("内部温度检测值为 -");
            }else{
                printf("内部温度检测值为 +");
            }
            sprintf(str, "%.2f", temp1);
            LCD_ShowFontHZ(10, 80,"内部温度");
            LCD_ShowString(130, 80 ,tftlcd_data.width,tftlcd_data.height,24,":");
            LCD_ShowString(150, 80 ,tftlcd_data.width,tftlcd_data.height,24,str);
        }
    }
}
