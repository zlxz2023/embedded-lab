#include "header.h"

/*
    函数名: LCD_Show()
    参数：
    功能：在LCD中显示文本
*/
char text[20];
void LCD_Show(){
    sprintf(text, "        test        ");
    LCD_DisplayStringLine(Line0, (uint8_t*)text);
}
