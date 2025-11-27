#include "dac.h"

void DAC1_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    DAC_InitTypeDef DAC_InitStructure;
    
    // 端口时钟以及DAC时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	// 初始化GPIO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   /* 初始化GPIO */
    // 初始化DAC
    DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
    DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    // 设置默认数据
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);
    // 使能DAC通道
    DAC_Cmd(DAC_Channel_1, ENABLE);
}
