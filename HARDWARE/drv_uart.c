/**
  ******************************************************************************
  * @file   
  * @author  xjc
  * @version v1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
#include "drv_uart.h"

void UART3_Init(void)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	#ifdef RECIVE
	NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3
	
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
  //USART3_RX	  GPIOB.11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  

	#ifdef RECIVE
  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  #endif
	
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate            = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Cmd(USART3, DISABLE);                    //ʹ�ܴ���3 
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	#ifdef RECIVE
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	#endif
	
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
}

void Serial_SendByte(USART_TypeDef *Serialx, uint16_t Data)
{
	while(USART_GetFlagStatus(Serialx, USART_FLAG_TC) != SET);
	USART_SendData(Serialx, Data);	
}

void Serial_SendString(USART_TypeDef *Serialx, char *str)
{
	uint32_t Index = 0;
	while(str[Index] != '\0')
	{
		Serial_SendByte(Serialx, str[Index]);
		Index++;
	}
}

/*-----------------------
				��ҫ�Ƽ�
-----------------------*/
/**
  * @brief :���ڷ�������
  * @param :
  *			@TxBuffer:���������׵�ַ
  *			@Length:���ݳ���
  * @note  :��
  * @retval:��
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( USART3, USART_FLAG_TXE ));
		USART3->DR = *TxBuffer;
		TxBuffer++;
	}
}

/**
  * @brief :���ڽ�������
  * @param :
  *			@RxBuffer:���������׵�ַ
  * @note  :��
  * @retval:���յ����ֽڸ���
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )			//�ȴ���ѯ��������
	{
		if( RESET != USART_GetFlagStatus( USART3, USART_FLAG_RXNE ))
		{
			*RxBuffer = (uint8_t)USART3->DR;
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;	//���յ�һ���ַ����ظ��ȴ�ʱ��
		}
		if( 100 == l_RxLength )
		{
			break;		//���ܳ���100���ֽ�
		}
	}
	
	return l_RxLength;					//�ȴ���ʱ�����ݽ������
}

