
/**
  ***********************************************************
  * @file    chassis.c
  * @author  Shengnan Wang
  * @version V1.3.3
  * @date    2020.10.10
  * @brief   ���̵�һЩ����
  *          , 
  *
  ***********************************************************
  * @����˵��:
  *           1.1.0 ��һ�����
  *                 ��������˶������������
  *           
  *           1.2.0    1.���ӿ����и��ĵ�����ת������.
  *                    2.���ӽǶ���������
  *    20,1,8  1.3.0   1.���Ӷ��ֵĿ��� 
  *
  *    20,1,10 1.3.1   1.������������    
  *
  *    20,1,19 1.3.2   1.ȥ���ٶ��ữ���������ٶ��ữ
  *                    2.���м���������
  *                    3.���Ӷ��ֵİ���ģʽ�� ������������δ����
  *    20.10.10 1.3.3  1.������ģʽ�Ĳ���Ϊ�궨��
  *
  **********************************************************
  * ��Ȩ����,��������ô��?
  ****/


#include "chassis.h"


TYPE_ChassisConfig_t g_ChassisConfig_t;
TYPE_ChassisStatus_t g_ChassisStatus_t;


/**
  * @brief ����״̬������
  *        ���õ���Ϊ��ʼ״̬
  *        �趨���е�����������Ϊ0
  *        ���ú���̴�ʧ��ģʽ
  *        
  * @param 
  * @retval 
  * 
  */

void Chassis_API_StatusReset(void)
{    
    
    g_ChassisStatus_t.speed_x = 0;
    g_ChassisStatus_t.speed_y = 0;
    g_ChassisStatus_t.speed_a = 0;
   
    g_ChassisStatus_t.refSpeed_x = 0;
    g_ChassisStatus_t.refSpeed_y = 0;
    g_ChassisStatus_t.refSpeed_a = 0;
 
    g_ChassisStatus_t.speed_Wheel_1 = 0;
    g_ChassisStatus_t.speed_Wheel_2 = 0;
    g_ChassisStatus_t.speed_Wheel_3 = 0;   
    g_ChassisStatus_t.speed_Wheel_4 = 0;
    
    g_ChassisStatus_t.x = 0;
    g_ChassisStatus_t.y = 0;
    g_ChassisStatus_t.theta = 0;
    
    #if USE_STREER
    g_ChassisStatus_t.position_Wheel_1 = 0;
    g_ChassisStatus_t.position_Wheel_2 = 0;
    g_ChassisStatus_t.position_Wheel_3 = 0;
    g_ChassisStatus_t.position_Wheel_4 = 0;
        
    g_ChassisStatus_t.zero_Position_Wheel_1 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_2 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_3 = 0;
    g_ChassisStatus_t.zero_Position_Wheel_4 = 0;
    #endif
    
    g_ChassisConfig_t.chassisMode   = CHASSIS_MODE_DISNABEL;
}

/**
  * @brief                     ���̳�ʼ��
  *        ���õ��̵Ļ�������         ���趨        ���̵ĳ�ʼ״̬
  *        ֱ���޸Ĵ˺�������                     ���̴���ʧ��״̬
  *        ���趨���� 
  * @param 
  * @retval 
  * 
  */

void Chassis_API_Init(void)
{
    
    g_ChassisConfig_t.wit  = 0    ;
    g_ChassisConfig_t.len  = 0    ;
    g_ChassisConfig_t.r    = 0    ;
    
    g_ChassisConfig_t.maxAxisSpeed = MAX_AXIS_SPEED;
    g_ChassisConfig_t.maxOmagaSpeed= MAX_OMAGA_SPEED;
    
    g_ChassisConfig_t.axisRatio    = RPM_RATION / 1000.0f;  
    g_ChassisConfig_t.omegaRatio   = RPM_RATION / CHASSIS_R;
    
    g_ChassisConfig_t.dirRatio     = 1000 ;
    g_ChassisConfig_t.turnRatio    = 1000 ;
    
    g_ChassisConfig_t.accLimit      = ACC_AXIS  / 1000;
    g_ChassisConfig_t.omagaAccLimit = ACC_OMAGA / 1000;
    
    g_ChassisConfig_t.accLimitStatu = ACC_LIMIT;
    g_ChassisConfig_t.chassisMode   = CHASSIS_MODE_DISNABEL;
    g_ChassisConfig_t.wheelNum      = DEFAULT_CHASSIS_WHEEL; //��������
    
    Chassis_API_StatusReset();
    
}

