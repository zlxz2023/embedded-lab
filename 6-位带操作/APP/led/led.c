#include "led.h"

void LED_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(LED0_PORT_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LED1_PORT_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=LED0_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED0_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED0_PORT, LED0_PIN);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=LED1_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED1_PORT, LED1_PIN);
}