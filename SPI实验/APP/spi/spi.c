#include "spi.h"

void MY_SPI_Init(void){
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    /* SPI1 配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB, ENABLE);
    /* 配置SPI1管脚为复用的推挽 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* 配置AT45DB161芯片片选为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* AT45DB161芯片片选置高，即不选中该芯片 */
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
    /* 必须先禁能,才能改变SPI配置 */
    SPI_Cmd(SPI1, DISABLE);
    /* 配置SPI1为双线全双工，主模式，8位数据，软件片选，高bit在前 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    /* 使能 SPI1 */
    SPI_Cmd(SPI1, ENABLE);
}

void AT45DB161_CS_H(void){
    /* AT45DB161片选置高 */
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
}
void AT45DB161_CS_L(void){
    /* AT45DB161片选置低 */
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

u8 SPI_RW_Byte(u8 data){
    /* 判断SPI发送结束 */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
    /* 发送一个字节数据 */
    SPI_I2S_SendData(SPI1, data);
    /* 判断SPI接收结束 */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
    /* 返回SPI接收数据 */
    return SPI_I2S_ReceiveData(SPI1);
}
