#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"

int main(){
    u8 i=0;
    u8 k=0;
    u16 len=0;
    u16 j=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART1_Init(115200);
    USART3_Init(115200);
    
	while(1){
        /* 所有发出的消息必须加回车换行 */
        // USART1
        // 从串口工具发送信息传送到USART3使得K230显示
        if(USART1_RX_STATA & 0x8000){
            len=(USART1_RX_STATA & 0x3fff);
            for(j=0;j<len;j++){
                USART_SendData(USART3, USART1_RX_BUF[j]);
                while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
            }
            USART1_RX_STATA=0;
        }
		i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
        // USART3
        // 从K230发送信息到USART3，然后转发至USART1显示到串口助手
        if(USART3_RX_STATA & 0x8000){
            len=(USART3_RX_STATA & 0x3fff);
            for(j=0;j<len;j++){
                USART_SendData(USART1, USART3_RX_BUF[j]);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
            }
            USART3_RX_STATA=0;
        }
		k++;
        if(k%20==0){
            LED2=!LED2;
        }
        delay_ms(10);
    }
}
