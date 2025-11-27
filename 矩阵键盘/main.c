#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

// 共阴极0-F
uchar table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void delay_10us(uint z){
    while(z--);
}

// 行列扫描
uint key_scan(){
    uint key_value = 0;
    
    P1=0xf7; //给第一列赋值0，其余全为1
	if(P1!=0xf7)//判断第一列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(P1)//保存第一列按键按下后的键值	
		{
			case 0x77: key_value=1;break;
			case 0xb7: key_value=5;break;
			case 0xd7: key_value=9;break;
			case 0xe7: key_value=13;break;
		}
	}
	while(P1!=0xf7);//等待按键松开	
    
    P1=0xfb;//给第二列赋值0，其余全为1
	if(P1!=0xfb)//判断第二列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(P1)//保存第二列按键按下后的键值	
		{
			case 0x7b: key_value=2;break;
			case 0xbb: key_value=6;break;
			case 0xdb: key_value=10;break;
			case 0xeb: key_value=14;break;
		}
	}
	while(P1!=0xfb);//等待按键松开	
	
	P1=0xfd;//给第三列赋值0，其余全为1
	if(P1!=0xfd)//判断第三列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(P1)//保存第三列按键按下后的键值	
		{
			case 0x7d: key_value=3;break;
			case 0xbd: key_value=7;break;
			case 0xdd: key_value=11;break;
			case 0xed: key_value=15;break;
		}
	}
	while(P1!=0xfd);//等待按键松开	
	
	P1=0xfe;//给第四列赋值0，其余全为1
	if(P1!=0xfe)//判断第四列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(P1)//保存第四列按键按下后的键值	
		{
			case 0x7e: key_value=4;break;
			case 0xbe: key_value=8;break;
			case 0xde: key_value=12;break;
			case 0xee: key_value=16;break;
		}
	}
	while(P1!=0xfe);//等待按键松开
    
    return key_value;
}

// 线翻转法
uint flip_scan(){
    uint key_value = 0;
    
    P1=0x0f;//给所有行赋值0，列全为1
	if(P1!=0x0f)//判断按键是否按下
	{
		delay_10us(1000);//消抖
		if(P1!=0x0f)
		{
			//测试列
			switch(P1)//保存行为0，按键按下后的列值	
			{
				case 0x07: key_value=1;break;
				case 0x0b: key_value=2;break;
				case 0x0d: key_value=3;break;
				case 0x0e: key_value=4;break;
			}
			//测试行
			P1=0xf0;
			switch(P1)//保存列为0，按键按下后的键值	
			{
				case 0x70: key_value=key_value;break;
				case 0xb0: key_value=key_value+4;break;
				case 0xd0: key_value=key_value+8;break;
				case 0xe0: key_value=key_value+12;break;
			}
			while(P1!=0xf0);//等待按键松开	
		}
	}
	return key_value;	
}

void main(){
    uint key;
    while(1){
        key = flip_scan();
        if(key != 0){
            P0 = table[key-1];
        }
    }
}