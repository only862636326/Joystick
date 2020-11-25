#ifndef __PATH_PLAN__
#define __PATH_PLAN__

#include "POINTS.h"
#include "pid.h"
#include "user_math.h"
#include "math.h"
#include "drv_uart.h"


#define PATH_PLAN_A_RATIO           2.0f
#define PATH_PLAN_AXIS_RATIO        2.1f
#define PATH_PLAN_MODE1_RATO        0.4f
#define PATH_PLAN_DEAD_BAND         25.0f
#define PATH_PLAN_DEAD_BAND_THETA   0.5f
#define PATH_PLAN_DEAD_BAND_P       5.0f
//相应函数重定义,以便更改
#define pathPlan_printf my_printf
#define pathPlan_cos    cosf
#define pathPlan_sin    sinf
#define pathPlan_sqrt   sqrtf 

typedef enum 
{
    PATH_PLAN_MODE_1, // 自动跑点
    PATH_PLAN_MODE_2, // 自动到定点
}ENUM_PathPlanMode;

typedef struct TYPE_PathPlan_Config_t
{
    float dead_Band;        //死区
    float dead_Band_xx_2;   //死区平方
    float dead_Band_Theta;  //角度死区
    
    float dead_Band_p;
    float dead_Band_p_xx_2;
    
    ENUM_PathPlanMode mode_enum;
    float a_ratio;          //调角角度的KP
    float axis_ratio;       //速度的KP
}TYPE_PathPlan_Config_t, *pTYPE_PathPlan_Config_t;

typedef struct TYPE_PathPlan_ChassisInfo_t
{
    float x; 
    float y;       //当前位置
    float theta;            //底盘角度    
}TYPE_PathPlan_ChassisInfo_t, pTYPE_PathPlan_ChassisInfo_t;
typedef struct TYPE_PathPlan_Chassis_V
{
    float x;
    float y;
    float a;
}TYPE_PathPlan_Chassis_V,*pTYPE_PathPlan_Chassis_V;

typedef struct TYPE_Path_Plan_t
{
    TYPE_PathPlan_Config_t      config_t;        //设置
    pTYPE_Path_Info_t           pPlanInfo;       //路经信息指针
    
    uint16_t                   prePointNum;     //运行到第几点
    TYPE_PathPlan_ChassisInfo_t chassisInfo_t;   //底盘信息
    pTYPE_Point_Info_t          pPrePoint;       //当前点指针

    TYPE_PathPlan_Chassis_V     planV_t;         //规化的速度
}TYPE_Path_Plan_t, *pTYPE_Path_Plan_t;           


void PathPlan_SetChassisInfo    (pTYPE_Path_Plan_t pPlan_t, float x, float y, float theta);
void PathPlan_SetDeadBand       (pTYPE_Path_Plan_t pPlan_t, float dead_Band, float dead_Band_Theta);
void PathPlan_GetV_XYA          (pTYPE_Path_Plan_t pPlan_t, float *x, float *y, float *A);

uint8_t PathPlan_Run_One_Time   (pTYPE_Path_Plan_t pPlan_t);
void PathPlan_Cla_Theta         (pTYPE_Path_Plan_t pPlan_t);
void PathPlan_Cla_Plan_V        (pTYPE_Path_Plan_t pPlan_t);

void PathPlan_Init              (pTYPE_Path_Plan_t pPlan_t, pTYPE_Path_Info_t pPathInfo, ENUM_PathPlanMode mode);
void PathPlan_Point_Print       (pTYPE_Path_Plan_t pPlan_t, uint16_t num);

uint8_t Is_Position_In_Dead_Band_2(pTYPE_Path_Plan_t pPlan_t);
uint8_t Is_Position_In_Dead_Band_1(pTYPE_Path_Plan_t pPlan_t); 

#endif