/**
  * @brief ���ٶ��ữ����������
  *        �˺����ڶ�ʱ����ÿms���������һ��
  *
  * @param 
  * @retval 
  * 
  */

 void Chassis_API_AxisSpeedSoften_Acclimit_Task_1ms(void)
{
 
    if (g_ChassisConfig_t.accLimitStatu == AccLimit_Axis)
    {
        g_ChassisStatus_t.speed_a = FlexibelValue(g_ChassisStatus_t.refSpeed_a, g_ChassisStatus_t.speed_a, g_ChassisConfig_t.omagaAccLimit);
        g_ChassisStatus_t.speed_x = FlexibelValue(g_ChassisStatus_t.refSpeed_x, g_ChassisStatus_t.speed_x, g_ChassisConfig_t.accLimit);
        g_ChassisStatus_t.speed_y = FlexibelValue(g_ChassisStatus_t.refSpeed_y, g_ChassisStatus_t.speed_y, g_ChassisConfig_t.accLimit);
    }
    else
    {
        g_ChassisStatus_t.speed_a = g_ChassisStatus_t.refSpeed_a;
        g_ChassisStatus_t.speed_x = g_ChassisStatus_t.refSpeed_x;
        g_ChassisStatus_t.speed_y = g_ChassisStatus_t.refSpeed_y;
    }
}

/**
  * @brief ��ƽ���ٶȺͽ��ٶȼ��������ٶ�
  * @param 
  * @retval 
  * 
  */

void Chassis_API_CalcWheelSpeed(void)
{
   #if USE_STREER
    int32_t position;
    #endif
    
    if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_3)
    {
        ;
        //���ֵ�, �ò���,Ҳδ��֤��ȷ�� ��ע����
        /*
            g_ChassisStatus_t.Speed_Wheel_1 = +g_ChassisStatus_t.Speed_x + 1.7320508f * g_ChassisStatus_t.Speed_y - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            g_ChassisStatus_t.Speed_Wheel_2 = +g_ChassisStatus_t.Speed_x - 1.7320508f * g_ChassisStatus_t.Speed_y - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            wser 
            g_ChassisStatus_t.Speed_Wheel_3 = -2 * g_ChassisStatus_t.Speed_x - g_ChassisStatus_t.OmegaRatio * g_ChassisStatus_t.Speed_a;
            //g_ChassisStatus_t.Speed_Wheel_4 = 0;
        }*/
    }
    else if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_4)
    {
     /*                                   
                         2 ---                                          
                            |                                         
                            |                                         
                            |                                         
                   1 |------+-------|3                                         
                            |                                         
                            |                                         
                            |         
                         4 ---         
     */
        //ȫ����
        g_ChassisStatus_t.speed_Wheel_1 = - g_ChassisStatus_t.speed_x * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_2 = - g_ChassisStatus_t.speed_y * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_3 = + g_ChassisStatus_t.speed_x * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
        g_ChassisStatus_t.speed_Wheel_4 = + g_ChassisStatus_t.speed_y * g_ChassisConfig_t.axisRatio 
                                          - g_ChassisStatus_t.speed_a * g_ChassisConfig_t.omegaRatio;
    }
#if USE_STREER
    else if(g_ChassisConfig_t.wheelNum == CHASSIS_WHEEL_8)
    {
        //����
        
    
        g_ChassisStatus_t.speed_Wheel_1 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_2 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_3 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        g_ChassisStatus_t.speed_Wheel_4 = g_ChassisStatus_t.speed_y  * g_ChassisConfig_t.axisRatio;
        
        position = g_ChassisStatus_t.speed_x * g_ChassisConfig_t.dirRatio;
        absLimit_int32(&position, MAX_C610_POSITON);
        g_ChassisStatus_t.position_Wheel_1 = position;
        g_ChassisStatus_t.position_Wheel_2 = position;
        g_ChassisStatus_t.position_Wheel_3 = position;
        g_ChassisStatus_t.position_Wheel_4 = position;
        
        position = g_ChassisStatus_t.speed_a * g_ChassisConfig_t.turnRatio;
        g_ChassisStatus_t.position_Wheel_1 += position;
        g_ChassisStatus_t.position_Wheel_2 += position;     
    }
#endif
    else
    {
        
    }
}
/**
  * @brief ��������ת������,����������и���
  *        
  * @param 
  * @retval 
  * 
  */
