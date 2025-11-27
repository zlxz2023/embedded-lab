#include "header.h"

uint32_t fre1, capture_val1;
uint32_t fre2, capture_val2;
/*
    函数名: LCD_Show()
    参数：
    功能：在LCD中显示文本
*/
char text[20];
void LCD_Show(){
    sprintf(text, "        test        ");
    LCD_DisplayStringLine(Line0, (uint8_t*)text);
    sprintf(text, "    fre1: %d    ", fre1);
    LCD_DisplayStringLine(Line1, (uint8_t*)text);
    sprintf(text, "    fre2: %d    ", fre2);
    LCD_DisplayStringLine(Line2, (uint8_t*)text);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    // 频率输出2
    if(htim->Instance==TIM16){
        capture_val1=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 捕获到上升沿时，将CNT赋值给CCR
        TIM16->CNT=0;
        fre1=80000000/(80*capture_val1);
    }
    // 频率输出1
    if(htim->Instance==TIM2){
        capture_val2=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 捕获到上升沿时，将CNT赋值给CCR
        TIM2->CNT=0;
        fre2=80000000/(80*capture_val2);
    }
}
