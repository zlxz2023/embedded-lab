#include "mq2_fire.h"
#include "Systick.h"
#include "math.h"
#include "usart.h"

// 初始化ADC1的两个通道
void MY_ADC_Init(){
    ADC_InitTypeDef ADC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIO以及ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // GPIO初始化，配置两个通道对应的引脚为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // 假设通道0和1分别对应PA0和PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置通道0和1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    
    // 设置分频系数
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    // 初始化ADC
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE; // 启用扫描模式，可以连续读取多个通道
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_NbrOfChannel = 2; // 设置通道数量为2
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; // 单次转换模式
    ADC_Init(ADC1, &ADC_InitStruct);
    ADC_DMACmd(ADC1, ENABLE);
    // 使能ADC并校准
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1); // 开始校准
    while(ADC_GetCalibrationStatus(ADC1));
}

float Get_MQ135_PPM(uint16_t mq135){
    float vol=mq135*3.3/4095; // 实际电压
    float RS=(5.0/vol-1)*1.0;
    float R0=3.82; // 标准环境中为0.049ppm
    // return pow(11.5428*R0/RS, 0.6549f);
    return 49.4353f * pow(RS/R0, -3.1125);
}

float Get_PPM(uint16_t mq2){
    float vol=mq2*3.3/4095; // 实际电压
    float RS=(5.0/vol-1)*1.0;
    float R0=1.61; // 标准环境中为20ppm
    // return pow(11.5428*R0/RS, 0.6549f);
    return 619.17f * pow(RS/R0, -2.064f); // 丁烷-----火机内的液体
}
