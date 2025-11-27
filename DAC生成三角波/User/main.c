#include "system.h"
#include "SysTick.h"
#include "dac.h"

int main(){
	SysTick_Init(72);
    DAC_TriangleWave_Init();
	while(1){
        
    }
}
