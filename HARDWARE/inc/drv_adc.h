#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "drv_timer.h"
#include "wave_filter.h"



#define   ADC_DMA           0x02
#define   JOY_ADC_DEAD_BAN  350
extern int16_t ADC_Value_G[7];

void JOY_ADC_Init(void);
void JOY_ADC_Calibration(void);
void JOY_ADC_Pro(void);
	
#endif
