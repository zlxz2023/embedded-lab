#include "led.h"
#include "system.h"
#include "SysTick.h"

int main(){
    SysTick_Init(72);
    LED_Init();
    while(1){
        LED0=0;
        delay_ms(10);
        LED0=1;
        delay_ms(10);
    }
}