
#include "drv_can.h"
#include "bsp_esc.h"
#include "moto_control.h"
#include "bsp_robomodule.h"

#include "drv_can.h"
#include "drv_pwm.h"
#include "drv_gpio.h"
#include "bsp_led.h"
#include "drv_timer.h"
#include "drv_systick.h"
#include "moto_demo.h"
#include "bsp_oled.h"

#define SET_VALUE 500

void World_Init(void)
{
    int i = 0;
	SystemInit();
	uart_init(115200);
	printf("usart is ready\r\n");
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	#ifdef SX1278_COMMUNICATION
    SX1278_Init();
	#endif
    
	LED_Init();
    GPIOC_Init();
	OLED_Init();
	GUI_Init();
	Key_Init();
    TIM3_PWM_Init(2000-1, 720-1);
    TIM4_Init();  
	TIM2_Init(); // KEY
   
    TIM6_Init(); // updata, tansmit

    JOY_ADC_Init();
    
    delay_ms(500);
    do
    {
        if(ADC_Value_G[0] > SET_VALUE)
        {
            i++;
        }
        else
        {
            break;
        }
        delay_ms(5);
    }
    while (i<100);
    
    if(i >50)
    {
        Change_LED_Mode(2);
        CAN1_Init(); 
        if(ADC_Value_G[1] < -SET_VALUE)
        {
            Moto_Init(0x08); // DJI 2006
        }
        else
        {
            Moto_Init(0x04); // DJI 3508
        }
    }
    i = 0;
    do
    { if(ADC_Value_G[0] < -SET_VALUE)
        {
            i++;
        }
        else
        {
            break;
        }
        delay_ms(5);
    }
    while (i < 100);
    
    if(i >50)
    {
        Change_LED_Mode(1);
        CAN1_Init(); 
        Moto_Init(0x02); // Mmodule
    }
    
    
    TIM5_Init(); // LED
}


int	main()
{
    
    int i = 0;

    World_Init();
    

	while(1)
	{
        
        i ++;
        delay_ms(2);
        Moto_Task();
        PWM_Task();
        
    }
}





