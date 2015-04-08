#include "stm32f0xx.h"			//include file that define all SFR

#define MODE_OUTPUT		(GPIO_MODER_MODER9_0)

void CPUInit(void)
{
	//Enable GPIO_C block
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//Configure PC9 as Output
	GPIOC->MODER &= ~GPIO_MODER_MODER9; 			//mask the bit to 00
	GPIOC->MODER |= MODE_OUTPUT;							//set the bit
}

int main(void)
{
		CPUInit();
		while(1)
		{
			//Turn on LED
			GPIOC->ODR |= GPIO_ODR_9;
			//Turn off LED
			GPIOC->ODR &= ~GPIO_ODR_9;
		}
}