void Chassis_API_Coordinate_Tranform(float *x, float *y)
{

 //ȫ���ִ����
   float x_ = *x, y_ = *y;
    *x =  -x_ *  0.707f + y_ * 0.707f;
    *y =   x_ *  0.707f + y_ * 0.707f;  
    
}
/**
  * @brief �Ƕ�����
  *        
  * @param 
  * @retval 
  * 
  */
void Chassis_API_Angle_Lock(float kp, float deadBand)
{   
    float delta = g_ChassisStatus_t.theta - g_ChassisStatus_t.lock_a;
    if(delta <-deadBand || delta >deadBand)
        g_ChassisStatus_t.refSpeed_a = -delta * kp;
    else
        g_ChassisStatus_t.refSpeed_a = 0;
}
/**
  * @brief ��������
  *        
  * @param 
  * @retval 
  * 
  */
 void  Chassis_API_Coordinate_Lock(float kp, float deadBand)
{
    float delta_x = g_ChassisStatus_t.x - g_ChassisStatus_t.lock_x;
    float delta_y = g_ChassisStatus_t.y - g_ChassisStatus_t.lock_y;
    float x = 0;
    float y = 0;
    
    if(delta_x <-deadBand || delta_x >deadBand)
        x = -delta_x * kp;
    else
        x = 0.0f;
   
    if(delta_y <-deadBand || delta_y >deadBand)
        y = -delta_y * kp;
    else
        y = 0.0f;
    
    Chassis_API_Coordinate_Tranform(&x, &y);
    
    g_ChassisStatus_t.refSpeed_x = x; 
    g_ChassisStatus_t.refSpeed_y = y;
}

/**
  * @brief �����ٶ��ٶ�, x y �ĵ�λΪ mm/s 
  *        ��������Ӧ�ôﵽ���ٶ�
  * @param x, y, a:
  *        lock_flag : 0��ͨң��
  *                  : 1�Ƕ�����
  *                  : 2�Ƕȡ�λ�ö�����
  *                  : 3λ������
  * @retval 
  * 
  */


void Chassis_API_setV(float x, float y,float a, uint8_t lock_flag)
{
    
    
    absLimit(&x, g_ChassisConfig_t.maxAxisSpeed);
    absLimit(&y, g_ChassisConfig_t.maxAxisSpeed);
    absLimit(&a, g_ChassisConfig_t.maxOmagaSpeed);
    
    
    if(lock_flag==1)
    {
         Chassis_API_Coordinate_Tranform(&x, &y);
        g_ChassisStatus_t.refSpeed_x = x; 
        g_ChassisStatus_t.refSpeed_y = y;
        
        g_ChassisStatus_t.lock_a +=  a;
        g_ChassisStatus_t.lock_x = g_ChassisStatus_t.x;
        g_ChassisStatus_t.lock_y = g_ChassisStatus_t.y;
        
        Chassis_API_Angle_Lock(CHASSIS_LOCK_A_KP, CHASSIS_LOCK_A_DEADBAND);
    }

    else if(lock_flag == 0)
    {
        Chassis_API_Coordinate_Tranform(&x, &y); 
        g_ChassisStatus_t.refSpeed_x = x;
        g_ChassisStatus_t.refSpeed_y = y;
        g_ChassisStatus_t.refSpeed_a = a;
        
        g_ChassisStatus_t.lock_a = g_ChassisStatus_t.theta;
        g_ChassisStatus_t.lock_x = g_ChassisStatus_t.x;
        g_ChassisStatus_t.lock_y = g_ChassisStatus_t.y;   
    }
    else if(lock_flag == 2)
    {
       a *= 0.01f ;
       x *= 0.01f;
       y *= 0.01f;
       g_ChassisStatus_t.lock_a += a;
       g_ChassisStatus_t.lock_x += x;
       g_ChassisStatus_t.lock_y += y;
        
        Chassis_API_Angle_Lock(CHASSIS_LOCK_A_KP, CHASSIS_LOCK_A_DEADBAND);
        Chassis_API_Coordinate_Lock(CHASSIS_LOCK_X_KP, CHASSIS_LOCK_X_DEADBAND);
    }
}

/**
  * @brief ���õ��̵�ǰ��Ϣ 
  * @param 
  * @retval 
  * 
  */
