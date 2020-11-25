/**
  ******************************************************************************
  * @file   
  * @author  xjc
  * @version v1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "task.h"

void Bey_Bound_Deal(void)
{
    uint8_t i;
    for(i = 0;i < 4;i++)
    {
        if(KEY_G[12+i])
            GPIO_SetBits(GPIOC, GPIO_Pin_0 << i);
        else
            GPIO_ResetBits(GPIOC, GPIO_Pin_0 << i);
    }    
    
    if(KEY_G[2])
        GPIO_SetBits(GPIOC, GPIO_Pin_5);
    else
        GPIO_ResetBits(GPIOC, GPIO_Pin_5);
     
    if(KEY_G[3])
         GPIO_SetBits(GPIOC, GPIO_Pin_4);
    else
        GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}

void Task_1ms(void)
{   
    ;
}

void Task_30ms(void)
{
	GUI_ADC_Updata();
    Bey_Bound_Deal();
    
#ifdef UART_COMMUNICATION                   //按键刷新函数在发送函数内部 为确保按键刷新无误 刷新函数在发送后 清零前
	Uart_Transmit();
#endif
		
#ifdef SX1278_COMMUNICATION
	SX1278_Transmit();
#endif
	
#ifdef UART_DEBUG
	Uart_Debug_Transmit();
#endif

}

void Task_500ms(void)
{
	 LED_Task();
}



