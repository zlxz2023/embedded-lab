#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "dma.h"

#define SEND_BUF_LEN    5000
u8 send_buf[SEND_BUF_LEN];

void BUF_Init(u8 *p){
    u16 i=0;
    for(i=0;i<SEND_BUF_LEN;i++){
        *p='5';
        p++;
    }
}

int main(){
    u8 i=0;
    u8 key=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    KEY_Init();
    USART1_Init(115200);
    MY_DMA_Init(DMA1_Channel4, (u32)&USART1->DR, (u32)send_buf, SEND_BUF_LEN);
    BUF_Init(send_buf);
    
	while(1){
        key=KEY_Scan(0);
        if(key==KEY_UP_PRESS){
            // 使能串口1发送功能
            USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
            // 开启DMA
            MY_DMA_Enable(DMA1_Channel4, SEND_BUF_LEN);
            // 验证DMA和CPU同时工作
            while(1){
                if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET){
                    DMA_ClearFlag(DMA1_FLAG_TC4);
                    break;
                }
                LED2=!LED2;
                delay_ms(100);
            }
        }
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
