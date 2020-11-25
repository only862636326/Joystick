#ifndef __GUI_H
#define __GUI_H

#include "stm32f10x.h"
#include "bsp_oled.h"
#include "bsp_key.h"
#include <stdio.h>
#include "drv_adc.h"


/* OLED��Ļ��7�� */
#define line1 0
#define line2 1
#define line3 2
#define line4 3
#define line5 4
#define line6 5
#define line7 6
#define line8 7
/* OLED��Ļ��128�ĵ�,Ҳ����16�У�16*8�� */
#define byte(data)  data*8

void GUI_Init(void);
void GUI_Key_Updata(void);
void GUI_ADC_Updata(void);

#endif
