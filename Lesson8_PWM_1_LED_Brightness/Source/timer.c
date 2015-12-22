
/*
	FILE NAME: timer.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "timer.h"

#define TIMER_RELOAD_VALUE			(100)	//10*TIMER_FREQUENCY

//#define COUNT_DOWN					(0)
//#define COUNT_UP					(1)

//TIM14_IRQHandler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void TIM14_IRQHandler(void)
{
	/*
	static unsigned char count=0;
	static unsigned char reload=TIMER_RELOAD_VALUE;
	static unsigned char direction=COUNT_DOWN;
	
	count++;
	if(count >= 20)
	{
		count = 0;
		if(!reload)
		{
			reload++;
			direction = COUNT_UP;
		}
		else if(reload == TIMER_RELOAD_VALUE)
		{
			reload--;
			direction = COUNT_DOWN;
		}
		else
		{
			if(direction == COUNT_DOWN)
				reload --;
			else if(direction == COUNT_UP)
				reload ++;
			else
				reload = TIMER_RELOAD_VALUE/2;
		}
		TIM14->CCR1 = reload;
	}
	*/
	//clear interrupt flag so it can interrupt system again next round
	TIM14->SR &= ~TIM_SR_UIF;
	
	//do thing require when interrupt occur
	//LEDOnOff ^= 0x01;
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
