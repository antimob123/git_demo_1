#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"

static uint16_t	 gUart1ReceStartPtr;
//static uint16_t  gUart1ReceEndPtr;

void STM32F103_UART1_Init(uint32_t BaudRate)      //其他芯片添加新函数         
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	USART_Cmd(USART1, ENABLE);
}

//

void SendDataToUART1(uint8_t *Data,uint8_t Len)  //测试可用
{
	uint8_t i = 0;
	
	for(;i < Len;i++)
	{
		USART_SendData(USART1, Data[i]);

		while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC))
		{
		}
	}
		
}
//

void USART1_SendChar(uint8_t c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //等待的是发送数据寄存器空闲
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    //循环等待整个数据帧发送完毕
}


void USART1_IRQHandler(void)    //测试可用
{
    u8 recvbyte;	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {		
		recvbyte = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        gUSART1_RxBuf[gUart1ReceStartPtr] = recvbyte;
        
		if ((uint16_t)gUart1ReceStartPtr >= (uint16_t)(USART1_MAX_RECV_LEN-1))
        {
            gUart1ReceStartPtr = 0;
        }
        else
        {
            gUart1ReceStartPtr++;
        }
    }			
}
//


void ParseUSART1Buffer(void)    // 环形buff解析思路 可以根据数据刷新的时间来  调整调用间隔
{
    uint16_t i = 0;  // 定义一个遍历数组的索引

    while (i < USART1_MAX_RECV_LEN - 1)  // 保证有足够的字节读取（防止指针越界）
    {
        if (gUSART1_RxBuf[i] == 0xA5 && gUSART1_RxBuf[(i + 1) % USART1_MAX_RECV_LEN] == 0x5A)   // 检查是否找到包头 A5 5A
        {
            uint16_t thirdByteIndex = (i + 3) % USART1_MAX_RECV_LEN;         // 计算第三个字节的索引位置，处理跨越缓冲区边界的情况
            
            // 读取第三个字节的值  根据需求调整
            volatile uint8_t thirdByteValue = gUSART1_RxBuf[thirdByteIndex];  // 在这里可以处理第三个字节的值或者进行其他操作

            // 更新索引，跳过已处理的字节，继续寻找下一个包头
            i = (i + 4) % USART1_MAX_RECV_LEN;  // 根据需要解析的长度来调整步长
        }
        else
        {
            i = (i + 1) % USART1_MAX_RECV_LEN;    // 如果未找到包头，继续查找下一个字节
        }
    }

    gUart1ReceStartPtr = 0;             // 解析完成后，将指针 gUart1ReceStartPtr 重置为 0x00
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}


