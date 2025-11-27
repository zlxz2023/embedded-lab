#include "usart.h"
#include "stdio.h"

u8 USART1_RX_BUF[USART1_REC_LEN]; // 存放读取的数据
u16 USART1_RX_STATA=0; // 15-接受完成标志     14-是否接收0x0D     0~13-接收到的数据字节数

int fputc(int ch, FILE *p){
    USART_SendData(USART1, ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    return ch;
}

void USART1_Init(u32 baud){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 端口时钟以及串口时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	// 配置GPOIA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	 //设置推挽复用模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //设置浮空输入模式
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    
    // USART初始化
    USART_InitStruct.USART_BaudRate=baud;
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_InitStruct.USART_Parity=USART_Parity_No;
    USART_InitStruct.USART_StopBits=USART_StopBits_1;
    USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USART1 ,&USART_InitStruct);
    
    // 串口中断配置
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // NVIC 初始化
    NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    // 使能串口1
    USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(){
    u8 dat=0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET){
        /*
        为了防止数据还没有被接受完全就发送，这里制定一个简易的规则，enter表示结束
        enter ------- 0x0D，0x0A
        */
        dat=USART_ReceiveData(USART1);
        if((USART1_RX_STATA & 0x8000)==0){
            if(USART1_RX_STATA & 0x4000){
                if(dat != 0x0A){
                    USART1_RX_STATA=0;
                }else{
                    USART1_RX_STATA |= 0x8000;
                }
            }else{
                if(dat==0x0D)
                    USART1_RX_STATA |= 0x4000;
                else{
                    USART1_RX_BUF[USART1_RX_STATA & 0x3fff]=dat;
                    USART1_RX_STATA++;
                    if(USART1_RX_STATA > USART1_REC_LEN-1){
                        USART1_RX_STATA=0;
                    }
                }
            }
        }
    }
}
