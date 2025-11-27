#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"

#define MAX_DATA_LENGTH 50  // 最大缓冲区大小
#define END_CHAR '\n'  // 结束标志字符

int main(){
    u8 i=0;
    u8 tmp=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
//    // 中断初始化
//    USART2_INTERUPT_Init(115200);
    // 查询初始化
    USART2_SEARCH_Init(115200);
    
	while(1){
        // 查询程序时解除注释
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)!=RESET){
            LED2=0;
            tmp=USART_ReceiveData(USART2);
            USART_SendData(USART2, tmp);
            while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
            LED2=1;
        }
		i++;
        if(i%20==0){
            LED1=!LED1;
        }
    }
}
