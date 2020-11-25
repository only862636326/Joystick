
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

    /* ���ù��ܺ�GPIOB�˿�ʱ��ʹ��*/    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);                                                                      
    
    /* CAN1 ģ��ʱ��ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

    /* Configure CAN pin: RX */  // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */   // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //#define GPIO_Remap_CAN    GPIO_Remap1_CAN1
   // GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
   
    // CAN_NVIC_Configuration(); //CAN�жϳ�ʼ��   
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

    CAN_DeInit(CAN1);                       //������CAN��ȫ���Ĵ�������Ϊȱʡֵ
    CAN_StructInit(&CAN_InitStructure);     //��CAN_InitStruct�е�ÿһ��������ȱʡֵ����

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM=DISABLE;         //û��ʹ��ʱ�䴥��ģʽ
    CAN_InitStructure.CAN_ABOM=DISABLE;         //û��ʹ���Զ����߹���
    CAN_InitStructure.CAN_AWUM=DISABLE;         //û��ʹ���Զ�����ģʽ
    CAN_InitStructure.CAN_NART=DISABLE;         //û��ʹ�ܷ��Զ��ش�ģʽ
    CAN_InitStructure.CAN_RFLM=DISABLE;         //û��ʹ�ܽ���FIFO����ģʽ
    CAN_InitStructure.CAN_TXFP=ENABLE;         //û��ʹ�ܷ���FIFO���ȼ�
    CAN_InitStructure.CAN_Mode=CAN_Mode_Normal; //CAN����Ϊ����ģʽ

    CAN_InitStructure.CAN_SJW  							 = CAN_SJW_1tq;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1  							 = CAN_BS1_3tq; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2  							 = CAN_BS2_2tq; //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler 					 = 6;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
    CAN_Init(CAN1,&CAN_InitStructure);          //������Ϊ��72M / 2 / 6 / (1+3+2)=1 ��������Ϊ1000KBPs
    
    		//���ù�����
 	CAN_FilterInitStructure.CAN_FilterNumber         	 = 0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode           	 = CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale          	 = CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh         	 = 0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow          	 = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     	 = 0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow      	 = 0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment 	 = 0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation     	 = ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
    /* CAN FIFO0 message pending interrupt enable */ 
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME ,ENABLE);                        //ʹ��FIFO0��Ϣ�Һ��ж�
}


/* USB�жϺ�CAN�����жϷ������USB��CAN����I/O������ֻ�õ�CAN���жϡ� */
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



