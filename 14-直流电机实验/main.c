#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit OUT1 = P1^0;

void delay_10us(uint z){
    while(z--);
}

void delay_ms(uint ms){
    uint i, j;
    for(i = ms;i > 0;i--){
        for(j = 110;j > 0;j--);
    }
}

void main(){
    OUT1 = 1;
    delay_ms(5000);
    OUT1 = 0;
    while(1){
       
    }
}