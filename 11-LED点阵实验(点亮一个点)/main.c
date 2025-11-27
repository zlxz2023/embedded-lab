#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit SER = P3^4;
sbit SCLK = P3^5; // 上升沿触发，将寄存器中的值送到端口
sbit SRCLK = P3^6; // 上升沿触发，将数据送到寄存器
uchar sdat = 0x01;
                    
void delay_10us(uint z){
    while(z--);
}

void delay_ms(uint ms){
    uint i, j;
    for(i = ms;i > 0;i--){
        for(j = 110;j > 0;j--);
    }
}

void hc595_data(uchar dat){
    uint i = 0;
    for(i = 0;i < 8;i++){
        SER = dat >> 7;
        dat <<= 1;
        SRCLK = 0;
        delay_10us(1);
        SRCLK = 1;
        delay_10us(1);
    }
    SCLK = 0;
    delay_10us(1);
    SCLK = 1;
}

void main(){
    P0 = 0x7f;
    while(1){
        hc595_data(0x00); // 消隐
        hc595_data(0x80);
    }
}