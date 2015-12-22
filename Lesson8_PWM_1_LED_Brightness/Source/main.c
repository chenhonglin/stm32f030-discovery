
/*
	FILE NAME: main.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "pwm.h"
#include "gpio.h"

void CPUInit(void)
{
	//Initialise GPIO as alternate function: PWM function
	GPIOInit();
	//Setting PWM
	PWMInit();
}

int main(void)
{
	CPUInit();
	
	while(1)
	{
		//idle
		;
	}
}
