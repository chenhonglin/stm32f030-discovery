
/*
	FILE NAME: gpio.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "gpio.h"

#define MODE_OUTPUT		(GPIO_MODER_MODER9_0)

void OnBoardLEDInit(void)
{
	//Enable GPIO_C block
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//Configure PC9 as Output
	GPIOC->MODER &= ~GPIO_MODER_MODER9; 			//mask the bit to 00
	GPIOC->MODER |= MODE_OUTPUT;					//set the bit

	//Turn on LED
	GPIOC->ODR |= GPIO_ODR_9;	
}

void GPIOInit(void)
{
	//On board LED: LD3:PC9
	OnBoardLEDInit();
	
	//PWM14, at pin PB1
	
	//Enable GPIO_B block
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	
	//GPIO: Set as alternate funciton
	GPIOB->MODER |= (GPIO_MODER_MODER1_1);

	//Configure PB1 as Alternate Function 0 (AF0)
	//TIM14_CH1
	GPIOB->AFR[0] |= SFR_AFRL_1_00;
	
	//change pin speed: High Speed
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR1;
}
