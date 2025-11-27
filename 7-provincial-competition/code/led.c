#include "header.h"

void LED_Show(uint8_t led, uint8_t led_mode){
    if(led_mode){
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(GPIOC, led<<8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}

uint8_t rec_data;
char rec_buff[20];
char send_buff[20];
uint8_t buff_len, time;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        rec_buff[buff_len++]=rec_data;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
        time=0;
    }
}

void UART_Proc(){
    if(buff_len==1){
        if(strcmp("C", rec_buff)==0){
            rec_flag=1;
            rec_led=0;
            sprintf(send_buff, "C:H%.0f+L%d\r\n", height, level);
            HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
        }
        if(strcmp("S", rec_buff)==0){
            rec_flag=1;
            rec_led=0;
            sprintf(send_buff, "S:TL%d+TM%d+TH%d\r\n", th1, th2, th3);
            HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
        }
    }
    if(level!=level_temp){
        sprintf(send_buff, "A:H%.0f+L%d+%s\r\n", height, level, level<level_temp?"D":"U");
        HAL_UART_Transmit(&huart1, (uint8_t*)send_buff, sizeof(send_buff), 50);
    }
    if(time>50){
        buff_len=0;
        memset(rec_buff, 0, 20);
    }
}
