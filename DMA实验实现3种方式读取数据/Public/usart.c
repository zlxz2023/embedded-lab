#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

u8 USART1_RX_BUF[USART1_REC_LEN]; // 存放读取的数据
u16 USART1_RX_STATA=0; // 15-接受完成标志     14-是否接收0x0D     0~13-接收到的数据字节数

u8 USART2_RX_BUF[USART2_REC_LEN]; // 存放读取的数据
u16 USART2_RX_STATA=0; // 15-接受完成标志     14-是否接收0x0D     0~13-接收到的数据字节数

// 重定向USART1
int fputc(int ch, FILE *p){
    USART_SendData(USART1, ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    return ch;
}

// 重定向USART2
char UART2_TX_BUF[200];
void u2_printf(char* fmt, ...)    //无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
{
    u16 i, j;
    va_list ap;          //va_list 是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
    va_start(ap, fmt);   //va_start函数来获取参数列表中的参数，使用完毕后调用va_end()结束
    vsprintf((char*)UART2_TX_BUF, fmt, ap);	// 把生成的格式化的字符串存放在这里
    va_end(ap);
    i = strlen((const char*)UART2_TX_BUF);              //此次发送数据的长度
    for(j = 0; j < i; j++)                                                    //循环发送数据
    {
        while((USART2->SR & 0X40) == 0);                    //循环发送,直到发送完毕
        USART2->DR = UART2_TX_BUF[j];
    }
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

void USART2_Init(u32 baud){
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

void USART2_IRQHandler(){
    u8 dat=0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET){
        /*
        为了防止数据还没有被接受完全就发送，这里制定一个简易的规则，enter表示结束
        enter ------- 0x0D，0x0A
        */
        dat=USART_ReceiveData(USART2);
        if((USART2_RX_STATA & 0x8000)==0){
            if(USART2_RX_STATA & 0x4000){
                if(dat != 0x0A){
                    USART2_RX_STATA=0;
                }else{
                    USART2_RX_STATA |= 0x8000;
                }
            }else{
                if(dat==0x0D)
                    USART2_RX_STATA |= 0x4000;
                else{
                    USART2_RX_BUF[USART2_RX_STATA & 0x3fff]=dat;
                    USART2_RX_STATA++;
                    if(USART2_RX_STATA > USART2_REC_LEN-1){
                        USART2_RX_STATA=0;
                    }
                }
            }
        }
    }
}