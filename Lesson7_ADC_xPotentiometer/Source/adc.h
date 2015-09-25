
/*
	FILE NAME: adc.h
*/

#ifndef _ADC_H_
#define _ADC_H_

enum{
	ADC_CHANNEL_0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_12,
	ADC_CHANNEL_13,
	ADC_CHANNEL_14,
	ADC_CHANNEL_15,
	ADC_CHANNEL_16_TEMPERATURE,	
	ADC_CHANNEL_17_INT_REF,
	MAX_ADC_CHANNEL
};

extern unsigned char ADCRead(unsigned char channel, unsigned int *value);
extern void ADCInit(void);


#endif //#ifndef _ADC_H_
