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
* @brief 定时器4初始化函数ADC扫描10ms
**********************************************************************************
利用定时器TIM4的通道4的输出比较功能，给ADC1提供采样触发信号，这里触发周期为：100ms
触发周期计算公式为：psc/72*period/1000000（单位为秒）

主函数初始化代码为：
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
	
	u16 period 	= 1000;										  //设置周期值10ms发送一次
	u16 pluse 	= 1000;											//设置CRR值，这里的值可以随便设置，不影响ADC采样周期，但要保证在正确的范围内

	//使能TIM4的时钟，TIM4是挂在APB1总线上的，注意TIM4的时钟是72M的
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_timeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;      	//分频因子，输出给定时器的ETRP数字滤波器提供时钟
	TIM_timeBaseStucture.TIM_Prescaler = 720-1;                		//预分频，给TIMx_CNT驱动的时钟120
	TIM_timeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_timeBaseStucture.TIM_Period = period;						//设置周期，给ARR赋值
	TIM_TimeBaseInit(TIM4,&TIM_timeBaseStucture);
	
	//配置TIM4通道4的输出比较
	TIM_ocInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM输出模式为PWM1
	TIM_ocInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//设置有效电平的极性
	TIM_ocInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能通道输出
	TIM_ocInitStructure.TIM_Pulse = pluse;							//设置PWM的脉冲宽度值，即CRR值
	TIM_OC4Init(TIM4,&TIM_ocInitStructure);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);								//使能TIM4的寄存器ARR的预装载功能，DISABLE时将会使改变ARR值时立即生效
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);				//使能TIM4通道1的CCR的预装载功能，DISABLE时将会使改变CRR值时立即生效
	
	//使能TIM4定时器
	TIM_Cmd(TIM4,ENABLE);
}

/**
* @brief 定时器2初始化函数1ms中断一次，TIM2的

*
* @param none
* @return none
*/
void TIM2_Init(void)
{
    u16 arr = 7199; //自动重装载值
    u16 psc = 9;    //预分频值
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //定时器2时钟使能

    /*定时器2初始化*/
    TIM_TimeBaseStructure.TIM_Period = arr;              //设置重载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;           //设置分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //递增计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //将结构参数用于初始化定时器2

    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE );           //使能定时器2更新中断

    /*设置中断参数*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            //定时器2中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断通道
    NVIC_Init(&NVIC_InitStructure);                            //初始化中断
    TIM_Cmd(TIM2, ENABLE);  //使能定时器
}

/**
* @brief 定时器3初始化函数30ms中断一次，TIM3的

*
* @param none
* @return none
*/
// 已用做PWM
void TIM3_Init(void)
{
    u16 arr = 7199;  //自动重装载值
    u16 psc = 299;    //预分频值
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //定时器3时钟使能

    /*定时器2初始化*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置重载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //递增计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             //将结构参数用于初始化定时器3

    TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE );                  //使能定时器3更新中断

    /*设置中断参数*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //定时器3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断通道
    NVIC_Init(&NVIC_InitStructure);                             //初始化中断
    TIM_Cmd(TIM3, ENABLE);  //使能定时器
}

/**
* @brief 定时器3初始化函数30ms中断一次，TIM3的

*
* @param none
* @return none
*/
void TIM6_Init(void)
{
    u16 arr = 7199;  //自动重装载值
    u16 psc = 299;    //预分频值
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //定时器3时钟使能

    /*定时器2初始化*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置重载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //递增计数模式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             //将结构参数用于初始化定时器3

    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE );                  //使能定时器3更新中断

    /*设置中断参数*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //定时器3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断通道
    NVIC_Init(&NVIC_InitStructure);                             //初始化中断
    TIM_Cmd(TIM6, ENABLE);  //使能定时器
}
	
/**
* @brief 定时器5初始化函数500ms中断一次，TIM5的

*
* @param none
* @return none
*/
void TIM5_Init(void)
{
    u16 arr = 7199;  //自动重装载值
    u16 psc = 4999;  //预分频值
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //定时器5时钟使能

    /*定时器5初始化*/
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置重载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //递增计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);             //将结构参数用于初始化定时器5

    TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE );                  //使能定时器5更新中断

    /*设置中断参数*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;            //定时器5中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断通道
    NVIC_Init(&NVIC_InitStructure);                            //初始化中断
    TIM_Cmd(TIM5, ENABLE);  //使能定时器
}
	
/**
* @brief 定时器2中断函数，检测按键的状态
* @param none
* @return none
*/	
void TIM2_IRQHandler(void)
{	
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   //每隔1ms进入一次更新中断
  {	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      //清除标志位
        Task_1ms();
        keyHandle((keysTypedef_t *)&keys);               //每1ms检测一次按键的状态
	}
}


	
/**
* @brief 定时器3中断函数，串口/sx1278发送
* @param none
* @return none
*/	
void TIM3_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //每隔30ms进入一次更新中断
  {	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除标志位
		Task_30ms();
	}
}
	
/**
* @brief 定时器6中断函数，串口/sx1278发送
* @param none
* @return none
*/	
void TIM6_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)   //每隔30ms进入一次更新中断
  {	
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);      //清除标志位
		Task_30ms();
	}
}

/**
* @brief 定时器5中断函数，
* @param none
* @return none
*/	
void TIM5_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)   //每隔500ms进入一次更新中断
  {	
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);      //清除标志位
		Task_500ms();
	}
}

