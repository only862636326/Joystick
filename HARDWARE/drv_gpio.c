







#include "drv_gpio.h"

/**
  * @brief  
  * @param  
  * @retval 
  */
void GPIOC_Init(void)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Init_Structure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
	GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Init_Structure);
	
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5);
}








