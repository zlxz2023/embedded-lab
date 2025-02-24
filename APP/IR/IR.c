#include "IR.h"
#include "SysTick.h"
#include "adc.h"

uint16_t IR_FireData(void){
    uint32_t  tempData = 0;
	//设置指定ADC的规则组通道，采样时间
	tempData=GET_IR_VAL(ADC_CHANNEL, IR_READ_TIMES);
    return 4095 - (uint16_t)tempData;
}
