#ifndef _key_H
#define _key_H
#include "stdint.h"

#define B1_PRESS 1
#define B2_PRESS 2
#define B3_PRESS 3
#define B4_PRESS 4

extern uint32_t ckey;
extern uint32_t skey;

void KEY_Proc(void);
uint8_t KEY_Scan(void);
void KEY_Duration(uint8_t key_val, uint8_t key_up, uint8_t key_down, uint32_t* ckey);
void KEY_detectButtonClick(uint8_t key_val, uint8_t key_up, uint8_t key_down, uint32_t* skey, uint8_t* key_flag);

#endif
