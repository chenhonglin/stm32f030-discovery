/*
 * gpio.c
 *
 */
#include "stm32f0xx.h"			//include file that define all SFR

#include "gpio.h"

#define MODE_OUTPUT8				(GPIO_MODER_MODER8_0)
#define MODE_OUTPUT9				(GPIO_MODER_MODER9_0)

void GPIOLED0Toggle(void)
{
	static unsigned char onoff = 0;
	
	//toggle LD3
	onoff ^= 0x01;
	if(onoff)
	{
		//Turn on LED
		GPIOC->ODR |= GPIO_ODR_9;
	}
	else
	{
		//Turn off LED
		GPIOC->ODR &= ~GPIO_ODR_9;
	}
}

void GPIOLED1Toggle(void)
{
	static unsigned char onoff = 0;
	
	//toggle LD4
	onoff ^= 0x01;
	if(onoff)
	{
		//Turn on LED
		GPIOC->ODR |= GPIO_ODR_8;
	}
	else
	{
		//Turn off LED
		GPIOC->ODR &= ~GPIO_ODR_8;
	}
}

void GPIOInit(void)
{
	/* *** INPUT INIT *** */
	/* Input config */
	//Enable GPIO_A block
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//Configure PA0 as Input
	GPIOA->MODER &= ~GPIO_MODER_MODER0; 			//mask the bit to 00

	/* *** OUTPUT INIT *** */
	//Enable GPIO_C block
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//Configure PC8 as Output
	GPIOC->MODER &= ~GPIO_MODER_MODER8; 			//mask the bit to 00
	GPIOC->MODER |= MODE_OUTPUT8;							//set the bit
	//Configure PC9 as Output
	GPIOC->MODER &= ~GPIO_MODER_MODER9; 			//mask the bit to 00
	GPIOC->MODER |= MODE_OUTPUT9;							//set the bit
}