void Chassis_API_setSta_XYA(float x, float y, float a)
{
    g_ChassisStatus_t.x     = x;   
    g_ChassisStatus_t.y     = y;
    g_ChassisStatus_t.theta = a;
}

/**
  * @brief ��ȡ�ٶ�, x y �ĵ�λΪ mm/s 
  * @param 
  * @retval 
  * 
  */
void Chassis_API_getSta_XYA(float *x, float *y,float *a)
{
    *x = g_ChassisStatus_t.x;   
    *y = g_ChassisStatus_t.y;
    *a = g_ChassisStatus_t.theta;
}

/**
  * @brief ��ȡ�ﵽ�趨�ٶ�ʱ �ֵ��ٶ� ��API(��λRPM)
  * @param 
  * @retval
  * 
  */
void Chassis_API_GetWheelV(float *v1,float *v2,float *v3,float *v4)
{
    *v1 = g_ChassisStatus_t.speed_Wheel_1;
    *v2 = g_ChassisStatus_t.speed_Wheel_2;
    *v3 = g_ChassisStatus_t.speed_Wheel_3;
    *v4 = g_ChassisStatus_t.speed_Wheel_4;
}
/**
  * @brief ��ȡ���͵��ٶ�API(��λRPM)
  * @param �ⲿ��ÿ���ֵ��ٶȵ�ָ��
  * @retval 
  * 
  */

#if USE_STREER  //������
/**
  * @brief ��ȡ�ﵽ�趨λ�� ��API(��λRPM)
  * @param 
  * @retval
  * 
  */
void Chassis_API_getPosition(float *p1,float *p2,float *p3,float *p4)
{
    *p1 = g_ChassisStatus_t.position_Wheel_1;
    *p2 = g_ChassisStatus_t.position_Wheel_2;
    *p3 = g_ChassisStatus_t.position_Wheel_3;
    *p4 = g_ChassisStatus_t.position_Wheel_4;
}
// 2����
void  Chassis_API_Streer_To_Rotate(void)
{
    g_ChassisStatus_t.position_Wheel_1 = STREER_DIR_RATION * 90;
    g_ChassisStatus_t.position_Wheel_2 = STREER_DIR_RATION * 180;
    g_ChassisStatus_t.position_Wheel_3 = STREER_DIR_RATION * -90;
    g_ChassisStatus_t.position_Wheel_4 = STREER_DIR_RATION * 180;
    
    g_ChassisConfig_t.chassisMode = CHASSIS_MODE_STREER_ROTATE;
}
// ��ת
void  Chassis_API_Streer_Rotate(void)
{
    
    g_ChassisStatus_t.speed_Wheel_1 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_2 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_3 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
    g_ChassisStatus_t.speed_Wheel_4 = g_ChassisStatus_t.speed_a  * g_ChassisConfig_t.omegaRatio;
}

//  2ң��
void  Chassis_API_Streer_To_Romate(void)
{
    g_ChassisStatus_t.position_Wheel_1 = 0;
    g_ChassisStatus_t.position_Wheel_2 = 0;
    g_ChassisStatus_t.position_Wheel_3 = 0;
    g_ChassisStatus_t.position_Wheel_4 = 0;
    
    g_ChassisConfig_t.chassisMode = CHASSIS_MODE_ROMOTE;
}

void  Chassis_API_Streer_Mark_Zero(int32_t* p)
{
    ;
}

#endif

float Chassis_API_getMaxSpeed(void)
    
{
    return g_ChassisConfig_t.maxAxisSpeed;   
}

/**
  * @brief ��ȡ�������õ�API
  * @param None
  * @retval ���̵����õĽṹ��ָ��
  * 
  */


pTYPE_ChassisConfig_t Chassis_API_GetConfig(void)
{ 
    return &g_ChassisConfig_t;
}
/**
  * @brief ��ȡ����״̬��API
  * @param None
  * @retval ���̵�״̬�Ľṹ��ָ��
  * 
  */

pTYPE_ChassisStatus_t Chassis_API_GetStatus(void)
{    
    return &g_ChassisStatus_t;
}

/*
a demo,

void Chassis_Test(void)
{
    Chassis_API_Init();

    tim_1ms_task:
        Chassis_API_AxisSpeedSoften_Acclimit_Task_1ms();

    Chassis_API_setV(0, 0, 0); 
    Chassis_API_CalcWheelSpeed();
    Chassis_API_GetWheelV(*v1, *v2, *v3, *v4);
    
    
    
}

*/



















