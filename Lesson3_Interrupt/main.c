#include "stm32f0xx.h"			//include file that define all SFR

#define MODE_OUTPUT		(GPIO_MODER_MODER9_0)

volatile unsigned char InputStateInterrupt;

//EXTI0_1_IRQHandler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void EXTI0_1_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR0)
	{
		//Clear Interrupt Flag
		//To Enable Next Trigger
		EXTI->PR = 1;

		if(GPIOA->IDR & GPIO_IDR_0)
			InputStateInterrupt = 0;
		else 
			InputStateInterrupt = 1;		
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

void InterruptInit(void)
{
	//Connect Input To Interrupt Source 0
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	//Select as Interrupt Source	
	EXTI->IMR |= EXTI_IMR_MR0;
	//Enable Rising Interrupt
	EXTI->RTSR |= EXTI_RTSR_TR0;
	//Enable Falling Interrupt
	EXTI->FTSR |= EXTI_FTSR_TR0;
	
	//Enable Interrupt
	NVIC->ISER[0] |= (0x01<<EXTI0_1_IRQn);
}

void InputRead(void)
{
	if(GPIOA->IDR & GPIO_IDR_0)
		InputStateInterrupt = 0;
	else 
		InputStateInterrupt = 1;
}

void CPUInit(void)
{
	GPIOInit();
	
	InterruptInit();
}

int main(void)
{
	CPUInit();
	InputRead();
	while(1)
	{
		//InputStateInterrupt is updated in interrupt
		//in function EXTI0_1_IRQHandler
		if(InputStateInterrupt)
		{
			//Turn off LED
			GPIOC->ODR &= ~GPIO_ODR_9;
		}
		else
		{
			//Turn on LED
			GPIOC->ODR |= GPIO_ODR_9;
		}
	}
}
