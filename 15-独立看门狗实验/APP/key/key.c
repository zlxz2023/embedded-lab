#include "key.h"
#include "SysTick.h"

void KEY_Init(){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // GPIO时钟配置
    RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(KEY_UP_CLK, ENABLE);
    
    // GPIO端口配置
    GPIO_InitStructure.GPIO_Pin=KEY0_PIN | KEY1_PIN | KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=KEY_UP_PIN;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; // 下拉输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY_UP_PORT, &GPIO_InitStructure);
    
}

// mode==0 单次检测
// mode==1 连续检测
u8 KEY_Scan(u8 mode){
    static u8 key = 1; // 静态全局变量
    if(mode == 1){
        key = 1;
    }
    if(key==1 && (KEY_UP==1 || KEY0==0 || KEY1==0 || KEY2==0)){
        key = 0;
        delay_ms(10); // 消抖
        if(KEY_UP==1){
            return KEY_UP_PRESS;
        }else if(KEY0 == 0){
            return KEY0_PRESS;
        }else if(KEY1 == 0){
            return KEY1_PRESS;
        }else if(KEY2 == 0){
            return KEY2_PRESS;
        }
    }else if(key==0 && (KEY_UP==0 && KEY0==1 && KEY1==1 && KEY2==1)){
        key = 1;
    }
    return 0;
}
