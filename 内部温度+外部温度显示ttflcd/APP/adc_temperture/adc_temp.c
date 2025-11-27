#include "adc_temp.h"
#include "SysTick.h"

void ADC_TEMP_Init(){
	ADC_InitTypeDef ADC_InitStruct;
    
    // 使能GPIO以及ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    // 设置分频系数
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    // 开启内部温度传感器
    ADC_TempSensorVrefintCmd(ENABLE);
    // 初始化ADC
    ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode=DISABLE;
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_NbrOfChannel=1;
    ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
    ADC_Init(ADC1, &ADC_InitStruct);
    // 使能ADC并校准
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1); // 开始校准
    while(ADC_GetCalibrationStatus(ADC1));
    // 开启转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 GET_ADC_TEMP_VAL(u8 ch, u8 times){
    u8 i=0;
    u32 dat=0;
    
    // 规则通道配置
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    
    // 读取数据
    for(i = 0;i < times;i++){
        // 开启转换
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        dat += ADC_GetConversionValue(ADC1);
        delay_ms(5);
    }
    return dat/times;
}

float GET_TEMPERTURE(){
    u16 dat;
    float temperture;
    float vol=0;
    dat = GET_ADC_TEMP_VAL(ADC_Channel_16, 10);
    vol = (float)dat*(3.3/4096);
    temperture=(1.43-vol)/0.0043+25;
    return temperture;
}
