#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "spi.h"

int main(){
    u8 i=0;
    u8 id[128];
    
	SysTick_Init(72);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
    USART2_Init(115200);
    MY_SPI_Init();
    /* 片选置低以选中芯片 */
    AT45DB161_CS_L();
    /* 发送读取ID号命令0x9f */
    SPI_RW_Byte(0x9f);
    SPI_RW_Byte(0xaa);
    SPI_RW_Byte(0xaa);
    SPI_RW_Byte(0xaa);
    for(i=0;i<128;i++){
        /* 发送无关字符0xaa维持时钟，并将接收数据缓存 */
        id[i]=SPI_RW_Byte(0xaa);
    }
    /* 结束读取后，片选置高 */
    AT45DB161_CS_H();
    
    for(int i = 64;i < 128;i++){
        u2_printf("%d", id[i]);
    }
	while(1){
        i++;
        if(i%20==0){
            LED1=!LED1;
        }
        delay_ms(10);
    }
}
