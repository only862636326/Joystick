#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PBout(8)
#define LED1 PBout(9)

void LED_Init(void);
void LED_Task(void);
void Change_LED_Mode(uint8_t i);

#endif
