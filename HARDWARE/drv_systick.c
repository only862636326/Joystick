#include "drv_systick.h"

u32 count;

void SysTick_Init(void)
{
	SysTick->LOAD  = (uint32_t)(SystemCoreClock/1000000 - 1UL);
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void delay_us(u32 time)
{
	if(time<=0)
		return;

	count = time;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(count!=0);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(u32 time)
{
	if(time<=0)
		return;

	count = time*1000;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(count!=0);
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//systick中断处理函数
void SysTick_Handler(void)
{
	if(count!=0){
		count--;
	}
}

