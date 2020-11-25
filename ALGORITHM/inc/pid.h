#ifndef __PID_H__
#define __PID_H__

// #include <stdio.h>
// #include <windows.h>                              
#include "stm32f10x.h"


// Ĭ�ϲ���                      
#define PID_DEFAULT_P_I_D              0.2
#define PID_DEFAULT_DEAD_BAND          0
#define PID_DEFAULT_MAX_OUT_PUT        10000.0f
#define PID_DEFAULT_MAX_INTEGRAL       10000.0f
#define PID_DEFAULT_MAX_INCREASE       100.0f

// �˲� ���幦��δʵ��
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
    
    // ƫ�� �������
    TYPE_PID_DATA err, lastErr, preErr;     // ָ��
    TYPE_PID_DATA errDifferential;          // ΢��
    TYPE_PID_DATA errIntegral;              // ���� 
    
    // �˲��� 
    #if PID_FEEDBACK_WAVE_FILTERING
    TYPE_PID_DATA errWaveFilter[PID_ERR_WAVE_FILTERING *2 + 1];      //
    #endif 
    
    #if PID_DIFFERENTIAL_WAVE_FILTERING
    TYPE_PID_DATA differentialWaveFilter[PID_DIFFERENTIAL_WAVE_FILTERING * 2 + 1];
    #endif
    
    // ��ǰֵ
    TYPE_PID_DATA feedBack;
    // �ο�ֵ
    TYPE_PID_DATA reference;
    // ���
    TYPE_PID_DATA outPutPositon;
    TYPE_PID_DATA outPutIncrement;
    
}TYPE_PID_t, *pTYPE_PID_t;


//�ѷ���
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












