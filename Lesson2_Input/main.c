#include "stm32f0xx.h"

void MCUInit(void)
{
	/* Input config */
	//Enable GPIO_A block
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//Configure PA0 as Input
	GPIOA->MODER &= ~GPIO_MODER_MODER0; 			//mask the bit to 00
}

int main(void)
{
	unsigned char InputState;
	
	MCUInit();
	
	while(1)
	{
		//read input state
		InputState = 	GPIOA->IDR & GPIO_IDR_0;
	}
}
