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
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	#ifdef RECIVE
	NVIC_InitTypeDef NVIC_InitStructure;
	#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3
	
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

	#ifdef RECIVE
  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  #endif
	
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate            = 115200;//串口波特率
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Cmd(USART3, DISABLE);                    //使能串口3 
  USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	#ifdef RECIVE
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	#endif
	
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
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
				泽耀科技
-----------------------*/
/**
  * @brief :串口发送数据
  * @param :
  *			@TxBuffer:发送数据首地址
  *			@Length:数据长度
  * @note  :无
  * @retval:无
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
  * @brief :串口接收数据
  * @param :
  *			@RxBuffer:发送数据首地址
  * @note  :无
  * @retval:接收到的字节个数
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )			//等待查询串口数据
	{
		if( RESET != USART_GetFlagStatus( USART3, USART_FLAG_RXNE ))
		{
			*RxBuffer = (uint8_t)USART3->DR;
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;	//接收到一个字符，回复等待时间
		}
		if( 100 == l_RxLength )
		{
			break;		//不能超过100个字节
		}
	}
	
	return l_RxLength;					//等待超时，数据接收完成
}

