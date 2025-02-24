#include "lsens.h"
#include "SysTick.h"

void Lsens_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    ADC_InitTypeDef ADC_InitStruct;
    
    // 使能GPIO以及ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOF, ENABLE);
    // 初始化GPIO
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 	   /* 初始化GPIO */
    // 初始化ADC
    ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
    ADC_InitStruct.ADC_ScanConvMode=DISABLE;
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_NbrOfChannel=1;
    ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
    ADC_Init(ADC3, &ADC_InitStruct);
    // 使能ADC并校准
    ADC_Cmd(ADC3, ENABLE);
    ADC_ResetCalibration(ADC3); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC3));
    ADC_StartCalibration(ADC3); // 开始校准
    while(ADC_GetCalibrationStatus(ADC3));
    // 开启转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16 GET_ADC_LIGHT_VAL(u8 ch, u8 times){
    u8 i=0;
    u32 dat=0;
    
    // 规则通道配置
    ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5);
    
    // 读取数据
    for(i = 0;i < times;i++){
        // 开启转换
        ADC_SoftwareStartConvCmd(ADC3, ENABLE);
        while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
        dat += ADC_GetConversionValue(ADC3);
        delay_ms(5);
    }
    return dat/times;
}

u8 GET_LSENS(){
    u16 dat;
    dat = GET_ADC_LIGHT_VAL(ADC_Channel_6, 10);
    if(dat > 4000){
        dat=4000;
    }
    return (100-dat/40);
}
