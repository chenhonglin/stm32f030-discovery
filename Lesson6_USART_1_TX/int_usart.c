
/*
	FILE NAME: int_usart.c
*/

#include "stm32f0xx.h"			//include file that define all SFR

#include "int_usart.h"

#define TX_TIMEOUT_COUNT		(1000)

signed char USART1_Tx(unsigned char *buffers, unsigned int length)
{
	unsigned int break_count = TX_TIMEOUT_COUNT;

	while(length --)
	{		
		break_count = TX_TIMEOUT_COUNT;
		
		while((USART1->ISR & USART_ISR_TXE) == 0)
		{
			break_count--;
			if(!break_count)
			{
				return -1;
			}
		}
		
		//load data into TDR buffers
		USART1->TDR = *buffers++;
	}

	//ensure last byte transmit before proceed
	break_count = TX_TIMEOUT_COUNT;
	while(break_count --)
	{
		//break when last byte had transmit
		if((USART1->ISR & USART_ISR_TC) == 1)
			return 0;
	}
	
	return -1;
}

void IntUSARTInit(void)
{	
	//GPIO: Enable GPIO block, PA9, PA10
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	//GPIO: Set as alternate funciton
	GPIOA->MODER &= ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10);
	GPIOA->MODER |= (GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1);
	/*
	//GPIO: output type:0
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_9|GPIO_OTYPER_OT_10);
	
	//GPIO: speed: 11
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR9|GPIO_OSPEEDR_OSPEEDR10);
	
	//GPIO: pull up/down:00
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR9|GPIO_PUPDR_PUPDR10);
	//GPIO: pull up:0`
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR9_0|GPIO_PUPDR_PUPDR10_0);
	*/
	//GPIO: AFRH: Alternate funciton
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFR9|GPIO_AFRH_AFR10);
	GPIOA->AFR[1] |= SFR_AFRH_9_01 | SFR_AFRH_10_01;
	
	//Disable Reset USART1 block
	RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;
	
	//Enable USART1 block
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	//setting 8 data bits, oversampling of 16
	USART1->CR1 &= ~(USART_CR1_M|USART_CR1_OVER8);			//set zero
	
	//setting baudrate
	USART1->BRR = SFR_OVER16_BAUD_115200;
	
	//stop bits: 1
	USART1->CR2 &= ~(USART_CR2_STOP);
	
	//enable Transmmit enable
	USART1->CR1 |= USART_CR1_TE;
	
	//enable Receive enable
	//USART1->CR1 |= USART_CR1_RE;
	
	//enable USART1
	USART1->CR1 |= USART_CR1_UE;	
}

