#include "moto_demo.h"
#include "drv_can.h"
#include "moto_control.h"
#include "drv_systick.h"
#include "drv_adc.h"

#define MOTO_DJI      0x01
#define MOTO_MODULE   0x02
#define MOTO_DJI_3508 0x04
#define MOTO_DJI_2006 0x08


uint8_t g_Moto_Control_flag;

TYPE_Moto_t moto_1;

void Moto_Init(uint8_t type)
{
    
    if(type == MOTO_DJI_2006)
    {
        Moto_API_PID_Init_C610(&moto_1);
        
        g_Moto_Control_flag = MOTO_DJI_2006;
        Change_Can_Use(MOTO_DJI);
    }
    else if(type == MOTO_DJI_3508)
    {
        Moto_API_PID_Init_C620(&moto_1);
        g_Moto_Control_flag = MOTO_DJI_3508;
        Change_Can_Use(MOTO_DJI);
    }
    else if(type == MOTO_MODULE)
    {
        CAN_RoboModule_DRV_Reset(0,0);                      //对0组所有驱动器进行复位 
        delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成    
        CAN_RoboModule_DRV_Mode_Choice(0,0,Velocity_Mode);  //0组的所有驱动器 都进入开环模式
        delay_ms(500);                                      //发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
        CAN_RoboModule_DRV_Config(0,0,100,0);               //1号驱动器配置为100ms传回一次数据
        delay_ms(100);   
        
        g_Moto_Control_flag = MOTO_MODULE;
        Change_Can_Use(MOTO_MODULE);
    }
    else
    {
        g_Moto_Control_flag = 0;
    }
}
void Moto_Task(void)
{
    int dstvelocity,dstPosition;
    int16_t I1;
    int v1, v2;
    if(g_Moto_Control_flag == MOTO_DJI_3508)
    {
        dstPosition = C620_MotorInfo[0].Position;
        dstvelocity = C620_MotorInfo[0].Velocity;


        Moto_API_SetMotoInfo(&moto_1, dstvelocity, dstPosition); 

        v1 = ADC_Value_G[2] / 200 * 200;
        v2 = ADC_Value_G[3] / 10 * 10;
        I1 = Moto_API_Cal_GetCurrentVelocityMode(&moto_1, v1 / 2048.0f * 5000 + v2 / 2048.0f * 500);
        absLimit_int16(&I1, 8000);

        C620_SendCurrentVal(I1, I1, I1, I1);
    }
   if(g_Moto_Control_flag == MOTO_DJI_2006)
    {
        dstPosition = C620_MotorInfo[0].Position;
        dstvelocity = C620_MotorInfo[0].Velocity;


        Moto_API_SetMotoInfo(&moto_1, dstvelocity, dstPosition); 
        
        v1 = ADC_Value_G[2] / 200 * 200;
        v2 = ADC_Value_G[3] / 10 * 10;
        I1 = Moto_API_Cal_GetCurrentVelocityMode(&moto_1, v1 / 2048.0f * 13000 + v2 / 2048.0f * 4000);
        absLimit_int16(&I1, 8000);

        C620_SendCurrentVal(I1, I1,I1, I1);
    }
    else if(g_Moto_Control_flag == MOTO_MODULE)
    {
         CAN_RoboModule_DRV_Velocity_Mode(0, 0, 3000, ADC_Value_G[2] / 4.0950f * 5 + ADC_Value_G[3] / 4.0950f);
    }
    else
    {
      ;  g_Moto_Control_flag = 0;
        
    }
}

void PWM_Task(void)
{
    
    int pwmval1, pwmval2;
    pwmval1 = ADC_Value_G[4] / 40.950f;
    pwmval2 = ADC_Value_G[4] / 40.950f;
    TIM_SetCompare1(TIM3,1800 + pwmval1);	
    TIM_SetCompare2(TIM3,1800 + pwmval2);
}










