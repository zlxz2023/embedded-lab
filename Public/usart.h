#ifndef _usart_H
#define _usart_H

#include "system.h" 
#include "stdio.h" 
#include <stdarg.h>
#include <string.h>

#define USART_DEBUG		USART1		//调试打印所使用的串口组

void USART1_Init(u32 baud);
void USART3_Init(u32 baud);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif


