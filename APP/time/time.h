#ifndef _time_H
#define _time_H

#include "system.h"
#include "stdio.h"

extern u8 send_data_flag;
extern char buf[200];
extern u8 temp;  	    
extern u8 humi;
extern char mqtt_cmd[100];

void TIM4_Init(u16 per, u16 psc);

#endif
