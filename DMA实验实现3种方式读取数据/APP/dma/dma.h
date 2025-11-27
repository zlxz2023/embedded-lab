#ifndef _dma_H
#define _dma_H

#include "system.h"

void MY_DMA_Init1(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr);
void MY_DMA_Init2(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr);
void MY_DMA_Init3(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr);
void MY_DMA_Enable(DMA_Channel_TypeDef* DMAy_Channelx, u16 ndtr);

#endif
