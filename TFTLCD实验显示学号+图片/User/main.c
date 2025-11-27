#include "system.h"
#include "SysTick.h"    
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"

int main(){
	u8 i=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(115200);
	TFTLCD_Init();			//LCD初始化
	
	FRONT_COLOR=BLACK;
	LCD_ShowFontHZ(10, 80,"周乐");
    LCD_ShowFontHZ(10, 110,"学号");
    LCD_ShowString(70, 110 ,tftlcd_data.width,tftlcd_data.height,24,":");
    LCD_ShowString(85, 110 ,tftlcd_data.width,tftlcd_data.height,24,"202200810274");
	
	LCD_ShowPicture(20,220,100,69,(u8 *)gImage_picture);

	while(1){
		i++;
		if(i%20==0){
			LED1=!LED1;
		}
		
		delay_ms(10);		
	}
}
