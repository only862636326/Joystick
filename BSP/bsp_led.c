/**
  ******************************************************************************
  * @file    led
  * @author  xjc
  * @version v1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "bsp_led.h"
#include "drv_systick.h"

/**
  * @brief  
  * @param  
  * @retval 
  */
  
uint8_t led_mode = 0;
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_Init_Structure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init_Structure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

void LED_Task(void)
{
     if(led_mode == 0)
        LED0 = ~LED0, LED1 = ~LED1;
     if(led_mode == 1)
        LED1 = 0, LED0 = ~LED0;
     if(led_mode == 2)
        LED0 = 0, LED1 = ~LED1;
}
void Change_LED_Mode(uint8_t i)
{
    
    
    led_mode = i;
    
}

