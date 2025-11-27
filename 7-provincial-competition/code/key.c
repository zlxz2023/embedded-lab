#include "header.h"

uint8_t KEY_Scan(){
    uint8_t key_value;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_RESET){
        key_value=B1_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==GPIO_PIN_RESET){
        key_value=B2_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)==GPIO_PIN_RESET){
        key_value=B3_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_RESET){
        key_value=B4_PRESS;
    }
    return key_value;
}
uint8_t B1_State, B1_Last_State;
uint8_t B2_State, B2_Last_State;
uint8_t B3_State, B3_Last_State;
uint8_t B4_State, B4_Last_State;
void KEY_Detection(){
    B1_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    B2_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    B3_State=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
    B4_State=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    if(B1_State==0 && B1_Last_State==1){
        page++;
        page%=2;
        if(page==0){
            EEPROM_Write(0, th1);
            EEPROM_Write(1, th2);
            EEPROM_Write(2, th3);
        }
    }
    if(B2_State==0 && B2_Last_State==1){
        line++;
        line%=3;
    }
    if(B3_State==0 && B3_Last_State==1){
        if(line==0){
            if(th1+5>95){
                th1=95;
            }else{
                th1+=5;
            }
        }
        if(line==1){
            if(th2+5>95){
                th2=95;
            }else{
                th2+=5;
            }
        }
        if(line==2){
            if(th3+5>95){
                th3=95;
            }else{
                th3+=5;
            }
        }
    }
    if(B4_State==0 && B4_Last_State==1){
        if(line==0){
            if(th1-5<5){
                th1=5;
            }else{
                th1-=5;
            }
        }
        if(line==1){
            if(th2-5<5){
                th2=5;
            }else{
                th2-=5;
            }
        }
        if(line==2){
            if(th3-5<5){
                th3=5;
            }else{
                th3-=5;
            }
        }
    }
    B1_Last_State=B1_State;
    B2_Last_State=B2_State;
    B3_Last_State=B3_State;
    B4_Last_State=B4_State;
}
