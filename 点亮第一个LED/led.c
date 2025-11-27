#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit LED1 = P2^0;

void delay(uint z){
    uint x,y;
    for(x = z;x > 0;x--){
        for(y = 144;y > 0;y--);
    }
}
	
void main(){
    P2 = 0xfe;
	while(1){
        /* 简单的点亮与闪烁
		LED1 = 0;
		delay(200);
		LED1 = 1;
		delay(200);*/
        
        // 流水灯
        P2 = _crol_(P2,1);
        delay(500);
	}
}