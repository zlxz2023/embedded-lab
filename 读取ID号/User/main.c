#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"


// 从固定地址读取 UID
void Read_UID(u8 *uid) {
    uid[2] = *(__IO u32 *)(0x1FFFF7E8);
    uid[1] = *(__IO u32 *)(0x1FFFF7EC);
    uid[0] = *(__IO u32 *)(0x1FFFF7F0);
}

int main(){
    u8 i=0;
    u8 uid[3];
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    
    Read_UID(uid);
    
	while(1){
        printf("STM的ID号:%X-%X-%X\r\n",uid[0],uid[1],uid[2]);
        delay_ms(100);
		i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
