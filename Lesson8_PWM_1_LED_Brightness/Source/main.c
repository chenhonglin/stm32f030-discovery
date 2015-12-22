
/*
	FILE NAME: main.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "pwm.h"
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

void CPUInit(void)
{
	//On board LED: LD3:PC9
	//				LD4:PC8
	OnBoardLEDInit();
	
	//Initialise GPIO as alternate function: PWM function
	GPIOInit();
	//Setting PWM
	PWMInit();
}

void TestLED(void)
{
	//Turn on LED
	GPIOC->ODR |= GPIO_ODR_9;
	//Turn off LED
	GPIOC->ODR &= ~GPIO_ODR_9;
}

int main(void)
{
	CPUInit();
	
	TestLED();
	
	while(1)
	{
		;
	}
}
