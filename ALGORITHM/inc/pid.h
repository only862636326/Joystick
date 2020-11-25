#ifndef __PID_H__
#define __PID_H__

// #include <stdio.h>
// #include <windows.h>                              
#include "stm32f10x.h"


// 默认参数                      
#define PID_DEFAULT_P_I_D              0.2
#define PID_DEFAULT_DEAD_BAND          0
#define PID_DEFAULT_MAX_OUT_PUT        10000.0f
#define PID_DEFAULT_MAX_INTEGRAL       10000.0f
#define PID_DEFAULT_MAX_INCREASE       100.0f

// 滤波 具体功能未实现
#define PID_FEEDBACK_WAVE_FILTERING         0  
#define PID_DIFFERENTIAL_WAVE_FILTERING     0


typedef float  TYPE_PID_DATA, *pTYPE_PID_DATA;

typedef struct TYPE_PID_t{
    TYPE_PID_DATA Kp;
    TYPE_PID_DATA Ki;
    TYPE_PID_DATA Kd;
    
    TYPE_PID_DATA deadBand;
    TYPE_PID_DATA maxOutput;
    TYPE_PID_DATA maxIntegral;
    TYPE_PID_DATA maxIncrease;
    
    // 偏差 相关数据
    TYPE_PID_DATA err, lastErr, preErr;     // 指针
    TYPE_PID_DATA errDifferential;          // 微分
    TYPE_PID_DATA errIntegral;              // 积分 
    
    // 滤波器 
    #if PID_FEEDBACK_WAVE_FILTERING
    TYPE_PID_DATA errWaveFilter[PID_ERR_WAVE_FILTERING *2 + 1];      //
    #endif 
    
    #if PID_DIFFERENTIAL_WAVE_FILTERING
    TYPE_PID_DATA differentialWaveFilter[PID_DIFFERENTIAL_WAVE_FILTERING * 2 + 1];
    #endif
    
    // 当前值
    TYPE_PID_DATA feedBack;
    // 参考值
    TYPE_PID_DATA reference;
    // 输出
    TYPE_PID_DATA outPutPositon;
    TYPE_PID_DATA outPutIncrement;
    
}TYPE_PID_t, *pTYPE_PID_t;


//已分离
//TYPE_PID_DATA PID_cal_output          ( pTYPE_PID_t  pPID_t);
TYPE_PID_DATA PID_cal_positon_output  ( pTYPE_PID_t  pPID_t);
TYPE_PID_DATA PID_cal_increment_output( pTYPE_PID_t  pPID_t);
//void PID_cal_err                      ( pTYPE_PID_t  pPID_t);
void PID_cal_positon_err              ( pTYPE_PID_t  pPID_t);
void PID_cal_increment_err            ( pTYPE_PID_t  pPID_t);




void PID_t_init                      ( pTYPE_PID_t   pPID_t);
void PID_t_err_clear                 ( pTYPE_PID_t   pPID_t);
void PID_t_data_clear                ( pTYPE_PID_t   pPID_t);

void PID_set_pid_parameter           ( pTYPE_PID_t   pPID_t,
                                       TYPE_PID_DATA Kp,
                                       TYPE_PID_DATA Ki, 
                                       TYPE_PID_DATA Kd);
                                     
void PID_set_feed_back               ( pTYPE_PID_t   pPID_t,
                                       TYPE_PID_DATA feedBack);
TYPE_PID_DATA PID_get_outPutIncrement( pTYPE_PID_t   pPID_t);
TYPE_PID_DATA PID_get_outPutPosition ( pTYPE_PID_t   pPID_t);



void          PID_API_set_parameter               ( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA Kp,
                                                    TYPE_PID_DATA Ki, 
                                                    TYPE_PID_DATA Kd,
                                                    TYPE_PID_DATA deadBand,                                                    
                                                    TYPE_PID_DATA maxOutput,
                                                    TYPE_PID_DATA maxIntegral,
                                                    TYPE_PID_DATA maxIncrease);

void          PID_API_set_reference               ( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA reference);
TYPE_PID_DATA PID_API_inputData_getOutputIncrement( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack);
TYPE_PID_DATA PID_API_inputData_getOutputPosition ( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack);



#endif












