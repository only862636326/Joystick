/**
  ******************************************************************************
  * @file    adc.c
  * @author  xjc
  * @version v1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "drv_adc.h"

// uint8_t ADC_CALIBRATOR_OK;  //adcУ׼��־
int16_t  ADC_Value_G[7]    = {0};
uint16_t ADC_Value[7]      = {0};
uint16_t ADC_Calibrator[7] = {0};

#ifdef ADC_DMA

void JOY_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_Init_Structure;
	ADC_InitTypeDef  ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
	//adc&dma clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1,ENABLE);
	
	//GPIO init
	GPIO_Init_Structure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;  //7 ����ͨ��
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AIN;  // ģ������
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	ADC_DeInit(ADC1);   //ADC1��λ ����Ĵ�����Ϊȱʡֵ
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );     // �������ַΪadc1dr�Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;                     // �洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			                  	// ������������	
	DMA_InitStructure.DMA_BufferSize = 7;							            // ��������С Ϊ����Ŀ�ĵش�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // ����Ĵ���Ψһ��ַ���õ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   	// �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ����16λ ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       	// �ڴ��Сͬ�������ݴ�С
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						        // ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMA ����ͨ�����ȼ���	ͨ��Ψһ ��Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						                  	// ��ֹ�洢�����洢��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);						                    // ��ʼ��DMA
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);			                		//��DMAͨ��1���ݴ�������ж�
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);							                    //��NVIC�ж�Ӧ��DMAͨ��1���ж�ͨ��
	
	//��ʼ��ADC1
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;            //����ģʽ
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                          //7ͨ��ɨ�����
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //���β�������tim4��������
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T4_CC4;     //���ﲻ��ɶ�������򴥷�����������������tm��ʱ��
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;             //�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel       = 7;                               //����ͨ����7
	
	//adcʱ�ӷ�Ƶ ע��adcƵ�ʲ�����14m
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//����adc����ͨ������˳���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_239Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_239Cycles5);
	
	//ADC1 ENABLE
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);										//����ADC����У׼������ֹ���ֽϴ�����
	while(ADC_GetResetCalibrationStatus(ADC1));		//�ȴ���׼�Ĵ���׼�����
	ADC_StartCalibration(ADC1);										//����ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�У׼�ɹ�
	DMA_Cmd(DMA1_Channel1 , ENABLE);                                            //ʹ��dma
	
	//ʹ���ⲿ����ADC����
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);	
}
#endif

#ifdef ADC_DERECT

void JOY_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	//adc clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,ENABLE);
		
	//GPIO init
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;  //7 ����ͨ��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;  // ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	ADC_DeInit(ADC1);   //ADC1��λ ����Ĵ�����Ϊȱʡֵ
	
	//��ʼ��ADC1
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;   //����ģʽ
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                 //7ͨ��ɨ�����
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                //���β�������tim4��������
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T4_CC4;
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;    //�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel       = 1;                      //����ͨ����7
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//adcʱ�ӷ�Ƶ ע��adcƵ�ʲ�����14m
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//����adc����ͨ������˳���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_239Cycles5);	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_239Cycles5);

	//ADC1 ENABLE
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);										//����ADC����У׼������ֹ���ֽϴ�����
	while(ADC_GetResetCalibrationStatus(ADC1));		//�ȴ���׼�Ĵ���׼�����
	ADC_StartCalibration(ADC1);										//����ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));				//�ȴ�У׼�ɹ�
	
	//ʹ���ⲿ����ADC����
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);	
}

#endif

void JOY_ADC_Calibration(void)
{
	uint8_t i = 0;
    int16_t temp[7];
	float   max_axis = 2048.0f - JOY_ADC_DEAD_BAN;
    static int16_t filter[7][5] = {0};
    
	//  4096
	for(i = 0;i < 7;i++)
	{
		temp[i] = wave_filter_median(filter[i], filter[i]+5, ADC_Value[i]);
        // temp[i] = ADC_Value[i];
	}
  
    for(i = 0;i < 4;i++)
    {
        if(temp[i] >= 2048 + JOY_ADC_DEAD_BAN)
        {
            temp[i] -= 2048 + JOY_ADC_DEAD_BAN;
            temp[i] = ((float) temp[i]  / max_axis) *
                      ((float) temp[i]  / max_axis) *
                       temp[i] / max_axis * 2048;
                       
            
        }
        else if(temp[i] <= 2048 - JOY_ADC_DEAD_BAN)
        {
            temp[i] -= 2048 - JOY_ADC_DEAD_BAN;
            temp[i] = ((float) temp[i]  / max_axis) *
                      ((float) temp[i]  / max_axis) *
                       temp[i] / max_axis * 2048;
            
        }
        else
        {
            temp[i] = 0;
        }
        temp[i] = -temp[i];
    }
    

    for(i = 0;i < 7;i++)
    {
        ADC_Value_G[i] =temp[i];
        
    }	
}




//DMA1ͨ��1�ж�ͨ�����������������ж�ʱ��˵���Ѿ������ݴ�ADCת�Ƶ��ڴ������У����Դ������ݷ�����
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{
        DMA_ClearITPendingBit(DMA1_IT_TC1);		//���DMA1ͨ��1��������ж�
        JOY_ADC_Calibration();
		
	}
}


