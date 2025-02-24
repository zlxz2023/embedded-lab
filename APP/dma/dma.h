#ifndef _dma_H
#define _dma_H

#include "system.h"

extern uint16_t tmp[2];  // 作为读取两个通道值的缓存

void MY_DMA_Init1(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr);
void MY_DMA_Init2(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr);
void MY_DMA_Init3(u16 ndtr);
void MY_DMA_Enable(DMA_Channel_TypeDef* DMAy_Channelx, u16 ndtr);
void P2M(uint16_t *tmp);
void Get_Value(u16 times);

#endif
