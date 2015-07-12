#include "stm32f0xx.h"			//include file that define all SFR

#include "int_usart.h"

#define TEST_TX_STRING		("Hello World!\n")

void MCUInit(void)
{
	IntUSARTInit();
}

int main(void)
{
	unsigned char FlagSend=1;
	
	MCUInit();

	USART1_Tx(TEST_TX_STRING, sizeof(TEST_TX_STRING)-1);
	
	while(1)
	{
		if(FlagSend)
			USART1_Tx(TEST_TX_STRING, sizeof(TEST_TX_STRING)-1);
	}
}
