
/*
	FILE NAME: systick.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "systick.h"

volatile unsigned int SysTickCount;	

//SysTick_Handler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void SysTick_Handler(void)
{
	static unsigned int Delay;	
	
	Delay++;	
	if(Delay >= 10)
	{
		Delay = 0;
		//do thing require when interrupt occur
		SysTickCount++;
	}
}

unsigned int SysTickGet(void)
{
	return SysTickCount;
}

void SysTickInit(void)
{
	SysTickCount = 0;
	
	//SFR configurations
	SysTick->LOAD = 4800000;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_CLKSOURCE_Msk;
}
