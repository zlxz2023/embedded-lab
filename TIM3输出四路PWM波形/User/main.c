#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "pwm.h"

int main(void){
	PWM1(7199,999,1000);
	PWM2(7199,999,2000);
	PWM3(7199,999,3000);
	PWM4(7199, 999, 3600);
    while(1){
        
    }
}