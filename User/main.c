#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "dht11.h"
#include "beep.h"
#include "esp8266.h"
#include "onenet.h"
#include "tftlcd.h"
#include "lsens.h"
#include "HC_SR.h"
#include "step_motor.h"
#include "mq2_fire.h"
#include "dma.h"
#include <math.h>

const char *sub_topic[] = {"subtest"};
const char *pub_topic = "pubtest";
char PUB_BUF[200]; //上传数据
char str[255];
u8 temp;  // 存储温度
u8 humi;  // 存储湿度
u8 lsens_val;  // 存储光敏
float ppm;  // 丁烷浓度
float mq135;  // 空气质量
int curtain=0;  // 存储窗帘
int exist_person=0;  // 存储附近是否有人
int led_flag = 0; // 用来强调以人为操作为主
int exist_p = 1; // 是否启用红外

int main(){
    unsigned short timeCount = 0;	//发送间隔变量
    unsigned char *dataPtr = NULL;
    
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
    DHT11_Init();
    BEEP_Init();
    Lsens_Init();
    MY_ADC_Init();  //mq2+mq135
    MY_DMA_Init3(2);
    BODY_HW_Init();  // 人体红外初始化
    STEP_Init();  // 步进电机
    TFTLCD_Init();
	USART1_Init(115200);
    USART3_Init(115200);
    UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
    ESP8266_Init();
    while(OneNet_DevLink())			//接入服务器
		delay_ms(500);

    // 订阅
    OneNet_Subscribe(sub_topic, 1);
	while(1){
        // 检测是否有人
        exist_person=BODY_HW_GetData();
        if(exist_p==1 && led_flag==0 && exist_person==1){
            LED2=0;
        }else if(exist_p==1 && exist_person==0 && led_flag==0){
            LED2=1;
        }
        if(timeCount % 40 == 0){ // 1s执行一次
            DHT11_Read_Data(&temp, &humi);
            lsens_val = GET_LSENS();
            Get_Value(10);
            mq135=Get_MQ135_PPM(tmp[0]);
            // UsartPrintf(USART_DEBUG, "%.2f\r\n", tmp[0]*3.3/4095);  // 在空气中测定出R0,稳定时为0.53, 0.14
            ppm=Get_PPM(tmp[1]);
            // LCD_Clear(WHITE);
            LCD_ShowFontHZ(10, 80, "温度");
            sprintf(str, ": %d °C", temp);
            LCD_ShowString(85, 80, tftlcd_data.width, tftlcd_data.height, 24, str);
            LCD_ShowFontHZ(10, 120, "湿度");
            sprintf(str, ": %d %", humi);
            LCD_ShowString(80, 120, tftlcd_data.width, tftlcd_data.height, 24, str);
            LCD_ShowFontHZ(10, 160, "丁烷浓度");
            sprintf(str, ": %.2f ppm", ppm);
            LCD_ShowString(150, 160, tftlcd_data.width, tftlcd_data.height, 24, str);
            LCD_ShowFontHZ(10, 200, "是否有人");
            LCD_ShowString(140, 200, tftlcd_data.width, tftlcd_data.height, 24, ":");
            if(exist_person){
                LCD_ShowFontHZ(150, 200, "有人");
            }else{
                LCD_ShowFontHZ(150, 200, "无人");
            }
            LCD_ShowFontHZ(10, 240, "空气质量");
            sprintf(str, ": %.2f ppm", mq135); 
            LCD_ShowString(150, 240, tftlcd_data.width, tftlcd_data.height, 24, str);
            LCD_ShowFontHZ(10, 280, "空气净化器");
            LCD_ShowString(170, 280, tftlcd_data.width, tftlcd_data.height, 24, ":");
            if(mq135>0.2){
                LCD_ShowFontHZ(180, 280, "开启");
            }else{
                LCD_ShowFontHZ(180, 280, "关闭");
            }
        }
        if(++timeCount >= 200){ //发送间隔5s
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF, "{\"temp\": %d, \"humi\": %d, \"led\": %d, \"beep\": %d, \"light\": %d, \"mq2\": %.2f, \"exist_person\": %d, \"mq135\": %.2f}", temp, humi, (int)!LED2, (int)BEEP, lsens_val, ppm, exist_person, mq135);
			OneNet_Publish(pub_topic, PUB_BUF);
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(3);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
        
		delay_ms(10);
    }
}
