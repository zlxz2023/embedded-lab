#include "header.h"

int count=0;
uint8_t highlight_display;

/*
    函数名：LED_Show()
    参数：mode------1为点亮，0为熄灭
          num ------选择点亮第几个灯，1-8
    功能：点亮一个LED
*/
void LED_Show(uint8_t mode, uint8_t num){
    // PD2=1，使能锁存器
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    if(mode==1){
        // 点亮LED
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (num-1), GPIO_PIN_RESET);
    }else{
        // 熄灭LED
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << (num-1), GPIO_PIN_SET);
    }
    // PD2=0，失能锁存器
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

/*
    函数名: KEY_Scan()
    参数：
    功能：检测按键是否按下
*/
uint8_t B1_State;
uint8_t B1_Last_State=1;
uint8_t B2_State;
uint8_t B2_Last_State=1;
uint8_t B3_State;
uint8_t B3_Last_State=1;
uint8_t B4_State;
uint8_t B4_Last_State=1;
void KEY_Scan(){
    B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
    B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if(B1_State==0 && B1_Last_State==1){
        // 按键B1按下
        TIM3->CNT=0;
    }else if(B1_State==0 && B1_Last_State==0){
        // 1s长按
        if(TIM3->CNT>=10000){
            count++;
        }
    }else if(B1_State==1 && B1_Last_State==0){
        // 松开按键
        if(TIM3->CNT<10000){ // 短按按键
            count += 2;
        }
    }
    if(B2_State==0 && B2_Last_State==1){
        // 按键B2按下
        TIM3->CNT=0;
    }else if(B2_State==0 && B2_Last_State==0){
        // 1s长按
        if(TIM3->CNT>=10000){
            count--;
        }
    }else if(B2_State==1 && B2_Last_State==0){
        // 松开按键
        if(TIM3->CNT<10000){ // 短按按键
            count -= 2;
        }
    }
    if(B3_State==0 && B3_Last_State==1){
        // 按键B3按下
        highlight_display++;
        highlight_display %= 2;
        LED_Show(1,2);
    }
    if(B4_State==0 && B4_Last_State==1){
        // 按键B4按下
        LED_Show(0,2);
    }
    B1_Last_State=B1_State;
    B2_Last_State=B2_State;
    B3_Last_State=B3_State;
    B4_Last_State=B4_State;
}

/*
    函数名: LCD_Show()
    参数：
    功能：在LCD中显示文本
*/
char text[20];
void LCD_Show(){
    // 使某一行高亮显示
    if(highlight_display==0){
        LCD_SetBackColor(Yellow);
        // 将字符串拷贝到数组中
        sprintf(text, "count: %d", count);
        // 显示文本
        LCD_DisplayStringLine(Line0, (uint8_t*)text);
        LCD_SetBackColor(Black);
        sprintf(text, "    test    ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
    }else if(highlight_display==1){
        // 将字符串拷贝到数组中
        sprintf(text, "count: %d", count);
        // 显示文本
        LCD_DisplayStringLine(Line0, (uint8_t*)text);
        LCD_SetBackColor(Yellow);
        sprintf(text, "    test    ");
        LCD_DisplayStringLine(Line1, (uint8_t*)text);
        LCD_SetBackColor(Black);
    }
    LED_Show(count%2, 1);
}

/*
    函数名: HAL_TIM_PeriodElapsedHalfCpltCallback()
    参数：htim-----TIMx
    功能：定时器中断服务函数
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM2){
        count++;
    }
}
