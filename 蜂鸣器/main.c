#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

sbit BEEP = P2^5;

void delay(uint z){
    uint x,y;
    for(x = z;x > 0;x--){
        for(y = 144;y > 0;y--);
    }
}
	
void main(){
    
	while(1){
        BEEP = 1;
        delay(500);
        BEEP = ~BEEP;
        delay(500);
	}
}