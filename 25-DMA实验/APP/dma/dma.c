#include "dma.h"

void MY_DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr){
    DMA_InitTypeDef DMA_InitStruct;
    
    // 使能时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 初始化DMA
    DMA_InitStruct.DMA_PeripheralBaseAddr=par;
    DMA_InitStruct.DMA_MemoryBaseAddr=mar;
    DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize=ndtr;
    DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;
    DMA_Init(DMAy_Channelx, &DMA_InitStruct);
}

void MY_DMA_Enable(DMA_Channel_TypeDef* DMAy_Channelx, u16 ndtr){
    // 失能
    DMA_Cmd(DMAy_Channelx, DISABLE);
    // 设置数据量
    DMA_SetCurrDataCounter(DMAy_Channelx, ndtr);
    // 使能
    DMA_Cmd(DMAy_Channelx, ENABLE);
}
