/**
  ***********************************************************
  * @file    moto_control.c
  * @author  Shengnan Wang
  * @version V1.2.3
  * @date    2020.1.9
  * @brief   ʹ�ÿ��Ƶ��������ģʽ
  *          �ٶ�ģʽ  λ��ģʽ  �ٶ�λ��ģʽ ����PID, 
  *
  ***********************************************************
  * @����˵��:
  *     1.0.1 ����,C620 ΪMoto ��ʾ���ǿ��Ƶ����, ������ϵ����
  *     1.1.0 ��Ϊmoto_control ʹ�ļ�����, ��ص���ȫͨ���������
  *           ��Ҫʹ��ʱ�ýṹ�嶨��һ�����������ʼ��
  *     1.1.1 ����λ����Ϣ��¼��BUG,���������ٶ�λ��ģʽ,
  *              ʹ����˳������,����3508����ɲ���
  *     1.1.2 1.����C610�ĵ����ʼ��,
  *           2.����Moto_API_PID_Init_C620����,��ԭMoto_API_PID_Init
  *           3.���Ӷ����ϵ���PID
  *     1.2.1 1.���ӵ����MAX_RPM����, ʹ��ͬ���MAX_RPM���Բ�ͬ
  *           2.����˿���C610�Ĳ���
  *           3.��������MAX_RPM��API
  *     1.2.2 1.����������������������
  *     1.2.3 1.�����ٶ�ģʽ��,ÿ�μ������ǰ���ٶȽ����ữ
  *
  *
  **********************************************************
  * ��Ȩ����
****/
#include "moto_control.h"


/**
  * @brief PID��ʼ��C620
  * @param 
  * @retval
  */
