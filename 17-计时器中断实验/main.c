#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit LED1 = P2^0;

void delay_10us(uint z){
    while(z--);
}

void delay_ms(uint ms){
    uint i, j;
    for(i = ms;i > 0;i--){
        for(j = 110;j > 0;j--);
    }
}

void time0_init(){
    TMOD |= 0x01;
    ET0 = 1;  //打开定时器0中断允许
    EA = 1;  //打开总中断允许
    TR0 = 1;  //打开定时器
    TH0 = 0xFC;
    TL0 = 0x66;
}

void time0() interrupt 1{
    static uint cnt = 0;
    TH0 = 0xFC;
    TL0 = 0x66;
    cnt++;
    if(cnt == 1000){
        cnt = 0;
        LED1 = !LED1;
    }
}

void main(){
    time0_init();
    while(1){
        
    }
}