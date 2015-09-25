
/*
	FILE NAME: gpio.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "gpio.h"

#define GPIOA_0_GPIO		(0x00)
#define GPIOA_0_USART1_CTS	(0x01)
#define GPIOA_0_ADC_IN0		(0x02)

void GPIOInit(void)
{
	/* Input config */
	
	//Enable GPIO_A block
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	//Configure PA0 using alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER0; 			//mask the bit to 00
	GPIOA->MODER |= GPIO_MODER_MODER0_1;
	
	//configure to enable ADC_IN0
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFR0;
	GPIOA->AFR[0] |= GPIOA_0_ADC_IN0;		
}
