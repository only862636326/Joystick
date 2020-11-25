#ifndef _USER_MATH_H
#define _USER_MATH_H

#include "stm32f10x.h"

void absLimit(float* Val, float Limit);
void absLimit_int16(int16_t * Val, int16_t Limit);
void absLimit_int32(int32_t * Val, int32_t Limit);
float Min_f(float x, float y);
float Max_f(float x, float y);
float MinAbs_f(float x, float y);
float MaxAbs_f(float x, float y);


uint8_t ValueInRange_u(uint32_t Value, uint32_t Min, uint32_t Max);
uint8_t ValueInRange_i(int32_t Value, int32_t Min, int32_t Max);
uint8_t ValueInRange_f(float Value, float Min, float Max);
float   FlexibelValue(float dstVal, float srcVal, float step);
extern int16_t FlexibelValue_int16(int16_t dstVal, int16_t srcVal, int16_t step);
float point_distence_f(float x1, float y1, float x2, float y2);

float InvSqrt(float Val);

#endif
