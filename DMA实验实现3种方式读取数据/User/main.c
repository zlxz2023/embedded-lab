#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "dma.h"

#define LEN1 6
#define LEN2 2
#define LEN3 4

// 内存到内存使用
u8 arrayA[]="0123456789";
u8 arrayB[]="ABCDEFGHIJ";
// 存储从键盘输入的4字节数据
u8 inputData[LEN3];

void M2M(){
    // 开启DMA
    MY_DMA_Init1(DMA1_Channel4, (u32)arrayA, (u32)arrayB, LEN1);
    MY_DMA_Enable(DMA1_Channel4, LEN1);
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

void M2P(){
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    // 开启DMA
    MY_DMA_Init2(DMA1_Channel7, (u32)&USART2->DR, (u32)arrayA, LEN2);
    MY_DMA_Enable(DMA1_Channel7, LEN2);
    // 验证DMA和CPU同时工作
    while(1){
        if(DMA_GetFlagStatus(DMA1_FLAG_TC7)==SET){
            DMA_ClearFlag(DMA1_FLAG_TC7);
            break;
        }
        LED2=!LED2;
        delay_ms(100);
    }
}

void P2M(){
    // 确保 USART 缓冲区无残留数据
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {
        (void)USART_ReceiveData(USART2); // 逐字节读取并丢弃
    }
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    // 开启DMA
    MY_DMA_Init3(DMA1_Channel6, (u32)&USART2->DR, (u32)inputData, LEN3);
    MY_DMA_Enable(DMA1_Channel6, LEN3);
    // 验证DMA和CPU同时工作
    while(1){
        if(DMA_GetFlagStatus(DMA1_FLAG_TC6)==SET){
            DMA_ClearFlag(DMA1_FLAG_TC6);
            break;
        }
        LED2=!LED2;
        delay_ms(100);
    }
}

int main(){
    u8 i=0;
    u8 key=0;
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    KEY_Init();
    USART2_Init(115200);
    
	while(1){
        key=KEY_Scan(0);
        if(key==KEY_UP_PRESS){
            u2_printf("\r\nM2N传输之前:\n");
            u2_printf("\r\arrayA:%s\n",arrayA);
            u2_printf("\r\arrayB:%s\n",arrayB);
            delay_ms(1000);
            M2M();  //数据转运
            u2_printf("\r\nM2M传输之后:\n");
            u2_printf("\r\arrayA:%s\n",arrayA);
            u2_printf("\r\arrayB:%s\n",arrayB);
        }else if(key==KEY0){
            M2P();
            u2_printf("USART2输出\r\n");
        }else if(key==KEY1){
            u2_printf("\r\n请输入4个字节的数据:\n");
            P2M();
            // 打印接收到的后2个字节
            u2_printf("接收缓冲区内容：%c%c\n", inputData[2], inputData[3]);
        }
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
