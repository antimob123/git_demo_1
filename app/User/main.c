#include "stm32f10x.h"                  // Device header
#include "app.h"
#include "OLED.h"
#include "Serial.h"
#include "boot.h"
//#include "Key.h"

volatile uint8_t i = 0x00;

///////////////////////////////////////////////串口配置部分///////////////////////////////////////////

u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN] = {0};         //USART1_MAX_RECV_LEN 去.h文件去填写 buff 长度
typedef void (*UART_InitFunc)(uint32_t BaudRate);  
UART_InitFunc UART1_init = STM32F103_UART1_Init;     //更换芯片更改这里 例:STM32F103_UART1_Init

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
	SysTick_Init();
	OLED_Init();
    UART1_init(115200);

	while (1)
	{	
     Task_run();
////	 if (1==1)
////    {
//	    #ifdef DEBUG
//		for(int i=0;i<999999;i++ )	   {	
//	   }		
//		printf("app_run\r\n");
//		for(int i=0;i<999999;i++ )	   {	
//	   }
//        #endif	
//        JumpToBootloader();      //跳转到 Bootloader
		printf("ERRO\r\n");        //报错
//    }	
    }
		
}	

