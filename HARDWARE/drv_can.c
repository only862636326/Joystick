
#include "drv_can.h"

#define MOTO_DJI    0x01
#define MOTO_MODULE 0x02

uint8_t can_use = 0;

void CAN1_Init(void)

{
    GPIO_InitTypeDef       GPIO_InitStructure; 
    NVIC_InitTypeDef       NVIC_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /* 复用功能和GPIOB端口时钟使能*/    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);                                                                      
    
    /* CAN1 模块时钟使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

    /* Configure CAN pin: RX */  // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */   // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //#define GPIO_Remap_CAN    GPIO_Remap1_CAN1
   // GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
   
    // CAN_NVIC_Configuration(); //CAN中断初始化   
    /* Configure the NVIC Preemption Priority Bits */  
  //  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);


    #ifdef  VECT_TAB_RAM  
      /* Set the Vector Table base location at 0x20000000 */ 
      NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
    #else  /* VECT_TAB_FLASH  */
      /* Set the Vector Table base location at 0x08000000 */ 
//      NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
    #endif

    /* enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = USB_LP_CAN1_RX0_IRQn;;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);

    CAN_DeInit(CAN1);                       //将外设CAN的全部寄存器重设为缺省值
    CAN_StructInit(&CAN_InitStructure);     //把CAN_InitStruct中的每一个参数按缺省值填入

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM=DISABLE;         //没有使能时间触发模式
    CAN_InitStructure.CAN_ABOM=DISABLE;         //没有使能自动离线管理
    CAN_InitStructure.CAN_AWUM=DISABLE;         //没有使能自动唤醒模式
    CAN_InitStructure.CAN_NART=DISABLE;         //没有使能非自动重传模式
    CAN_InitStructure.CAN_RFLM=DISABLE;         //没有使能接收FIFO锁定模式
    CAN_InitStructure.CAN_TXFP=ENABLE;         //没有使能发送FIFO优先级
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal; //CAN设置为正常模式

    CAN_InitStructure.CAN_SJW  							 = CAN_SJW_1tq;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1  							 = CAN_BS1_3tq; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2  							 = CAN_BS2_2tq; //Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler 					 = 6;  //分频系数(Fdiv)为brp+1	
    CAN_Init(CAN1,&CAN_InitStructure);          //波特率为：72M / 2 / 6 / (1+3+2)=1 即波特率为1000KBPs
    
    		//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber         	 = 0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode           	 = CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale          	 = CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh         	 = 0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow          	 = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     	 = 0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow      	 = 0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment 	 = 0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation     	 = ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
    /* CAN FIFO0 message pending interrupt enable */ 
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME ,ENABLE);                        //使能FIFO0消息挂号中断
}


/* USB中断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{

    CanRxMsg RxMessage;
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)!= RESET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
		CAN_ClearFlag (CAN1, CAN_IT_FF0);
        CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
        if(can_use == MOTO_DJI)
            C620_GetMotorInfo(&RxMessage);
        else if(can_use == MOTO_MODULE)
            CAN1_RX0_Int(RxMessage);
    }
} 
void Change_Can_Use(uint8_t i)
{
    can_use = i;
}



