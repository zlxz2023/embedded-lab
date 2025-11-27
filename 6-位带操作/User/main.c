#include "led.h"
#include "system.h"
#define uint unsigned int
#define uchar unsigned char 

void delay_ms(uint ms){
    while(ms--);
}


int main(){
    LED_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    while(1){
        LED0=0;
        delay_ms(6000000);
        LED0=1;
        delay_ms(6000000);
    }
}