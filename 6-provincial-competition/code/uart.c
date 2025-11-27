#include "header.h"

uint8_t rec_data, time;
char rec_buff[20];
char send_buff[20];
uint8_t buff_len;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    time=0;
    if(huart->Instance == USART1){
        rec_buff[buff_len++]=rec_data;
        HAL_UART_Transmit(huart, &rec_data, 1, 50);
        HAL_UART_Receive_IT(huart, &rec_data, 1);
	}
}
uint8_t send_flag;
void UART_Proc(){
    if(buff_len==4){
        k=(rec_buff[buff_len-1]-'0')*1.0/10;
        EEPROM_Write(0, 10*k);
    }
    if(send_flag==1){
        RTC_TimeTypeDef sTimes;
        RTC_DateTypeDef sDates;
        HAL_RTC_GetDate(&hrtc, &sDates, RTC_FORMAT_BIN);
        HAL_RTC_GetTime(&hrtc, &sTimes, RTC_FORMAT_BIN);
        sprintf(send_buff, "%.2f+%.1f+%02d%02d%02d\r\n", V1, k, sTimes.Hours, sTimes.Minutes,sTimes.Seconds);
        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
        send_flag=0;
    }
    if(time>50){
        buff_len=0;
        memset(rec_buff, 0, 20);
    }
}
