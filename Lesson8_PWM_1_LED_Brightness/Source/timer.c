
/*
	FILE NAME: timer.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "timer.h"

#define TIMER_RELOAD_VALUE			(100)	//10*TIMER_FREQUENCY

//TIM14_IRQHandler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void TIM14_IRQHandler(void)
{
	//clear interrupt flag so it can interrupt system again next round
	TIM14->SR &= ~TIM_SR_UIF;
}


void TimerInit(unsigned int freq, unsigned int reload_val)
{
	unsigned short prescaler = (SystemCoreClock/freq) - 1;
	
	//Enable Timer 14 block
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	
	//Timer Prescale Value
	//Dividing from System Clock to have Shorter Timer Duration
	TIM14->PSC = prescaler;
	
	//Timer Reload Value Upon Overflow
	//Use to Further Fine Tune Timer Interrupt Duration
	TIM14->ARR = reload_val;

	//Run Timer
	TIM14->CR1 |= TIM_CR1_CEN;
}
