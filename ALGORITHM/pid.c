/**
  *****************************************************************
  * @file    pid.c
  * @author  ShengNan Wang
  * @version V1.2.2
  * @date    2020.2.2
  * @brief   PID�������
  *****************************************************************    
  * @����˵��:
  *     1.1.0: 
  *          1.��PID�����˼��ع� ��װ
  *     1.2.1:
  *          1.��������PID�ļ���, �Լ��ټ�����Դ
  *     1.2.2  2.11:
  *          1.ȥ����̵ĺ�������,Ӧ�����Ἣ�ٵ���
  *          2.ע�͵�ͬʱ��������PID�ĺ���
  *
  *****************************************************************
  */


# include "pid.h"


/**
  * @brief  ����PIDΪĬ�ϲ���
  * @param  
  * @retval 
  */
void PID_t_init(pTYPE_PID_t pPID_t)
{
    
    pPID_t -> Kp = PID_DEFAULT_P_I_D;
    pPID_t -> Ki = PID_DEFAULT_P_I_D;
    pPID_t -> Kd = PID_DEFAULT_P_I_D;
    
    pPID_t -> deadBand  = PID_DEFAULT_DEAD_BAND;
    pPID_t -> maxOutput = PID_DEFAULT_MAX_OUT_PUT;
    pPID_t -> maxIntegral = PID_DEFAULT_MAX_INTEGRAL;
    pPID_t -> maxIncrease = PID_DEFAULT_MAX_INCREASE;
    
    pPID_t -> err       = 0;
    pPID_t -> preErr    = 0;
    pPID_t -> lastErr   = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
    
}

/**
  * @brief  ����PID�������������Ϊ0
  * @param  
  * @retval 
  */

void PID_t_data_clear(pTYPE_PID_t pPID_t)
{    
    pPID_t -> err      = 0;
    pPID_t -> preErr   = 0;
    pPID_t -> lastErr  = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
 
}
/**
  * @brief  ����PID����ֵΪ0
  * @param  
  * @retval 
  */


void PID_t_all_clear(pTYPE_PID_t pPID_t)
{
    pPID_t -> Kp = 0;
    pPID_t -> Ki = 0;
    pPID_t -> Kd = 0;
    
    pPID_t -> deadBand    = 0;
    pPID_t -> maxOutput   = 0;
    pPID_t -> maxIntegral = 0;
    pPID_t -> maxIncrease = 0;
    
    pPID_t -> err            = 0;
    pPID_t -> preErr         = 0;
    pPID_t -> lastErr        = 0;
    
    pPID_t -> feedBack        = 0;
    pPID_t -> reference       = 0;
    pPID_t -> outPutPositon   = 0;
    pPID_t -> outPutIncrement = 0;
              
}             


/**
  * @brief  ����PID
  * @param  P I D 
  *         ������С��0ʱ����ԭֵ����
  * @retval 
  */

void PID_set_pid_parameter( pTYPE_PID_t   pPID_t,
                            TYPE_PID_DATA Kp,
                            TYPE_PID_DATA Ki, 
                            TYPE_PID_DATA Kd)
{
    if ( Kp >= 0) pPID_t -> Kp = Kp;
    if ( Ki >= 0) pPID_t -> Ki = Ki;
    if ( Kd >= 0) pPID_t -> Kd = Ki;
}




/**
  * @brief      ���òο�ֵ
  * @param      
  * @retval     
  *            
  *            
  */
void PID_API_set_reference( pTYPE_PID_t   pPID_t,
                            TYPE_PID_DATA reference)
{
    pPID_t -> reference = reference;
}


/**
  * @brief   ���õ�ǰֵ
  * @param   
  * @retval  
  *          
  *          
  */
void PID_set_feed_back( pTYPE_PID_t   pPID_t,
                        TYPE_PID_DATA feedBack)
{
    pPID_t -> feedBack = feedBack;
}


