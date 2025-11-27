#include "header.h"

char rec_buff[20];
char send_buff[20];
uint8_t buff_len;
uint8_t rec_data;
/*
    该函数为中断接收后进入的函数，需要重写，可以放置接收后进行的操作
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    time=0;
    if(huart->Instance == USART1){
        rec_buff[buff_len++]=rec_data;
        HAL_UART_Receive_IT(huart, &rec_data, 1);
	}
}

/*
    函数名：UART_Proc()
    参数：len------处理的定长字符长度
    返回值：void
    功能：每当收到数据长度和要处理的长度相同，就开始进行处理
*/
// 用来实现固定时间清除缓存
uint8_t time;
void UART_Proc(uint8_t len){
    if(buff_len==len){
        if(strcmp("LEDO", rec_buff)==0){
            LED_Show(0x01, 1);
        }
        if(strcmp("LEDC", rec_buff)==0){
            LED_Show(0x01, 0);
        }
        buff_len=0;
        memset(rec_buff, 0, 20);
    }
    // 下面这一段是及时清除缓存，避免错误指令的发送导致问题
    if(time>50){
        time=0;
        buff_len=0;
        memset(rec_buff, 0, 20);
    }
}
