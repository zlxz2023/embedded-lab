#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

void delay_10us(uint z){
    while(z--);
}

void delay_ms(uint ms){
    uint i, j;
    for(i = ms;i > 0;i--){
        for(j = 110;j > 0;j--);
    }
}

void uart_init(uchar baud){
    TMOD |= 0x20;
    ET0 = 1;  //打开定时器0中断允许
    ES = 1;  //打开串口
    EA = 1;  //打开总中断允许
    SCON = 0x50;  //设置工作方式1
    PCON = 0x80;  //波特率加倍
    TH1 = baud;
    TL1 = baud;
    TR1 = 1;  //打开定时器
}

void uart() interrupt 4{
    uchar dat;
    RI = 0;  //清零
    dat = SBUF;
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void main(){
    uart_init(0xFA);
    while(1){
        
    }
}