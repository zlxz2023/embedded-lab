#include "rtc.h"
#include "SysTick.h"

void RTC_Init(){
    u8 temp=0;
    
    // 使能电源时钟和后备域时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//打开电源时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//打开 RTC 后备域时钟
    PWR_BackupAccessCmd(ENABLE);//打开后备寄存器访问
    // 复位备份区域，开启外部低速振荡器
    BKP_DeInit();
    RCC_LSEConfig(RCC_LSE_ON);
    // 检测时钟是否准备就绪
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET && temp<255){
        temp++;
        delay_ms(10);
    }
    // 选择RTC时钟并使能
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
    RTC_WaitForSynchro(); //等待 RTC 寄存器同步
    // 配置秒中断
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
    // 进入配置模式
    RTC_EnterConfigMode();
    RTC_SetPrescaler(32767); // 设置预分频因子
}