/**
  * @brief   ����λ��PIDƫ���������
  * @param   
  * @retval  
  *          
  *          
  */

void PID_cal_positon_err(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA current_err =  pPID_t -> reference - pPID_t -> feedBack ;
    pPID_t -> err = current_err;
    // ���ּ���               
    pPID_t -> errIntegral += current_err;
    if (pPID_t -> errIntegral > pPID_t -> maxIntegral )
        pPID_t -> errIntegral = pPID_t -> maxIntegral;
    else if (pPID_t -> errIntegral < -pPID_t -> maxIntegral )
        pPID_t -> errIntegral = -pPID_t -> maxIntegral;
    
    // ΢�ּ��� 
    // �Ƿ�΢���˲�
    pPID_t -> errDifferential = current_err - pPID_t -> lastErr;
   
    #if PID_DIFFERENTIAL_WAVE_FILTERING
    {
        ;
    }
    #endif
}
/**
  * @brief   ��������PIDƫ���������
  * @param   
  * @retval  
  *          
  *          
  */

void PID_cal_increment_err(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA current_err =  pPID_t -> reference - pPID_t -> feedBack ;
    pPID_t -> preErr  = pPID_t -> lastErr;
    pPID_t -> lastErr = pPID_t -> err    ;
    pPID_t -> err     = current_err      ;
}



/**
  * @brief   ����λ��PID���ֵ
  * @param   
  * @retval  
  *          
  *          
  */
TYPE_PID_DATA PID_cal_positon_output(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA outPutPositon = 0.0f;
    // ����
    if (pPID_t -> feedBack > pPID_t -> reference - pPID_t -> deadBand &&
        pPID_t -> feedBack < pPID_t -> reference + pPID_t -> deadBand)
    {
            ;
    }
    //������
    else
    {
        outPutPositon =    pPID_t -> Kp * pPID_t -> err                        +
                           pPID_t -> Ki * pPID_t -> errIntegral                -
                           pPID_t -> Kd * pPID_t -> errDifferential            ;
        // limite outPutPositon
        if (outPutPositon > pPID_t -> maxOutput)
            outPutPositon = pPID_t -> maxOutput;
        else if (outPutPositon < -pPID_t -> maxOutput)
            outPutPositon = -pPID_t -> maxOutput;
        pPID_t -> outPutPositon = outPutPositon;
    }
    return pPID_t -> outPutPositon;
}

/**
  * @brief   ��������PID���ֵ
  * @param   
  * @retval  
  *          
  *          
  */
TYPE_PID_DATA PID_cal_increment_output(pTYPE_PID_t pPID_t)
{
    TYPE_PID_DATA outPutIncrement = 0.0f;    
    
    // ����
    if (pPID_t -> feedBack > pPID_t -> reference - pPID_t -> deadBand &&
        pPID_t -> feedBack < pPID_t -> reference + pPID_t -> deadBand)
    {
            ;
    }
    //������
    else
    {
        outPutIncrement =   pPID_t -> Kp * (pPID_t -> err - pPID_t -> lastErr)
                        +   pPID_t -> Ki *  pPID_t -> err
                        +   pPID_t -> Kd * (pPID_t -> err - 2 * pPID_t -> lastErr + pPID_t -> preErr);
        // limite outPutIntegral
        if (outPutIncrement > pPID_t -> maxIncrease)
            outPutIncrement = pPID_t -> maxIncrease;
        else if (outPutIncrement < -pPID_t -> maxIncrease)
            outPutIncrement = -pPID_t -> maxIncrease;
        pPID_t -> outPutIncrement = outPutIncrement;
    }
    return pPID_t -> outPutIncrement;
}

TYPE_PID_DATA PID_get_outPutPosition(pTYPE_PID_t pPID_t)
{
    return pPID_t -> outPutPositon;
}
TYPE_PID_DATA PID_get_outPutIncrement(pTYPE_PID_t pPID_t)
{
    return pPID_t -> outPutIncrement;
}