void Moto_API_PID_Init(pTYPE_Moto_t pMoto_t)
{   
    PID_t_data_clear(&pMoto_t ->pid_Position_t);
    PID_t_data_clear(&pMoto_t ->pid_Velocity_t);
    /*ȫ������*/
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, 
                    11.7000f,0.5000f, 8.0000f, //Kp,      Ki,    Kd, 
                    5.0f, 16383 / 20 * 10,5000.0f, 10.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    PID_API_set_parameter(& pMoto_t -> pid_Position_t,
                    0.0200f, 0.0100f, 0.0000f, //Kp,      Ki,    Kd, 
                    2000.0f, 8000,      3000.0f, 16.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    /*������       
    PID_API_set_parameter( & pMoto_t ->pid_Velocity_t,
                    5.0f, 0.5f, 0.0f, //Kp,      Ki,    Kd, 
                    50.0f, 0.8192 * 4000,3000.0f, 600.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    
    */
    pMoto_t -> max_rpm = MOTO_MAX_RPM_C620;
    pMoto_t -> soft_value = MOTO_SOFT_C620;
}
/**
  * @brief PID��ʼ��C620
  * @param 
  * @retval
  */
void Moto_API_PID_Init_C620(pTYPE_Moto_t pMoto_t)
{
    Moto_API_PID_Init(pMoto_t);
}


/**
  * @brief PID��ʼ��C610
  * @param 
  * @retval
  */

void Moto_API_PID_Init_C610(pTYPE_Moto_t pMoto_t)
{
    PID_t_data_clear(&pMoto_t ->pid_Position_t);
    PID_t_data_clear(&pMoto_t ->pid_Velocity_t);
           
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, 
                    5.0f, 0.05f, 1.0f, //Kp,      Ki,    Kd, 
                    5.0f, 0.8192 * 6000,3000.0f, 5.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
                    
    PID_API_set_parameter(& pMoto_t -> pid_Position_t, 
                    0.1f, 0.0f, 0.0f, //Kp,      Ki,    Kd, 
                    100.0f, 20000.0f, 5000.0f, 16.0f);
                    //deadBand, maxOutput, maxIntegral, maxIncrease
    pMoto_t -> max_rpm = MOTO_MAX_RPM_C610;
    pMoto_t -> soft_value = MOTO_SOFT_C610;
}


/**
  * @brief  ���õ����Ϣ
  *             �ٶ�ģʽ�ɽ������ٶ�,λ��ģʽ�ɽ�����λ��
  *               �ٶ�_λ��ģʽ��������Ҫ����
  * @param  pMoto_t �����һ���ṹ��
  *         present_velocity, present_position
  *         
  * @retval 
  */

void Moto_API_SetMotoInfo(pTYPE_Moto_t pMoto_t, int16_t present_velocity, int32_t present_position)
{
    pMoto_t -> present_velocity = present_velocity;
    pMoto_t -> present_position = present_position;
}

/**
  * @brief  �����ٶȻ�PID
  * @param  pMoto_t �����һ���ṹ��
            Kp, Ki, Kd
  * @retval 
  */
void Moto_API_SetVelocityPID(
pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd)
{
    PID_API_set_parameter(& pMoto_t -> pid_Velocity_t, Kp, Ki, Kd, -1, -1,-1 -1,-1);
}

/**
  * @brief  ���� �ٶ�ģʽ�� �趨�ٶ���Ӧ�÷��͵ĵ���
  *         
  * @param  dstVelocity : �趨��RPM
  * @retval Ӧ�÷��͵ĵ���
  */

int16_t Moto_API_Cal_GetCurrentVelocityMode(pTYPE_Moto_t pMoto_t, float dstVelocity)
{

    //����̵�����ٶȹ���
#if MOTO_SOFT
    if(pMoto_t -> soft_value != 0.0f)
       dstVelocity = FlexibelValue_int16(dstVelocity,
                pMoto_t -> src_rpm, pMoto_t -> soft_value);
#endif
    absLimit(&dstVelocity, pMoto_t -> max_rpm);
    PID_API_set_reference(& pMoto_t -> pid_Velocity_t, dstVelocity);
    return (int16_t) PID_API_inputData_getOutputPosition(& pMoto_t -> pid_Velocity_t, pMoto_t -> present_velocity);
}

/**
  * @brief  ����λ�û�PID
  * @param  Kp, Ki, Kd
  * @retval 
  */
void Moto_API_SetPositionPID(pTYPE_Moto_t pMoto_t, float Kp, float Ki, float Kd)
{
    PID_API_set_parameter(& pMoto_t -> pid_Position_t, Kp, Ki, Kd, -1, -1,-1 -1,-1);
}

/**
  * @brief  ���� λ��ģʽ�� �趨λ����Ӧ�÷��͵ĵ���
  * @param  dstPosition : �趨��
  *         
  * @retval Ӧ�÷��͵ĵ���
  */

int16_t Moto_API_Cal_GetCurrentPositionMode(pTYPE_Moto_t pMoto_t, int32_t dstPosition)
{
    PID_API_set_reference(& pMoto_t -> pid_Position_t,(float) dstPosition);
    return (int16_t) PID_API_inputData_getOutputPosition(& pMoto_t -> pid_Position_t, pMoto_t -> present_position);
}



/**
  * @brief  ���� �ٶ�_λ��ģʽ�� �趨λ��_�ٶ���Ӧ�÷��͵ĵ���
  * @param  dstPosition : �趨��λ��
  *         dstVelocity    : �趨�ٶ�
  * @retval Ӧ�÷��͵ĵ���
  * 
  */
int16_t Moto_API_Cal_GetCurrentVelocityPositionMode(pTYPE_Moto_t pMoto_t, int32_t dstPosition, float dstVelocity)
{
    int16_t v = Moto_API_Cal_GetCurrentPositionMode(pMoto_t, dstPosition);
    absLimit_int16(&v, dstVelocity);    
    return Moto_API_Cal_GetCurrentVelocityMode(pMoto_t, v);
}


/**
  * @brief  �趨�������ٶȵ�API
  * @param  pMoto_t     : ����Ľṹ��ָ��
  *         max_rpm     : �趨������ٶ�
  * @retval Ӧ�÷��͵ĵ���
  * 
  */
void Moto_API_Set_Max_RPM(pTYPE_Moto_t pMoto_t, float max_rpm)
{
    
    pMoto_t -> max_rpm = max_rpm;
}


/**
  * @brief  һ��ʹ��ʵ�� 
            �ο�ʹ�ã�����������
  * @param  
  *         
  * @retval 
  */
/*


uint8_t flag = 0;
TYPE_Moto_t moto_1;
int32_t g_set_position = 0;

C620_MotorInfomation_t** pp_MotorInfo_t;

void Moto_Init(void)
{
    Moto_API_PID_Init(&moto_1);
    Get_C620_MotorInfo(pp_MotorInfo_t);
}



void Moto_1ms_task(void)
{
    int dstPosition = pp_MotorInfo_t[1]->Position;
    int dstvelocity = pp_MotorInfo_t[1]->Velocity;
    int16_t I1;
	
    Moto_API_SetMotoInfo(&moto_1, dstvelocity, dstPosition); 
    
    I1 = Moto_API_Cal_GetCurrentVelocityPositionMode(&moto_1, g_set_position, 1000);
    absLimit_int16(&I1, 5000);
    if(flag)
        C620_SendCurrentVal(0, I1, 0, 0);
    else
        C620_SendCurrentVal(0, 0, 0, 0);
}
void Moto_Set_PID(float kp, float ki, float kd)
{
    Moto_API_SetVelocityPID(&moto_1, kp, ki, kd);
}




*/





































