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

// uint8_t ADC_CALIBRATOR_OK;  //adc校准标志
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
	GPIO_Init_Structure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;  //7 采样通道
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AIN;  // 模拟输入
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	ADC_DeInit(ADC1);   //ADC1复位 外设寄存器均为缺省值
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );     // 外设基地址为adc1dr寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;                     // 存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			                  	// 数据来自外设	
	DMA_InitStructure.DMA_BufferSize = 7;							            // 缓冲区大小 为数据目的地大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设寄存器唯一地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   	// 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设16位 半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       	// 内存大小同外设数据大小
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						        // 循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // DMA 传输通道优先级高	通道唯一 不影响
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						                  	// 禁止存储器到存储器
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);						                    // 初始化DMA
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);			                		//打开DMA通道1数据传输完成中断
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);							                    //打开NVIC中对应的DMA通道1的中断通道
	
	//初始化ADC1
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;            //独立模式
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                          //7通道扫描采样
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                         //单次采样，由tim4出发采样
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T4_CC4;     //这里不是啥鸡儿程序触发！！！！！！！是tm定时器
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;             //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel       = 7;                               //采样通道数7
	
	//adc时钟分频 注意adc频率不超过14m
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置adc规则通道采样顺序和时间
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
	ADC_ResetCalibration(ADC1);										//重置ADC采样校准器，防止出现较大的误差
	while(ADC_GetResetCalibrationStatus(ADC1));		//等待较准寄存器准备完成
	ADC_StartCalibration(ADC1);										//开启ADC校准
	while(ADC_GetCalibrationStatus(ADC1));				//等待校准成功
	DMA_Cmd(DMA1_Channel1 , ENABLE);                                            //使能dma
	
	//使能外部触发ADC采样
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
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;  //7 采样通道
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;  // 模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	ADC_DeInit(ADC1);   //ADC1复位 外设寄存器均为缺省值
	
	//初始化ADC1
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;   //独立模式
	ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                 //7通道扫描采样
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                //单次采样，由tim4出发采样
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T4_CC4;
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;    //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel       = 1;                      //采样通道数7
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//adc时钟分频 注意adc频率不超过14m
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//配置adc规则通道采样顺序和时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_239Cycles5);	
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_239Cycles5);

	//ADC1 ENABLE
	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);										//重置ADC采样校准器，防止出现较大的误差
	while(ADC_GetResetCalibrationStatus(ADC1));		//等待较准寄存器准备完成
	ADC_StartCalibration(ADC1);										//开启ADC校准
	while(ADC_GetCalibrationStatus(ADC1));				//等待校准成功
	
	//使能外部触发ADC采样
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




//DMA1通道1中断通道处理函数，触发此中断时，说明已经将数据从ADC转移到内存数组中，可以触发数据发送了
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{
        DMA_ClearITPendingBit(DMA1_IT_TC1);		//清除DMA1通道1传输完成中断
        JOY_ADC_Calibration();
		
	}
}


