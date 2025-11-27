#include <reg52.h>

typedef unsigned int uint;
typedef unsigned char uchar;

sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

// 共阴极0-F
uchar table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
             
void delay_10us(uint x){
    while(x--);
}

void main(){
    while(1){
        /* 静态数码管
        P0 = table[0];*/
        // 动态数码管.
        uint i = 0;
        for(i = 0;i < 8;i++){
            P0 = table[i];
            switch (i) {
                case 0:
                    LSC = 1; LSB = 1; LSA = 1; break;
                case 1:
                    LSC = 1; LSB = 1; LSA = 0; break;
                case 2:
                    LSC = 1; LSB = 0; LSA = 1; break;
                case 3:
                    LSC = 1; LSB = 0; LSA = 0; break;
                case 4:
                    LSC = 0; LSB = 1; LSA = 1; break;
                case 5:   
                    LSC = 0; LSB = 1; LSA = 0; break;
                case 6:   
                    LSC = 0; LSB = 0; LSA = 1; break;
                case 7:   
                    LSC = 0; LSB = 0; LSA = 0; break;
            }
            delay_10us(50000);
        }
    }
}