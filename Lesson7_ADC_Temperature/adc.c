
/*
	FILE NAME: adc.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "adc.h"

unsigned char ADCRead(unsigned char channel, unsigned int *value)
{	
	//TODO: find the proper wait_count
	unsigned int wait_count = 10000;
	unsigned char ret=0;

	if(channel >= MAX_ADC_CHANNEL)
		return 1;
	
	if(channel == ADC_CHANNEL_16_TEMPERATURE)
	{
		//enable temperature sensor for ADC measurement
		ADC->CCR |= ADC_CCR_TSEN;
	}
	else if(channel == ADC_CHANNEL_17_INT_REF)
	{
		//enable Vreference for ADC measurement
		ADC->CCR |= ADC_CCR_VREFEN;
	}
	
	//select ADC channel
	ADC1->CHSELR = channel;

	//single conversion mode : 0
	ADC1->CFGR1 &= ~ADC_CFGR1_CONT;

	//12 bit resolution: 00
	ADC1->CFGR1 &= ~ADC_CFGR1_RES;

	//start ADC: 1
	ADC1->CR = ADC_CR_ADSTART;

	//watiting ADC complete
	while(!(ADC1->ISR & ADC_ISR_EOSEQ))
	{
		//provided a timeout for ADC
		wait_count --;
		if(!wait_count)
		{
			ret = 1;
			break;
		}
	}
	//ADC complete, save value
	*value = ADC1->DR;

	if(channel == ADC_CHANNEL_16_TEMPERATURE)
	{
		//disable temperature sensor for ADC measurement
		ADC->CCR &= ~ADC_CCR_TSEN;
	}
	else if(channel == ADC_CHANNEL_17_INT_REF)
	{
		//disable Vreference for ADC measurement
		ADC->CCR &= ~ADC_CCR_VREFEN;
	}
	
	return ret;
}

void ADCInit(void)
{
	//Enable ADC clock
	RCC->APB2ENR |= RCC_APB2RSTR_ADCRST;
	
	//turn off ADC: 0
	ADC1->CR &= ~ADC_CR_ADEN;

	//start ADC calibration: 1
	ADC1->CR |= ADC_CR_ADCAL;

	while(1)
	{
		//wait till calibration process complete: 0
		if((ADC1->CR & ADC_CR_ADCAL) == 0)
			break;
	}
	
	//power up ADC: 1
	ADC1->CR |= ADC_CR_ADEN;
	
	while(1)
	{
		//wait until ADC ready: 1
		if(ADC1->ISR & ADC_ISR_ADRDY)
			break;
	}	
}
