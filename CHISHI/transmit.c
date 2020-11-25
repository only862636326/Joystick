/**
  ******************************************************************************
  * @file    transmit.c				
  * @author  jOKERII
  * @version v1.0
  * @date    2019-10-21	
  * @brief   
  ******************************************************************************
  */
#include "transmit.h"

//模块配置参数数组
//改变模块参数，只需改变参数数组值，然后初始化即可
const uint8_t g_ASxx_Param_Config[ 6 ] = { 0xC0, 0x12, 0x34, 0x1A, 0x17, 0xC4 };			//定点模式
const uint8_t  g_ASxx_Config_Status_OK[ ] = { 0x4F, 0x4B, 0x0D, 0x0A };

uint8_t TX_Buffer[20] = {0};

void PackDeal(void)
{
	static uint8_t i = 0;
	
	TX_Buffer[0] = 0xaa;
	TX_Buffer[1] = 0x55;
	
	for(i = 0;i < 7;i++)
	{
		TX_Buffer[2+i*2]   = ADC_Value_G[i] & 0xff;
		TX_Buffer[2+i*2+1] = ADC_Value_G[i] >>  8;
	}
	TX_Buffer[16] = 0;
	TX_Buffer[17] = 0;
	for(i = 0;i < 8 ;i++)
	{
		TX_Buffer[16] |= (KEY_G[15-i]&0x01) << (7  - i );
	}
	for(i = 8;i < 16;i++)
	{
		TX_Buffer[17] |= (KEY_G[15-i]&0x01) << (15 - i);
	}
	
	TX_Buffer[18] = 0x55;
	TX_Buffer[19] = 0xaa;
}

#ifdef UART_COMMUNICATION

void Uart_Transmit(void)
{
	static uint8_t i = 0;
	
	PackDeal();
	
	for(i = 0;i < 20;i++)
	{
		Serial_SendByte(USART1,TX_Buffer[i]);
	}
	GUI_Key_Updata();
	for(i = 0;i < 16;i++)
	{
		KEY_G[i] = 0;                   //按键按下尽置位 所以需要清零
	}

}

#endif

#ifdef UART_DEBUG

void Uart_Debug_Transmit(void)
{
	static uint8_t i = 0;
	for(i = 0;i < 10;i++)
	{
		printf("KEY%d:    %d\r\n",i,KEY_G[i]);
	}
	for(i = 10;i < 16;i++)
	{
		printf("KEY%d:   %d\r\n",i,KEY_G[i]);
	}
	for(i = 0;i < 7;i++)
	{
		printf("ADC%d:   %d\r\n",i,ADC_Value_G[i]);
	}
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	GUI_Key_Updata();
	for(i = 0;i < 16;i++)
	{
		KEY_G[i] = 0;                   //按键按下尽置位 所以需要清零
	}
}

#endif

#ifdef SX1278_COMMUNICATION

/*---------------------------
          泽耀科技
---------------------------*/
/**
  * @brief :ASxx模块初始化
  * @param :无
  * @note  :按照默认参数初始化，修改默认参数表即可改变模块初始化参数
  * @retval:
  *        @ASxx_Write_OK 写入成功
  *        @ASxx_Write_ERROR 写入失败
  */
ASxxWriteStatusType ASxx_param_init( void )
{
	uint8_t i = 0;
	uint8_t Read_Buff[ 5 ] = { 0 };
	
	drv_uart_tx_bytes((uint8_t *)g_ASxx_Param_Config, 6 );
	drv_uart_rx_bytes( Read_Buff );
	
	for( i = 0; i < 4; i++ )
	{
		if( Read_Buff[ i ] != g_ASxx_Config_Status_OK[ i ] )
		{
			break;
		}
	}
	
	if( 4 == i )
	{
		return ASxx_Write_OK;		//配置成功
	}
	else
	{
		return ASxx_Write_ERROR;	//配置失败
	}
	
}

/**
  * @brief :ASxx模块读配置参数
  * @param :
  *         @pReadBuffer：参数返回地址
  * @note  :无
  * @retval:无
  */
void ASxx_read_param( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC1, 0xC1, 0xC1 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
	
}

/**
  * @brief :ASxx模块读取硬件版本号
  * @param :
  *         @pReadBuffer：硬件版本号返回地址
  * @note  :无
  * @retval:无
  */
void ASxx_read_version( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC3, 0xC3, 0xC3 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
}

/**
  * @brief :ASxx模块读取实际电压值
  * @param :
  *         @pReadBuffer：电压值返回地址
  * @note  :无
  * @retval:无
  */
void ASxx_read_voltage( uint8_t *pReadBuffer )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC5, 0xC5, 0xC5 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
	drv_uart_rx_bytes( pReadBuffer );
}

/**
  * @brief :ASxx模块复位
  * @param :无
  * @note  :无
  * @retval:无
  */
void ASxx_reset( void )
{
	uint8_t Read_Cmd[ 3 ] = { 0xC4, 0xC4, 0xC4 };

	drv_uart_tx_bytes( Read_Cmd, 3 );
}

/**
  * @brief :ASxx模块发送数据（定点模式）
  * @param :
  *        @Addr_H：地址高位
  *        @Addr_L：地址低位
  *        @Channel：信道
  *        @pTxBuff：发送数据地址
  *        @Length：发送数据个数
  * @note  :定点模式 数据个数最29个
  * @retval:无
  */
void ASxx_tx_packet( uint8_t Addr_H, uint8_t Addr_L, uint8_t Channel, uint8_t *pTxBuff, uint8_t Length )
{
	uint8_t Header[ 3 ] = { 0 };
	
	Header[ 0 ] = Addr_H;
	Header[ 1 ] = Addr_L;
	Header[ 2 ] = Channel;
	
	drv_uart_tx_bytes( Header, 3 );
	//发送数据
	drv_uart_tx_bytes( pTxBuff, Length );
}

/**
  * @brief :ASxx模块接收数据（定点模式）
  * @param :无
  * @note  :定点模式 数据个数最29个
  * @retval:无
  */
uint8_t ASxx_rx_packet( uint8_t *pRxBuff )
{
	uint8_t Length = 0;
	
	Length = drv_uart_rx_bytes( pRxBuff );
	
	return Length;
}

void SX1278_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能GPIOA时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关键，禁用JTAG 只使用SWD
    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_15 ; //PA.7 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

    GPIO_ResetBits(GPIOA,GPIO_Pin_7);
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);

    UART3_Init();
//	ASxx_param_init();
}

void SX1278_Transmit(void)
{
	static uint8_t i = 0;
	
	PackDeal();
	
//	for(i = 0;i < 20;i++)
//	{
//		Serial_SendByte(USART3,TX_Buffer[i]);          
//	}
/*-------------------------------
	已测试 usart3的跳线帽也需要杜邦线交换 无问题
-------------------------------*/

	drv_uart_tx_bytes(TX_Buffer, 20 );			//
	
	GUI_Key_Updata();
	for(i = 0;i < 16;i++)
	{
		KEY_G[i] = 0;                   //按键按下尽置位 所以需要清零
	}
}


#endif

