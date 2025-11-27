#include "SysTick.h"

static u8 fac_us=0;
static u16 fac_ms=0;

void SysTick_Init(u8 SYSCLK){
    // 设置时钟源
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us=SYSCLK/8;
    fac_ms=(u16)fac_us*1000;
}

void delay_us(u32 ns){
    u32 temp;
    // 设置重载值
    SysTick->LOAD=ns*fac_us;
    // 设置清除值
    SysTick->VAL=0x00;
    SysTick->CTRL |= 1;
    do{
        temp=SysTick->CTRL;
    }while((temp&0x01) && !(temp&(1<<16)));
    SysTick->CTRL &= ~0x01;
    SysTick->VAL=0x00;
}

void delay_ms(u16 ms){
    u32 temp;
    // 设置重载值
    SysTick->LOAD=ms*fac_ms;
    // 设置清除值
    SysTick->VAL=0x00;
    SysTick->CTRL |= 1;
    do{
        temp=SysTick->CTRL;
    }while((temp&0x01) && !(temp&(1<<16)));
    SysTick->CTRL &= ~0x01;
    SysTick->VAL=0x00;

}