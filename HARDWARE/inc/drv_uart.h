#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

//#define RECIVE

void UART3_Init(void);
void Serial_SendByte(USART_TypeDef *Serialx, uint16_t Data);
void Serial_SendString(USART_TypeDef *Serialx, char *str);
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer );

#endif
