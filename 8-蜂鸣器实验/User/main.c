#include "led.h"
#include "system.h"
#include "SysTick.h"
#include "beep.h"

int main(){
    SysTick_Init(72);
    LED_Init();
    BEEP_Init();
    while(1){
        BEEP = !BEEP;
    }
}