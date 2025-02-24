#include "dma.h"

uint16_t tmp[2];

// 内存到内存
void MY_DMA_Init1(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr){
    DMA_InitTypeDef DMA_InitStruct;
    
    // 使能时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 初始化DMA
    DMA_InitStruct.DMA_PeripheralBaseAddr=par;
    DMA_InitStruct.DMA_MemoryBaseAddr=mar;
    DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize=ndtr;
    DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
    DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;
    DMA_Init(DMAy_Channelx, &DMA_InitStruct);
}

// 内存到外设
void MY_DMA_Init2(DMA_Channel_TypeDef* DMAy_Channelx, u32 par, u32 mar, u16 ndtr){
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

// 从外设到存储器
void MY_DMA_Init3(u16 ndtr){
    DMA_InitTypeDef DMA_InitStruct;
    
    // 使能时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 初始化DMA
    DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)tmp;
    DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize=ndtr;
    DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
    DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);
}

void Get_Value(u16 times){
    u16 sum0 = 0,sum1 = 0;
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待DMA传输完成
    for (u8 i = 0; i < times; i++) {
        // 失能DMA
        DMA_Cmd(DMA1_Channel1, DISABLE);
        // 设置DMA传输计数器为2，因为我们每次读取两个通道的数据
        DMA_SetCurrDataCounter(DMA1_Channel1, 2);
        // 使能DMA
        DMA_Cmd(DMA1_Channel1, ENABLE);
        
        // 启动ADC转换
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        
        // 等待DMA传输完成
        while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);  // 查看标志位
        DMA_ClearFlag(DMA1_FLAG_TC1);  // 清除标志位
        
        // 累加ADC值
        sum0 += tmp[0];
        sum1 += tmp[1];
    }
    
    // 计算平均值
    tmp[0] = sum0 / times;
    tmp[1] = sum1 / times;
}

void MY_DMA_Enable(DMA_Channel_TypeDef* DMAy_Channelx, u16 ndtr){
    // 失能
    DMA_Cmd(DMAy_Channelx, DISABLE);
    // 设置数据量
    DMA_SetCurrDataCounter(DMAy_Channelx, ndtr);
    // 使能
    DMA_Cmd(DMAy_Channelx, ENABLE);
}
