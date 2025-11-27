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
    sprintf(text, "    R37: %.2f    ", GET_ADC_VOL(&hadc2));
    LCD_DisplayStringLine(Line1, (uint8_t*)text);
    sprintf(text, "    R38: %.2f    ", GET_ADC_VOL(&hadc1));
    LCD_DisplayStringLine(Line2, (uint8_t*)text);
}

double GET_ADC_VOL(ADC_HandleTypeDef *hadc){
    HAL_ADC_Start(hadc);
    uint32_t vol = HAL_ADC_GetValue(hadc);
    return 3.3*vol/4096;
}
