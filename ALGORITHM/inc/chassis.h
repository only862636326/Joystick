#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "user_math.h"
/*****
�ƶ��ĵ�λͳһΪmm
�����ת�ٵ�λΪRPM
���ٶȵ�λΪ ���� / s
*******/
#define RPM_RATION           2292.21318f  //�����ٶ�Ϊ1 m/s ʱ��rpm  
                                          //1 / (pi * 2 * r) * (60 * 19) * 1000
#define STREER_DIR_RATION    25486.22222f //���ַ���תһ�ȵ�position 8192*80*14/360
#define OMEGA_RATION         100.0f

#define MAX_AXIS_SPEED        5000.0f  //(mm / s)
#define ACC_AXIS              500.0f  //(mm / s^2)
#define MAX_OMAGA_SPEED       500.0f    // d/s             
#define ACC_OMAGA             5000     // d/s^2

#define CHASSIS_R             535.0925f //mm
#define ACC_LIMIT             AccLimit_Axis
#define USE_STREER             0
#define MAX_C610_POSITON       3456000           //  4 * 36 * 8192 * 3
#define DEFAULT_CHASSIS_WHEEL  CHASSIS_WHEEL_4   // 


#define CHASSIS_LOCK_A_KP         2.0f
#define CHASSIS_LOCK_A_DEADBAND   0.5f
#define CHASSIS_LOCK_X_KP         4.0f
#define CHASSIS_LOCK_X_DEADBAND   2.0f


typedef enum
{
    CHASSIS_WHEEL_3 = 0x03,
    CHASSIS_WHEEL_4 = 0x04,
    CHASSIS_WHEEL_8 = 0x08,
}TYPE_ChassisWheelNum;

typedef enum
{
    CHASSIS_MODE_DISNABEL = 0x00,
    CHASSIS_MODE_ROMOTE   = 0x01,
    CHASSIS_MODE_DEBUG    = 0x02,
    CHASSIS_MODE_AUTO     = 0x03,
    CHASSIS_MODE_LOCK_1   = 0x04,
    CHASSIS_MODE_LOCK_2   = 0x05,
    
    CHASSIS_MODE_STREER_ROTATE = 0x06,
    CHASSIS_MODE_STREER_MARK   = 0x07,
    
}TYPE_ChassisMode;

typedef enum
{
    AccLimit_None         = 0x00,
    AccLimit_Axis         = 0x01,
}TYPE_AccLimit;

typedef struct TYPE_ChassisConfig_t
{
    float wit, len, r;   //
    float maxAxisSpeed;    //������ٶ�mm/s
    float maxOmagaSpeed;   //�����ٶ�
    
    float axisRatio;      //���ٶȱ���    
    float omegaRatio;     //ȫ���ֽ��ٶȽ������
    
    float dirRatio;       //���ַ���������
    float turnRatio;       //����ת�����
    
    float accLimit;
    float omagaAccLimit;
    
    TYPE_AccLimit        accLimitStatu;
    TYPE_ChassisMode     chassisMode;
    TYPE_ChassisWheelNum wheelNum; //��������
}TYPE_ChassisConfig_t, *pTYPE_ChassisConfig_t;

typedef struct TYPE_Chassis_Status_t
{
    //��λ: mm / s
        
    //��ǰҪ�ﵽ��ֵ, �ữ���
    float speed_x;
    float speed_y;
    float speed_a;
    
    //�趨��ֵ
    float refSpeed_x;
    float refSpeed_y;
    float refSpeed_a;
    
    float lock_a, lock_x, lock_y;
  
    //�ο�rpm ʵ��Ҫ�ﵽ��RPM
    float speed_Wheel_1;
    float speed_Wheel_2;
    float speed_Wheel_3;
    float speed_Wheel_4;
    
#if USE_STREER

    float zero_Position_Wheel_1;
    float zero_Position_Wheel_2;
    float zero_Position_Wheel_3;
    float zero_Position_Wheel_4;
               
    float position_Wheel_1;
    float position_Wheel_2;
    float position_Wheel_3;
    float position_Wheel_4;
#endif
    //��������
    float x;
    float y;
    //���̽� -180, 180
    float theta;
}TYPE_ChassisStatus_t, *pTYPE_ChassisStatus_t;

/*    Extern Global Variables    */
//�ⲿ��ֱ��ʹ��, Ҳ��ʹ��API���ָ��
extern TYPE_ChassisConfig_t  g_ChassisConfig_t;
extern TYPE_ChassisStatus_t  g_ChassisStatus_t;

void  Chassis_API_CalcWheelSpeed(void)         ;

void  Chassis_API_StatusReset(void)           ;
void  Chassis_API_Init(void)                  ;
void  Chassis_API_AxisSpeedSoften_Acclimit_Task_1ms(void)  ;

void  Chassis_API_Angle_Lock(float kp, float deadBand);
void  Chassis_API_Coordinate_Lock(float kp, float deadBand);
void  Chassis_API_Coordinate_Tranform(float *x, float *y);

void  Chassis_API_setV    (float x, float y, float a, uint8_t lock_flag);
void  Chassis_API_setSta_XYA  (float x, float y, float a);
void  Chassis_API_getSta_XYA  (float *x, float *y, float *a);
void  Chassis_API_GetWheelV   (float *v1,float *v2,float *v3,float *v4);

#if USE_STREER
void  Chassis_API_getPosition   (float *p1,float *p2,float *p3,float *p4);
void  Chassis_API_Streer_To_Rotate(void);
void  Chassis_API_Streer_Rotate(void);
void  Chassis_API_Streer_Mark_Zero(int32_t* p);
#endif

float Chassis_API_getMaxSpeed (void);

// ��ȡ������ָ��
pTYPE_ChassisConfig_t Chassis_API_GetConfig(void);
pTYPE_ChassisStatus_t Chassis_API_GetStatus(void);
#endif



















