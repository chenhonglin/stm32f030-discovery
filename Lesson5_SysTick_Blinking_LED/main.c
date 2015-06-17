
#include "stm32f0xx.h"			//include file that define all SFR

#define MODE_OUTPUT				(GPIO_MODER_MODER9_0)

volatile unsigned int Delay;
volatile unsigned char LEDOnOff;	

//unsigned int SystemCoreClock = 48000000;	//system running at 48MHz

void LEDOutput(int onoff)
{
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

//SysTick_Handler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void SysTick_Handler(void)
{
	Delay++;	
	if(Delay >= 10)
	{
		Delay = 0;
		//do thing require when interrupt occur
		LEDOnOff ^= 0x01;
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
	//Configure PC9 as Output
	GPIOC->MODER &= ~GPIO_MODER_MODER9; 			//mask the bit to 00
	GPIOC->MODER |= MODE_OUTPUT;							//set the bit
}

void SysTickInit(void)
{
	SysTick->LOAD = 4800000;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_CLKSOURCE_Msk;
}

void CPUInit(void)
{
	GPIOInit();	
	SysTickInit();
}

void SysInit(void)
{
	Delay=0;
	LEDOnOff = 0;	
}

int main(void)
{
	SysInit();
	CPUInit();	

	while(1)
	{
		//flag change in timer interrupt
		//change LED output base on flag
		if(LEDOnOff&0x01)
		{
			LEDOutput(0);
		}
		else
		{
			LEDOutput(1);
		}
	}
}
