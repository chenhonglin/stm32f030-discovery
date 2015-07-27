
/*
	FILE NAME: drv_dma.c
*/
#include <string.h>

#include "stm32f0xx.h"			//include file that define all SFR
#include "drv_dma_priv.h"
#include "drv_dma.h"

RDMA DMARunTime[MAX_CHANNEL];

void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA1->ISR & DMA_ISR_TCIF2)
    {
    	//disable interrupt flag
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        //disable DMA after transfer complete
        DMA1_Channel2->CCR &= ~DMA_CCR_EN;
        if(DMARunTime[CHANNEL2].CallBack)
            (*DMARunTime[CHANNEL2].CallBack)();	
    }
}

static DMA_Channel_TypeDef *DMAPtrGet(unsigned char channel)
{
    switch(channel)
    {
        case CHANNEL1:
            return DMA1_Channel1;
        case CHANNEL2:
            return DMA1_Channel2;
        case CHANNEL3:
            return DMA1_Channel3;
        case CHANNEL4:
            return DMA1_Channel4;
        case CHANNEL5:
            return DMA1_Channel5;
        default:
            return 0;
    }	
}

void DMAStructClear(SDMA *dma)
{
    memset(dma, 0, sizeof(SDMA));	
}

void DMAConfig(SDMA *dma)
{
    DMA_Channel_TypeDef *ptr;

    ptr = DMAPtrGet(dma->Channel);
    if(ptr == 0)
        return;
		
    if(dma->Config0 & BIT_CONFIG0_TRANSFER)
    {
        //memory to peripheral
        ptr->CMAR = dma->SourceAddr;
        ptr->CPAR = dma->DestinationAddr;
        ptr->CCR |= DMA_CCR_DIR;
    }
    else
    {
        //peripheral to memory
        ptr->CPAR = dma->SourceAddr;
        ptr->CMAR = dma->DestinationAddr;
        ptr->CCR &= ~DMA_CCR_DIR;
    }
    ptr->CNDTR = dma->TotalBytes;
    ptr->CCR &= ~(DMA_CCR_PSIZE|DMA_CCR_MSIZE|DMA_CCR_PL|DMA_CCR_MEM2MEM);
    ptr->CCR |= (DMA_CCR_MINC|DMA_CCR_TCIE);
    
    if(dma->CallBack != NULL)
    {
        DMARunTime[dma->Channel].CallBack = dma->CallBack;

        //enable DMA interrupt enable
        NVIC->ISER[0] |= (0x01<<DMA1_Channel2_3_IRQn);
    }

    //temporary testing here
    USART1->ICR |= USART_ICR_TCCF;
    USART1->CR3 |= USART_CR3_DMAT;
	
	//enable DMA transfer
    ptr->CCR |= DMA_CCR_EN;
}

signed int DMAByteRemainGet(unsigned char Channel)
{
    DMA_Channel_TypeDef *ptr;

    ptr = DMAPtrGet(Channel);
    if(ptr == 0)
        return -1;

    return ptr->CNDTR;	
}

void DMAInit(void)
{
    unsigned char i;
	
    for(i=0;i<MAX_CHANNEL;i++)
    {
        DMARunTime[i].CallBack = NULL;
    }	
	
    //DMA SFR
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}
