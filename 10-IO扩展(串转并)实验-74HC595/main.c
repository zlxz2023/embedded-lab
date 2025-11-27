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

// 8*8点阵
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

// 16*16点阵
void hc595_data1(uchar dat1, uchar dat2, uchar dat3, uchar dat4){
    uint i = 0;
    for(i = 0;i < 8;i++){
        SER = dat4 >> 7;
        dat4 <<= 1;
        SRCLK = 0;
        delay_10us(1);
        SRCLK = 1;
        delay_10us(1);
    }
    for(i = 0;i < 8;i++){
        SER = dat3 >> 7;
        dat3 <<= 1;
        SRCLK = 0;
        delay_10us(1);
        SRCLK = 1;
        delay_10us(1);
    }
    for(i = 0;i < 8;i++){
        SER = dat2 >> 7;
        dat2 <<= 1;
        SRCLK = 0;
        delay_10us(1);
        SRCLK = 1;
        delay_10us(1);
    }
    for(i = 0;i < 8;i++){
        SER = dat1 >> 7;
        dat1 <<= 1;
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
    uint i = 0;
    P0 = 0x00;
    while(1){
        hc595_data(0x00); // 消隐
        hc595_data(sdat);
        sdat = _crol_(sdat, 1);
        delay_10us(50000);
        /* 16*16点阵
        hc595_data(0, 0, 0, 0); // 消隐
        for(i = 0;i < 8;i++){
            hc595_data(sdat, 0, 0, 0);
            sdat = _crol_(sdat, 1);
            delay_10us(50000);
        }
        for(i = 0;i < 8;i++){
            hc595_data(0, sdat, 0, 0);
            sdat = _crol_(sdat, 1);
            delay_10us(50000);
        }
        */
    }
}