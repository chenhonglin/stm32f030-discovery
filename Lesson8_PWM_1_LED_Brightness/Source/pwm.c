
/*
	FILE NAME: pwm.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "timer.h"
#include "pwm.h"

//#define PWM_PC9_BLINK

#ifdef PWM_PC9_BLINK
	#define TIMER_RELOAD_VALUE			(500)								//x*TIMER_FREQUENCY
	#define TIMER_FREQUENCY				(1000) 								//(1mSec)
	#define TIMER_RELOAD_VALUE_ADJUST	(TIMER_RELOAD_VALUE-1)				//offset to get the correct pwm timeout
	#define PWM_DUTY_CYCLE				(TIMER_RELOAD_VALUE_ADJUST/2)		//Divide by 2, using 50% duty cycle
#else //#ifdef PWM_PC9_BLINK
	#define TIMER_RELOAD_VALUE			(10)								//x*TIMER_FREQUENCY
	#define TIMER_FREQUENCY				(1000) 								//(1mSec)
	#define TIMER_RELOAD_VALUE_ADJUST	(TIMER_RELOAD_VALUE-1)				//offset to get the correct pwm timeout
	#define PWM_DUTY_CYCLE				(TIMER_RELOAD_VALUE_ADJUST/2+1)		//Divide by 2, using 50% duty cycle
#endif //#ifdef PWM_PC9_BLINK

void PWMInit(void)
{
	//Enable timer 14 block	
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	
	//Setting PWM duty cycle
	TIM14->CCR1 = PWM_DUTY_CYCLE;
	
	//Configure as PWM mode 1
	//Signal active when TIMxCNT < TIMx_CCR1
	TIM14->CCMR1 |= (TIM_CCMR1_OC1M_2|TIM_CCMR1_OC1M_1);
	
	//Enable Preload register, as advice in datasheet
	TIM14->CCMR1 |= (TIM_CCMR1_OC1PE);
	
	//Enable auto reload
	TIM14->CR1 |= TIM_CR1_ARPE;
	
	//Output pin active high
	TIM14->CCER &= ~(TIM_CCER_CC1P);
	
	//Enable output pin
	TIM14->CCER |= TIM_CCER_CC1E;
	
	//Trigger update event, to initilize register and
	//transfer preload register to shadow register
	TIM14->EGR |= TIM_EGR_UG;
	
	//Setting PWM timer
	TimerInit(TIMER_FREQUENCY, TIMER_RELOAD_VALUE_ADJUST);
}
