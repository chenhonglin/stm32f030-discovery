#include "stm32f0xx.h"			//include file that define all SFR

#include "int_usart.h"

#define TEST_TX_STRING		("Hello World!\n")

void MCUInit(void)
{
	IntUSARTInit();
}

#define SIZE_RX_BUFFER		(10)
int main(void)
{
	unsigned int ReadLen;
	unsigned char RxBuffers[SIZE_RX_BUFFER];
	
	MCUInit();

	USART1_Tx(TEST_TX_STRING, sizeof(TEST_TX_STRING)-1);
	
	while(1)
	{		
		if(USART1_RxAvailable())	
		{		
			ReadLen	= USART1_Rx(RxBuffers, SIZE_RX_BUFFER);
			USART1_Tx(RxBuffers, ReadLen);			
		}
	}
}
