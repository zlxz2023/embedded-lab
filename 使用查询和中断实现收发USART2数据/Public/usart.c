#include "usart.h"
#include "led.h"

// 中断
void USART2_INTERUPT_Init(u32 baud){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 端口时钟以及串口时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// 配置GPOIA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	 //设置推挽复用模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //设置浮空输入模式
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    
    // USART初始化
    USART_InitStruct.USART_BaudRate=baud;
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_InitStruct.USART_Parity=USART_Parity_No;
    USART_InitStruct.USART_StopBits=USART_StopBits_1;
    USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USART2 ,&USART_InitStruct);
    // 使能串口2
    USART_Cmd(USART2, ENABLE);
    // 清除标志位
    USART_ClearFlag(USART2, USART_FLAG_TC);
    // 串口中断配置
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // NVIC 初始化
    NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

// 查询
void USART2_SEARCH_Init(u32 baud){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    USART_InitTypeDef USART_InitStruct;
    
    // 端口时钟以及串口时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// 配置GPOIA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	 //设置推挽复用模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //设置浮空输入模式
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	   /* 初始化GPIO */
    
    // USART初始化
    USART_InitStruct.USART_BaudRate=baud;
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_InitStruct.USART_Parity=USART_Parity_No;
    USART_InitStruct.USART_StopBits=USART_StopBits_1;
    USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USART2 ,&USART_InitStruct);
    // 使能串口2
    USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(){
    LED2=0;
    u8 dat=0;
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)==SET){
        dat=USART_ReceiveData(USART2);
        USART_SendData(USART2, dat);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
    }
    // 清除标志位
    USART_ClearFlag(USART2, USART_IT_RXNE);
    LED2=1;
}
