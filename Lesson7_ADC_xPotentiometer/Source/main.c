
/*
	FILE NAME: main.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "int_usart.h"
#include "systick.h"
#include "gpio.h"
#include "adc.h"

#define TEST_TX_STRING		("ADC Demo!\n")
#define TEST_END_LINE		("\n")

#define ADC_POTENTIOMETER	(ADC_CHANNEL_0)


void MCUInit(void)
{
	GPIOInit();
	IntUSARTInit();
	ADCInit();
	SysTickInit();
}

void DecByte2Ascii(unsigned char val, char *string)
{
	unsigned char temp;
	
	temp = val/10;
	*string++ = temp + '0';
	
	temp = val%10;
	*string++ = temp + '0';	
}

void HexByte2Ascii(unsigned char val, char *string)
{
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

#define ADC_VAL_GND			(0)
#define ADC_VAL_MAX_V		(0xfff) //maximum result

#define ADC_VOLT_SCALE		(100)
#define ADC_VOLT_GND		(0)
#define ADC_VOLT_MAX_V		(3.0*ADC_VOLT_SCALE)
unsigned char CalculateVoltage(unsigned int ADCValue, char * TxBuffers)
{
	unsigned int ADCVolt;
	unsigned char len=0;
	char *strptr = TxBuffers;
	
	if(ADCValue >= ADC_VAL_MAX_V)
		ADCValue = ADC_VAL_MAX_V;
	
	ADCVolt = (ADCValue*ADC_VOLT_MAX_V)/ADC_VAL_MAX_V;
	
	HexByte2Ascii(ADCVolt/ADC_VOLT_SCALE, strptr);
	strptr += 2;
	*strptr++ = '.';
	len += 3;
	DecByte2Ascii(ADCVolt%ADC_VOLT_SCALE, strptr);
	strptr += 2;
	*strptr++ = 'V';
	len += 3;
	
	return len;
}

#define SIZE_RX_BUFFER		(10)
int main(void)
{
	char TxBuffers[10];
	unsigned char len;
	unsigned int ADCValue;
	static unsigned int PreviousTick=0, CurrentTick;
	
	MCUInit();

	USART1_Tx((unsigned char *)TEST_TX_STRING, sizeof(TEST_TX_STRING)-1);
	
	while(1)
	{		
		CurrentTick = SysTickGet();
		if(CurrentTick != PreviousTick)		//execute when next tick happens
		{
			PreviousTick = CurrentTick;
			if(!ADCRead(ADC_POTENTIOMETER, &ADCValue))
			{
				len = CalculateVoltage(ADCValue, TxBuffers);
				USART1_Tx((unsigned char *)TxBuffers, len);
				
				USART1_Tx((unsigned char *)TEST_END_LINE, sizeof(TEST_END_LINE)-1);
			}
		}
	}
}
