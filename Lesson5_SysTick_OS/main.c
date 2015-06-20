
#include "stm32f0xx.h"			//include file that define all SFR

#include "gpio.h"
#include "task.h"

//unsigned int SystemCoreClock = 48000000;	//system running at 48MHz

//SysTick_Handler ISR is being define in file 'startup_stm32f030.s'
//We just need to redefine to overwrite the original
void SysTick_Handler(void)
{
	SystemTick_ISR_100mS();
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

int main(void)
{
	CPUInit();	

	TaskStart();
}
