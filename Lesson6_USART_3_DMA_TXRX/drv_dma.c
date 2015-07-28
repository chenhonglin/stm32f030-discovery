
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
	//Is DMA has complete transfer all data
    if(DMA1->ISR & DMA_ISR_TCIF2)
    {
    	//disable interrupt flag
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        //disable DMA after transfer complete
        DMA1_Channel2->CCR &= ~DMA_CCR_EN;
        //call application call back function
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

	//getting the pointer for right channel
    ptr = DMAPtrGet(dma->Channel);
    if(ptr == 0)
        return;
		
    if(dma->Config0 & BIT_CONFIG0_TRANSFER)
    {
        //memory to peripheral
        //set memory address
        ptr->CMAR = dma->SourceAddr;
        //set peripheral address
        ptr->CPAR = dma->DestinationAddr;
        //set as memory to peripheral transfer
        ptr->CCR |= DMA_CCR_DIR;
    }
    else
    {
        //peripheral to memory
        //set peirpheral address
        ptr->CPAR = dma->SourceAddr;
        //set memory address
        ptr->CMAR = dma->DestinationAddr;
        //set as peripheral to memory transfer
        ptr->CCR &= ~DMA_CCR_DIR;
    }
    //set the total of bytes require to be transfer
    ptr->CNDTR = dma->TotalBytes;
    //set memory size=8bits, dma priority=low, disable memory to memory transfer
    ptr->CCR &= ~(DMA_CCR_PSIZE|DMA_CCR_MSIZE|DMA_CCR_PL|DMA_CCR_MEM2MEM);
    //enable memory increment and interrupt upon transfer complete
    ptr->CCR |= (DMA_CCR_MINC|DMA_CCR_TCIE);
    
    if(dma->CallBack != NULL)
    {
        DMARunTime[dma->Channel].CallBack = dma->CallBack;

        //enable DMA interrupt enable
        NVIC->ISER[0] |= (0x01<<DMA1_Channel2_3_IRQn);
    }

	//enable DMA transfer
    ptr->CCR |= DMA_CCR_EN;
}

signed int DMAByteRemainGet(unsigned char Channel)
{
    DMA_Channel_TypeDef *ptr;

    ptr = DMAPtrGet(Channel);
    if(ptr == 0)
        return -1;

	//return the total of byte pending to be send
    return ptr->CNDTR;	
}

void DMAInit(void)
{
    unsigned char i;
	
    for(i=0;i<MAX_CHANNEL;i++)
    {
        DMARunTime[i].CallBack = NULL;
    }	
	
    //Power up DMA block
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
}
