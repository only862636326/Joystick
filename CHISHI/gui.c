/**
  ******************************************************************************
  * @file    gui.c				
  * @author  jOKERII
  * @version v1.0
  * @date    2019-09-21	
  * @brief   OLED_Init
						 OLED_ShowNum
						 OLED_ShowString
  ******************************************************************************
  */
#include "gui.h"

void GUI_Init(void)
{
	OLED_Init();
	OLED_Display_On();
	OLED_Clear();
	
	OLED_ShowString(byte(9),line1,"remote",8);
	OLED_ShowString(byte(0),line1,"RF:",8);
	OLED_ShowString(byte(0),line3,"R_X:",6);
	OLED_ShowString(byte(0),line4,"R_Y:",6);
	OLED_ShowString(byte(8),line3,"L_X:",6);
	OLED_ShowString(byte(8),line4,"L_Y:",6);
	OLED_ShowString(0 ,line5,"P1:",6);
	OLED_ShowString(42,line5,"P2:",6);
	OLED_ShowString(85,line5,"P3:",6);
	OLED_ShowString(byte(0),line7,"KEY",6);
}

void GUI_Key_Updata(void)
{
	static uint8_t i = 0;
/*-----------------------------------------------
											°´¼ü
------------------------------------------------*/
	for(i = 0;i < 16;i++)
	{
		if(i < 8)
		{
			OLED_ShowChar(50+10*i,line7,KEY_G[i]+48,6);
		}
		else
		{
			OLED_ShowChar(50+10*(i-8),line8,KEY_G[i]+48,6);
		}
	}
}

void GUI_ADC_Updata(void)
{
/*-----------------------------------------------
											ADC
------------------------------------------------*/
	static char str[4];
	sprintf(str,"%4d",ADC_Value_G[0]);
		OLED_ShowChar  (55,line3,' ',6);
		OLED_ShowString(30,line3,str,6);
	sprintf(str,"%4d",ADC_Value_G[1]);
		OLED_ShowChar  (55,line4,' ',6);
		OLED_ShowString(30,line4,str,6);
	sprintf(str,"%4d",ADC_Value_G[2]);
		OLED_ShowChar  (119,line3,' ',6);
		OLED_ShowString(94,line3,str,6);
	sprintf(str,"%4d",ADC_Value_G[3]);
		OLED_ShowChar  (119,line4,' ',6);
		OLED_ShowString(94,line4,str,6);
	sprintf(str,"%4d",ADC_Value_G[4]);
		OLED_ShowString(0 ,line6,str,6);
	sprintf(str,"%4d",ADC_Value_G[5]);
		OLED_ShowString(42,line6,str,6);
	sprintf(str,"%4d",ADC_Value_G[6]);
		OLED_ShowString(85,line6,str,6);
	
}
