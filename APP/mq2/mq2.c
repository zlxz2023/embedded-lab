#include "mq2.h"
#include "Systick.h"
#include "math.h"

float GET_ADC_VAL(u8 ch, u8 times){
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
    return (dat/times)*5.0/4096;
}

float Get_PPM(float vol){
    float RS=(5.0-vol)*0.5/vol;
    float R0=6.64;
    return pow(11.5428*R0/RS, 0.6549f);
}
