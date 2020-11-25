#ifndef __MOTO_CONTROL_H__
#define __MOTO_CONTROL_H__


#include "stm32f10x.h"
#include "pid.h"
#include "user_math.h"


//电机的最大转速设置,大约为3400 mm/s
//与底盘的最大速度共存
//几乎可达底盘最大, 一般无需更改
//也可加大以使电机到达更高效能
// 620 133r / s
#define MOTO_MAX_RPM_C620   8000.0f

//C610, 控制2006 最大为19000, 额定16000
#define MOTO_MAX_RPM_C610   16000.0f

#define MOTO_SOFT           0
        
#define MOTO_SOFT_C610      200
#define MOTO_SOFT_C620      10


typedef struct TYPE_Moto_t
{
    TYPE_PID_t pid_Velocity_t; 
    TYPE_PID_t pid_Position_t;
    int16_t    present_velocity;
    int32_t    present_position;
    
    float   src_rpm;
    float   soft_value;
    float   max_rpm;
    
}TYPE_Moto_t, *pTYPE_Moto_t;
 


// 速度环
void    Moto_API_SetVelocityPID             (pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd);
int16_t Moto_API_Cal_GetCurrentVelocityMode (pTYPE_Moto_t pMoto_t, float dstVelocity);
// 位置环
void    Moto_API_SetPositionPID             (pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd);
int16_t Moto_API_Cal_GetCurrentPositionMode (pTYPE_Moto_t pMoto_t, int32_t dstPosition);

void    Moto_API_PID_Init               (pTYPE_Moto_t pMoto_t);//620
void    Moto_API_PID_Init_C620          (pTYPE_Moto_t pMoto_t);//620
void    Moto_API_PID_Init_C610          (pTYPE_Moto_t pMoto_t);//610

//void    Moto_Soft          (void);

int16_t Moto_API_Cal_GetCurrentVelocityPositionMode(pTYPE_Moto_t pMoto_t, int32_t dstPosition, float dstVelocity);
void    Moto_API_SetMotoInfo(pTYPE_Moto_t pMoto_t, int16_t present_velocity, int32_t present_position);
#endif








