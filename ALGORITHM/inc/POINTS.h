#ifndef __POTINTS_H_
#define __POTINTS_H_
#include "stm32f10x.h"
typedef struct TYPE_Point_Info_t
{
    float x;       //目标位置X
    float y;       //目标位置Y
    float v;       //运行速度
    float theta;   //角度
    float vx;      //X方向速度,可以由以上据计算得,但事先计算好以减少计算
    float vy;      //y方向速度,可以由以上据计算得,但事先计算好以减少计算
}TYPE_Point_Info_t, *pTYPE_Point_Info_t;

typedef struct TYPE_Points_Info_t
{
    
    pTYPE_Point_Info_t  pPoints;   //指向所有点的头结构体的头指针
    uint16_t            point_num; //点的个数
    uint8_t*            path_name;
}TYPE_Path_Info_t, *pTYPE_Path_Info_t;


extern TYPE_Path_Info_t g_Path1_Info_t;
extern TYPE_Path_Info_t g_Path2_Info_t;
extern TYPE_Path_Info_t g_Path3_Info_t;
extern TYPE_Path_Info_t g_Path4_Info_t;

#endif






