
/*
	FILE NAME: pwm.c
*/
#include "stm32f0xx.h"			//include file that define all SFR

#include "timer.h"
#include "pwm.h"

#define TIMER_RELOAD_VALUE			(50)	//x*TIMER_FREQUENCY
#define TIMER_FREQUENCY				(1000) //(1mSec)

/*
11. Prescaler: TIM14_PSC[15:0]:PSC=480000 (scale to 10ms)
1. auto-reload register frequency: TIM14_ARR[15:0]:ARR=10 (100ms)
2. duty cycle: TIM14_CCR1[15:0]=0~10
3. PWM mode1 -TM14_CCMR1[6:4]:OC1M=110
4. Enable preload register: TIM14_CCMR1[3]:OC1PE=1
5. Autorelaod upcounting or center-align: TIM14_CR1[7]:ARPE=1
x6. Update into shadown register: TIMx_EGR:UG
x8. Center-aligh mode: TIM1_CR1:CMS:00
x9. Direction: TIM1_CR1: 0 (upcounter)
10. Set as Output: TIM14_CCER[3]:CC1NP=0
7. Active high: TIM14_CCER[1]:CC1P=0

---------try-----------------
1. TIM14_CR1[0]:CEN=1
7. Active high: TIM14_CCER[0]:CC1E=1


*/
void PWMInit(void)
{
	//Enable timer 14 block
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	
	//Divide by 2, using 50% duty cycle
	TIM14->CCR1 = TIMER_RELOAD_VALUE/2;
	
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
	TimerInit(TIMER_FREQUENCY, TIMER_RELOAD_VALUE);
}
