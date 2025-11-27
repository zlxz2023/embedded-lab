#include "header.h"

void LED_Show(uint8_t led, uint8_t led_mode){
    if(led_mode==1){
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}
uint32_t led_count;
uint8_t led_flag=1;
void LED_Proc(){
    KEY_Duration();
    if(V1>VDD*k && led_flag){
        LED_Show(0x01, 1);
        HAL_Delay(200);
        LED_Show(0x01, 0);
        HAL_Delay(200);
    }
}
