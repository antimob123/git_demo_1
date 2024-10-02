#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#define USART1_MAX_RECV_LEN  64
extern u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN];
void STM32F103_UART1_Init(uint32_t BaudRate);
void SendDataToUART1(uint8_t *Data,uint8_t Len);
void ParseUSART1Buffer(void);
void USART1_SendChar(uint8_t c);
#endif
