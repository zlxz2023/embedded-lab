#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit SER = P3^4;
sbit SCLK = P3^5; // 上升沿触发，将寄存器中的值送到端口
sbit SRCLK = P3^6; // 上升沿触发，将数据送到寄存器

uint gled_row[] = {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00};

void delay_10us(uint z){
    while(z--);
}

void delay_ms(uint ms){
    uint i, j;
    for(i = ms;i > 0;i--){
        for(j = 110;j > 0;j--);
    }
}

//8*8点阵
void hc595_data(uchar dat){
    uint i = 0;
    for(i = 0;i < 8;i++){
        SER = dat >> 7;
        dat <<= 1;
        SRCLK = 0;
        _nop_();
        SRCLK = 1;
        _nop_();
    }
    SCLK = 0;
    _nop_();
    SCLK = 1;
}

// 16*16点阵
void hc595_data1(uchar dat1, uchar dat2, uchar dat3, uchar dat4){
    uint i = 0;
    for(i = 0;i < 8;i++){
        SER = dat4 >> 7;
        dat4 <<= 1;
        SRCLK = 0;
        _nop_();
        SRCLK = 1;
        _nop_();
    }
    for(i = 0;i < 8;i++){
        SER = dat3 >> 7;
        dat3 <<= 1;
        SRCLK = 0;
        _nop_();
        SRCLK = 1;
        _nop_();
    }
    for(i = 0;i < 8;i++){
        SER = dat2 >> 7;
        dat2 <<= 1;
        SRCLK = 0;
        _nop_();
        SRCLK = 1;
        _nop_();
    }
    for(i = 0;i < 8;i++){
        SER = dat1 >> 7;
        dat1 <<= 1;
        SRCLK = 0;
        _nop_();
        SRCLK = 1;
        _nop_();
    }
    SCLK = 0;
    _nop_();
    SCLK = 1;
}

void main(){
    while(1){
        uint i = 0;
        P0 = 0x7f;
        for(i = 0;i < 8;i++){
            if(i != 0)
                P0 = _cror_(P0, 1);
            hc595_data(gled_row[i]);
            delay_ms(1);
            hc595_data(0); // 消隐
        }
    }
}