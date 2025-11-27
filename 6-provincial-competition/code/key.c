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
uint8_t key_val, key_up, key_down, key_last_val, modify_flag;
uint32_t ckey;
void KEY_Duration(){
    key_val=KEY_Scan();
    key_down=key_val&(key_last_val^key_val);
    key_up=~key_val&(key_last_val^key_val);
    key_last_val=key_val;
    
    if(key_down){
        ckey=0;
    }
    if(ckey<1000){
        if(key_up==B1_PRESS){
            led_flag^=1;
        }
        if(key_up==B2_PRESS){
            LCD_Clear(Black);
            page++;
            page%=2;
            if(page==0){
                RTC_AlarmTypeDef sAlarm = {0};
                sAlarm.AlarmTime.Hours = hours;
                sAlarm.AlarmTime.Minutes = minutes;
                sAlarm.AlarmTime.Seconds = seconds;
                sAlarm.AlarmTime.SubSeconds = 0;
                sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
                sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
                sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
                sAlarm.AlarmDateWeekDay = 1;
                sAlarm.Alarm = RTC_ALARM_A;
                HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
            }
        }
        if(key_up==B3_PRESS && page==1){
            hcount++;
            hcount%=3;
        }
        if(key_up==B4_PRESS && page==1){
            if(hcount==0){
                hours++;
                hours%=24;
            }
            if(hcount==1){
                minutes++;
                minutes%=60;
            }
            if(hcount==2){
                seconds++;
                seconds%=60;
            }
        }
    }else{
        if(key_val==B1_PRESS){
            
        }
        if(key_val==B2_PRESS){
            
        }
        if(key_val==B3_PRESS){
            
        }
        if(key_val==B4_PRESS){
            
        }
    }
}
