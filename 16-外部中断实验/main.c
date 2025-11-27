#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit KEY3 = P3^2;
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

void int0_init(){
    EA = 1;
    EX0 = 1;
    IT0 = 1;
}

void int0() interrupt 0{
    delay_ms(5);
    if(KEY3 == 0){
        LED1 = !LED1;
    }
    while(!KEY3);
}

void main(){
    int0_init();
    while(1){
    
    }
}