#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "user_math.h"
/*****
移动的单位统一为mm
电机的转速单位为RPM
解速度单位为 弧度 / s
*******/
#define RPM_RATION           2292.21318f  //当轮速度为1 m/s 时的rpm  
                                          //1 / (pi * 2 * r) * (60 * 19) * 1000
#define STREER_DIR_RATION    25486.22222f //舵轮方向转一度的position 8192*80*14/360
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
    float maxAxisSpeed;    //最大轴速度mm/s
    float maxOmagaSpeed;   //最大角速度
    
    float axisRatio;      //轴速度比率    
    float omegaRatio;     //全向轮角速度解算比率
    
    float dirRatio;       //舵轮方向解算比率
    float turnRatio;       //舵轮转弯比率
    
    float accLimit;
    float omagaAccLimit;
    
    TYPE_AccLimit        accLimitStatu;
    TYPE_ChassisMode     chassisMode;
    TYPE_ChassisWheelNum wheelNum; //底盘轮数
}TYPE_ChassisConfig_t, *pTYPE_ChassisConfig_t;

typedef struct TYPE_Chassis_Status_t
{
    //单位: mm / s
        
    //当前要达到的值, 柔化后的
    float speed_x;
    float speed_y;
    float speed_a;
    
    //设定的值
    float refSpeed_x;
    float refSpeed_y;
    float refSpeed_a;
    
    float lock_a, lock_x, lock_y;
  
    //参考rpm 实际要达到的RPM
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
    //底盘坐标
    float x;
    float y;
    //底盘角 -180, 180
    float theta;
}TYPE_ChassisStatus_t, *pTYPE_ChassisStatus_t;

/*    Extern Global Variables    */
//外部可直接使用, 也可使用API获得指针
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

// 获取变量的指针
pTYPE_ChassisConfig_t Chassis_API_GetConfig(void);
pTYPE_ChassisStatus_t Chassis_API_GetStatus(void);
#endif



