/**
  * @brief   ��ǰֵ �õ� �������ֵ ��API
             refence  �趨�Ĳο�ֵ  
  * @param   pPID_t   PID�ṹ��ָ��
             feedBack ��ǰֵ
  * @retval  ����ʽPIDֵ
  *        
  *        
  */
TYPE_PID_DATA PID_API_inputData_getOutputIncrement( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack)
{
    
    pPID_t -> feedBack           = feedBack;    
    PID_cal_increment_err          (pPID_t);
    return PID_cal_increment_output(pPID_t);
   // return PID_get_outPutIncrement (pPID_t);
}



/**
  * @brief   ��ǰֵ �õ� λ�����ֵ ��API
             refence  �趨�Ĳο�ֵ  
  * @param   pPID_t   PID�ṹ��ָ��
             feedBack ��ǰֵ
  * @retval  λ��ʽPIDֵ
  *        
  *        
  */
TYPE_PID_DATA PID_API_inputData_getOutputPosition( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA feedBack)
{
    
    pPID_t -> feedBack           = feedBack;    
    PID_cal_positon_err(pPID_t);
    return PID_cal_positon_output (pPID_t);
    //return PID_get_outPutPosition (pPID_t);
}

/**
  * @brief   ����PID���в�����API
  * @param   pPID_t   PID�ṹ��ָ��
              ...  ��ӦPID����ֵ

                  С��0ʱ��Ӧ�Ĳ���ֵ���ֲ���

  * @retval  None
  *        
  *        
  */
void          PID_API_set_parameter               ( pTYPE_PID_t   pPID_t,
                                                    TYPE_PID_DATA Kp,
                                                    TYPE_PID_DATA Ki, 
                                                    TYPE_PID_DATA Kd,
                                                    TYPE_PID_DATA deadBand,        
                                                    TYPE_PID_DATA maxOutput,
                                                    TYPE_PID_DATA maxIntegral,
                                                    TYPE_PID_DATA maxIncrease)
{
    
    if ( Kp >= 0) pPID_t -> Kp = Kp;
    if ( Ki >= 0) pPID_t -> Ki = Ki;
    if ( Kd >= 0) pPID_t -> Kd = Ki;
    if (deadBand    >= 0) pPID_t -> deadBand    = deadBand   ;
    if (maxOutput   >= 0) pPID_t -> maxOutput   = maxOutput  ;
    if (maxIntegral >= 0) pPID_t -> maxIntegral = maxIntegral;
    if (maxIncrease >= 0) pPID_t -> maxIncrease = maxIncrease;
  
}





/**
  * @brief   һ��ʹ��ʵ��
  * @param   
              
  * @retval  
  *        
  *        
  */

/*

void pid_test(void)
{
    float feedBack1 = 100, feedBack2 = 100 ;
    TYPE_PID_t  pid_t1, pid_t2;
    pTYPE_PID_t ppid_t1 = &pid_t1, ppid_t2 = &pid_t2;
    
    PID_t_init(ppid_t1);
    PID_API_set_parameter(ppid_t1, 0.2, 0.2, 0, -1, -1, -1, -1);
    
    PID_t_init(ppid_t2);    
    PID_API_set_parameter(ppid_t2, 0.02, 0.11, 0, -1, -1, -1, -1);
    PID_API_set_reference(ppid_t1, 19);
    PID_API_set_reference(ppid_t2, 1000);
    
    while(1)
    {
        feedBack1 = PID_API_inputData_getOutputPosition  (ppid_t1, feedBack1);
        feedBack2 += PID_API_inputData_getOutputIncrement(ppid_t2, feedBack2);
        
        printf("position  :%f  \t increment  :%f\r\n", feedBack1, feedBack2);
        Sleep(100);
    }   
}

int main()
{
    
    pid_test();
    return 0;   
    
}
*/

