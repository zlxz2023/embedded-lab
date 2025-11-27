#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main(){
	u8 key_val;
	u8 i=0;
    
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
    KEY_Init();
	USART1_Init(115200);

	
	delay_ms(1800);
	USB_Port_Set(0); 	//USB先断开
	delay_ms(700);
	USB_Port_Set(1);	//USB再次连接
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	    

	while(1){
		key_val=KEY_Scan(1);//支持连按
		if(key_val){
                Joystick_Send(key_val); 		//发送数据到电脑    
		}			    	 
		if(bDeviceState==CONFIGURED) LED1=0;//当USB配置成功了，LED1亮，否则，灭
		else LED1=1;    
		i++;
		if(i%200 == 0){
			LED2=!LED2;
		}
        delay_ms(10);
	}
}
