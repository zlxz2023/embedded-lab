#include "header.h"

uint32_t fre, capture_val;

/*
    函数名: LCD_Show()
    参数：
    功能：在LCD中显示文本
*/
char text[20];
void LCD_Show(){
    sprintf(text, "        test        ");
    LCD_DisplayStringLine(Line0, (uint8_t*)text);
    sprintf(text, "    fre: %d    ", fre);
    LCD_DisplayStringLine(Line1, (uint8_t*)text);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM17){
        capture_val=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // 捕获到上升沿时，将CNT赋值给CCR
        TIM17->CNT=0;
        fre=80000000/(80*capture_val);
    }
}
