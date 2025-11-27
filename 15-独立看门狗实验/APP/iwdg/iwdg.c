#include "iwdg.h"

void IWDG_Init(u8 pre,u16 rlr){
    // 向IWDG_KR 写入0x5555
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    // 设置预分频系数和重载值
    IWDG_SetPrescaler(pre);
    IWDG_SetReload(rlr);
    
    // 写入0xAAAA 重装重载值
    IWDG_ReloadCounter();
    
    // 写入0xCCCC 开启独立看门狗
    IWDG_Enable();
}
