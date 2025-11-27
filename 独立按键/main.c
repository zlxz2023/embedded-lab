#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit K1 = P3^1;
sbit LED1 = P2^0;

// 共阴极0-F
uchar table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void delay_10us(uint z){
    while(z--);
}

uint count = 0;

void main(){
    LED1 = 1;
    P0 = table[count];
    while(1){
        if(K1 == 0){
            while(!K1); // 确保在按键抬起时
            count++;
        }
        if(count % 2 == 0 && count != 0){
            LED1 = 1;
        }else if(count % 2 != 0 && count != 0){
            LED1 = 0;
        }
        P0 = table[count % 16];
    }
}