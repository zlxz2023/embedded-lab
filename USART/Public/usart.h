#ifndef _usart_H
#define _usart_H

#include "system.h"

#define USART1_REC_LEN      200
#define USART3_REC_LEN      200

extern u8 USART1_RX_BUF[USART1_REC_LEN];
extern u16 USART1_RX_STATA;

extern u8 USART3_RX_BUF[USART1_REC_LEN];
extern u16 USART3_RX_STATA;

void USART1_Init(u32 baud);
void USART3_Init(u32 baud);
#endif
