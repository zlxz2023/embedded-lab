#include "header.h"

/*
    函数名: LED_Show()
    参数：
    功能：点亮LED
*/
void LED_Show(uint8_t mode, uint8_t num){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    if(mode){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (num-1), GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (num-1), GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/*
    函数名: LCD_Show()
    参数：
    功能：在LCD中显示文本
*/
char text[20];
uint8_t count;
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};

void LCD_Show(){
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    sprintf(text, "        test        ");
    LCD_DisplayStringLine(Line0, (uint8_t*)text);
    sprintf(text, "    %2d:%2d:%2d    ", sTime.Hours, sTime.Minutes, sTime.Seconds);
    LCD_DisplayStringLine(Line1, (uint8_t*)text);
    sprintf(text, "   %d-%d-%d-%d   ", sDate.Year, sDate.Month, sDate.Date, sDate.WeekDay);
    LCD_DisplayStringLine(Line2, (uint8_t*)text);
    LED_Show(count, 1);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
    count=1;
}
