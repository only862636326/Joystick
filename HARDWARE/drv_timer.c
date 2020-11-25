/**
  ******************************************************************************
  * @file   
  * @author  xjc
  * @version v1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "drv_timer.h"

/**
* @brief ��ʱ��4��ʼ������ADCɨ��10ms
**********************************************************************************
���ö�ʱ��TIM4��ͨ��4������ȽϹ��ܣ���ADC1�ṩ���������źţ����ﴥ������Ϊ��100ms
�������ڼ��㹫ʽΪ��psc/72*period/1000000����λΪ�룩

��������ʼ������Ϊ��
	TIM_Init();
**********************************************************************************

*
* @param none
* @return none
*/
void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_timeBaseStucture;
	TIM_OCInitTypeDef TIM_ocInitStructure;
	
	u16 period 	= 1000;										  //��������ֵ10ms����һ��
	u16 pluse 	= 1000;											//����CRRֵ�������ֵ����������ã���Ӱ��ADC�������ڣ���Ҫ��֤����ȷ�ķ�Χ��

	//ʹ��TIM4��ʱ�ӣ�TIM4�ǹ���APB1�����ϵģ�ע��TIM4��ʱ����72M��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_timeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;      	//��Ƶ���ӣ��������ʱ����ETRP�����˲����ṩʱ��
	TIM_timeBaseStucture.TIM_Prescaler = 720-1;                		//Ԥ��Ƶ����TIMx_CNT������ʱ��120
	TIM_timeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_timeBaseStucture.TIM_Period = period;						//�������ڣ���ARR��ֵ
	TIM_TimeBaseInit(TIM4,&TIM_timeBaseStucture);
	
	//����TIM4ͨ��4������Ƚ�
	TIM_ocInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM���ģʽΪPWM1
	TIM_ocInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//������Ч��ƽ�ļ���
	TIM_ocInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ��ͨ�����
	TIM_ocInitStructure.TIM_Pulse = pluse;							//����PWM��������ֵ����CRRֵ
	TIM_OC4Init(TIM4,&TIM_ocInitStructure);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);								//ʹ��TIM4�ļĴ���ARR��Ԥװ�ع��ܣ�DISABLEʱ����ʹ�ı�ARRֵʱ������Ч
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);				//ʹ��TIM4ͨ��1��CCR��Ԥװ�ع��ܣ�DISABLEʱ����ʹ�ı�CRRֵʱ������Ч
	
	//ʹ��TIM4��ʱ��
	TIM_Cmd(TIM4,ENABLE);
}

/**
* @brief ��ʱ��2��ʼ������1ms�ж�һ�Σ�TIM2��

*
* @param none
* @return none
*/
void TIM2_Init(void)
{
    u16 arr = 7199; //�Զ���װ��ֵ
    u16 psc = 9;    //Ԥ��Ƶֵ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʱ��2ʱ��ʹ��

    /*��ʱ��2��ʼ��*/
    TIM_TimeBaseStructure.TIM_Period = arr;              //��������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;           //���÷�Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //���÷�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //��������ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //���ṹ�������ڳ�ʼ����ʱ��2

    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE );           //ʹ�ܶ�ʱ��2�����ж�

    /*�����жϲ���*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            //��ʱ��2�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                            //��ʼ���ж�
    TIM_Cmd(TIM2, ENABLE);  //ʹ�ܶ�ʱ��
}

/**
* @brief ��ʱ��3��ʼ������30ms�ж�һ�Σ�TIM3��

*
* @param none
* @return none
*/
// ������PWM
void TIM3_Init(void)
{
    u16 arr = 7199;  //�Զ���װ��ֵ
    u16 psc = 299;    //Ԥ��Ƶֵ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʱ��3ʱ��ʹ��

    /*��ʱ��2��ʼ��*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //���÷�Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //���÷�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //��������ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //���ṹ�������ڳ�ʼ����ʱ��3

    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE );                  //ʹ�ܶ�ʱ��3�����ж�

    /*�����жϲ���*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //��ʱ��3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ���ж�
    TIM_Cmd(TIM3, ENABLE);  //ʹ�ܶ�ʱ��
}

/**
* @brief ��ʱ��3��ʼ������30ms�ж�һ�Σ�TIM3��

*
* @param none
* @return none
*/
void TIM6_Init(void)
{
    u16 arr = 7199;  //�Զ���װ��ֵ
    u16 psc = 299;    //Ԥ��Ƶֵ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //��ʱ��3ʱ��ʹ��

    /*��ʱ��2��ʼ��*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //���÷�Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //���÷�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //��������ģʽ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             //���ṹ�������ڳ�ʼ����ʱ��3

    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE );                  //ʹ�ܶ�ʱ��3�����ж�

    /*�����жϲ���*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //��ʱ��3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                             //��ʼ���ж�
    TIM_Cmd(TIM6, ENABLE);  //ʹ�ܶ�ʱ��
}
	
/**
* @brief ��ʱ��5��ʼ������500ms�ж�һ�Σ�TIM5��

*
* @param none
* @return none
*/
void TIM5_Init(void)
{
    u16 arr = 7199;  //�Զ���װ��ֵ
    u16 psc = 4999;  //Ԥ��Ƶֵ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //��ʱ��5ʱ��ʹ��

    /*��ʱ��5��ʼ��*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //���÷�Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //���÷�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //��������ģʽ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);             //���ṹ�������ڳ�ʼ����ʱ��5

    TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE );                  //ʹ�ܶ�ʱ��5�����ж�

    /*�����жϲ���*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;            //��ʱ��5�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //ʹ���ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                            //��ʼ���ж�
    TIM_Cmd(TIM5, ENABLE);  //ʹ�ܶ�ʱ��
}
	
/**
* @brief ��ʱ��2�жϺ�������ⰴ����״̬
* @param none
* @return none
*/	
void TIM2_IRQHandler(void)
{	
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   //ÿ��1ms����һ�θ����ж�
  {	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      //�����־λ
        Task_1ms();
        keyHandle((keysTypedef_t *)&keys);               //ÿ1ms���һ�ΰ�����״̬
	}
}


	
/**
* @brief ��ʱ��3�жϺ���������/sx1278����
* @param none
* @return none
*/	
void TIM3_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //ÿ��30ms����һ�θ����ж�
  {	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //�����־λ
		Task_30ms();
	}
}
	
/**
* @brief ��ʱ��6�жϺ���������/sx1278����
* @param none
* @return none
*/	
void TIM6_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)   //ÿ��30ms����һ�θ����ж�
  {	
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);      //�����־λ
		Task_30ms();
	}
}

/**
* @brief ��ʱ��5�жϺ�����
* @param none
* @return none
*/	
void TIM5_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)   //ÿ��500ms����һ�θ����ж�
  {	
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);      //�����־λ
		Task_500ms();
	}
}

