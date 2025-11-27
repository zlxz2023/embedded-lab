#include "header.h"

/*
    函数名：KEY_Scan()
    参数：void
    返回值：key_val-----表示哪一个按键按下，0表示没有按键按下
*/
uint8_t KEY_Scan(){
    uint8_t key_value=0;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET){
        // B1按键
        key_value=B1_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET){
        // B2按键
        key_value=B2_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET){
        // B3按键
        key_value=B3_PRESS;
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET){
        // B4按键
        key_value=B4_PRESS;
    }
    return key_value;
}

/*
    函数名：KEY_Duration()
    参数：void
    返回值：void
    功能：检测长短按
*/
void KEY_Duration(uint8_t key_val, uint8_t key_up, uint8_t key_down, uint32_t* ckey){
    if(key_down){
        *ckey=0;
    }
    // 按键时间小于1s认为是短按
    if(*ckey<1000){
        // 按键B1短按
        if(key_up==B1_PRESS){
            
        }
        // 按键B2短按
        if(key_up==B2_PRESS){
            
        }
        // 按键B3短按
        if(key_up==B3_PRESS){
            
        }
        // 按键B4短按
        if(key_up==B4_PRESS){
            
        }
    }else{
        // 按键B1长按
        if(key_val==B1_PRESS){
            
        }
        // 按键B2长按
        if(key_val==B2_PRESS){
            
        }
        // 按键B3长按
        if(key_val==B3_PRESS){
            
        }
        // 按键B4长按
        if(key_val==B4_PRESS){
            
        }
    }
}

/*
    函数名：KEY_detectButtonClick()
    参数：void
    返回值：void
    功能：检测单双击
*/
// 临时存储上一次的按键
uint8_t key_temp;
void KEY_detectButtonClick(uint8_t key_val, uint8_t key_up, uint8_t key_down, uint32_t* skey, uint8_t* key_flag){
    // 有按键抬起
    if(key_up){
        key_temp=key_up;
        if(*key_flag==0){
            *skey=0;
            *key_flag=1;
        }else{
            // 双击结束后，下一次按键需重新计时
            *key_flag=0;
        }
    }
    if(*key_flag==1){
        if(*skey<300){
            // 按键B1双击
            if(key_down==B1_PRESS && key_temp==B1_PRESS){
                LCD_Clear(Black);
                LCD_SetBackColor(Black);
                page++;
                page %= 3;
            }
            // 按键B2双击
            if(key_down==B2_PRESS && key_temp==B2_PRESS){
                if(page==1){
                    line++;
                    line %= 3;
                }
            }
            // 按键B3双击
            if(key_down==B3_PRESS && key_temp==B3_PRESS){
                if(page==1){
                    if(line==0){
                        count1++;
                    }
                    if(line==1){
                        count2++;
                    }
                    if(line==2){
                        count3++;
                    }
                }
            }
            // 按键B4双击
            if(key_down==B4_PRESS && key_temp==B4_PRESS){
                
            }
        }else{
            // 按键B1单击
            if(key_temp==B1_PRESS){
                
            }
            // 按键B2单击
            if(key_temp==B2_PRESS){
                
            }
            // 按键B3单击
            if(key_temp==B3_PRESS){
                
            }
            // 按键B4单击
            if(key_temp==B4_PRESS){
                
            }
            // 表示单击，标志位置为0，下一次重新计时
            *key_flag=0;
        }
    }
}

/*
    函数名：KEY_Proc()
    参数：void
    返回值：void
*/

// 当前按键
uint8_t key_val;
// 按键抬起
uint8_t key_up;
// 按键按下
uint8_t key_down;
// 上一次按键的值
uint8_t key_last_val;
// 表示按键持续的时间，ckey毫秒
uint32_t ckey;
// 表示检测按键单双击的时间
uint32_t skey;
// 标志位，表示是否需要计时
uint8_t key_flag=0;
void KEY_Proc(){
    key_val=KEY_Scan();
    key_up=~key_val&(key_val^key_last_val);
    key_down=key_val&(key_val^key_last_val);
    key_last_val=key_val;
    // 下面两个只能同时写一个，即要么检测长短按，要么检测单双击
    // 判断长按短按
    //KEY_Duration(key_val, key_up, key_down, &ckey);
    // 判断单击双击
    KEY_detectButtonClick(key_val, key_up, key_down, &skey, &key_flag);
}
