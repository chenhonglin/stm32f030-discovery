
#include "stm32f0xx.h"			//include file that define all SFR

#define MODE_OUTPUT				(GPIO_MODER_MODER9_0)
#define REQUIRE_FREQ			(1000)					//1KHz, 1mSeconds
#define AUTO_REALOD_VAL			(1000)					//1Seconds timeout

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

//TIM3_IRQHandler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void TIM3_IRQHandler(void)
{

	//clear interrupt flag so it can interrupt system again next round
	TIM3->SR &= ~TIM_SR_UIF;
	
	//do thing require when interrupt occur
	LEDOnOff ^= 0x01;
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

void TimerInit(void)
{
	unsigned short prescaler = (SystemCoreClock/REQUIRE_FREQ) - 1;
	
	//Enable Timer 3 block
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		
	//Timer Prescale Value
	//Dividing from System Clock to have Shorter Timer Duration
	TIM3->PSC = prescaler;
	
	//Timer Reload Value Upon Interrupt
	//Use to Further Fine Tune Timer Interrupt Duration
	TIM3->ARR = AUTO_REALOD_VAL;

	//Enable Interrupt : Within Timer
	TIM3->DIER |= TIM_DIER_UIE;

	//Enable Interrupt For Timer 3	
	NVIC->ISER[0] |= (0x01<<TIM3_IRQn);

	//Stop Timer During Debugger Halt
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM3_STOP;
	
	//Run Timer
	TIM3->CR1 |= TIM_CR1_CEN;								//Enable bit CEN
}

void CPUInit(void)
{
	GPIOInit();	
	TimerInit();
}

void SysInit(void)
{
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
