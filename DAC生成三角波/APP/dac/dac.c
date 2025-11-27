#include "dac.h"
 
//单通道DAC初始化
void DAC_TriangleWave_Init(){
    DAC_InitTypeDef DAC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    /*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);	//开启DAC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/*GPIO初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  //模拟输入模式,ADC和DAC使用该模式
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period=20-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler=2-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=0x00;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;   //这里我选择为定时器2触发
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle; 
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047; //配置指定DAC是否产生,噪声波或三角波,这里我用不到,选择None
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;  //DAC通道输出缓冲,这里我开启,需要提高其驱动能力
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    DAC_SetChannel1Data(DAC_Align_12b_R, 0); //12位右对齐数据格式设置DAC值
    DAC_Cmd(DAC_Channel_1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}