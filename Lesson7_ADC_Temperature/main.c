
/*
	FILE NAME: main.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "int_usart.h"
#include "systick.h"
#include "adc.h"

#define TEST_TX_STRING		("ADC Demo!\n")
#define TEST_END_LINE		("\n")

void MCUInit(void)
{
	IntUSARTInit();
	ADCInit();
	SysTickInit();
}

void HexByte2Ascii(unsigned char val, char *string)
{
	volatile char debug;
	unsigned char temp;
	
	temp = ((val>>4)&0x0f);
	if(temp > 9)
		*string++ = temp + 'A' - 10;
	else
		*string++ = temp + '0';
	
	temp = ((val>>0)&0x0f);
	if(temp > 9)
		*string++ = temp + 'A' - 10;
	else
		*string++ = temp + '0';	
}

void HexInt2Ascii(unsigned int val, char *string)
{
	HexByte2Ascii(val>>24, string);
	string += 2;
	HexByte2Ascii(val>>16, string);
	string += 2;
	HexByte2Ascii(val>>8, string);
	string += 2;
	HexByte2Ascii(val>>0, string);	
}

#define SIZE_RX_BUFFER		(10)
int main(void)
{
//	unsigned int ReadLen;
//	unsigned char RxBuffers[SIZE_RX_BUFFER];
	char TxBuffers[9]={0,0,0,0,0,0,0,0,'\n'};
	unsigned int ADCValue;
	static unsigned int PreviousTick=0, CurrentTick;
	
	MCUInit();

	USART1_Tx((unsigned char *)TEST_TX_STRING, sizeof(TEST_TX_STRING)-1);
	
	while(1)
	{		
		CurrentTick = SysTickGet();
		if(CurrentTick != PreviousTick)		//execute during odd seconds
		{
			PreviousTick = CurrentTick;
			if(!ADCRead(ADC_CHANNEL_16_TEMPERATURE, &ADCValue))
			{
				HexInt2Ascii(ADCValue, TxBuffers);
				USART1_Tx((unsigned char *)TxBuffers, sizeof(TxBuffers));
				
				USART1_Tx((unsigned char *)TEST_END_LINE, sizeof(TEST_END_LINE)-1);
			}
		}
	}
}
