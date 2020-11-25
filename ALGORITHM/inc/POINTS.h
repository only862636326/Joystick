#ifndef __POTINTS_H_
#define __POTINTS_H_
#include "stm32f10x.h"
typedef struct TYPE_Point_Info_t
{
    float x;       //Ŀ��λ��X
    float y;       //Ŀ��λ��Y
    float v;       //�����ٶ�
    float theta;   //�Ƕ�
    float vx;      //X�����ٶ�,���������Ͼݼ����,�����ȼ�����Լ��ټ���
    float vy;      //y�����ٶ�,���������Ͼݼ����,�����ȼ�����Լ��ټ���
}TYPE_Point_Info_t, *pTYPE_Point_Info_t;

typedef struct TYPE_Points_Info_t
{
    
    pTYPE_Point_Info_t  pPoints;   //ָ�����е��ͷ�ṹ���ͷָ��
    uint16_t            point_num; //��ĸ���
    uint8_t*            path_name;
}TYPE_Path_Info_t, *pTYPE_Path_Info_t;


extern TYPE_Path_Info_t g_Path1_Info_t;
extern TYPE_Path_Info_t g_Path2_Info_t;
extern TYPE_Path_Info_t g_Path3_Info_t;
extern TYPE_Path_Info_t g_Path4_Info_t;

#endif






