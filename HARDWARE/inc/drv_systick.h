#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void delay_us(u32 time);
void delay_ms(u32 time);



#endif

