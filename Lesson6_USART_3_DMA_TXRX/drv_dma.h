
/*
	FILE NAME: drv_dma.h
*/

#define CHANNEL1                                (0)
#define CHANNEL2                                (1)
#define CHANNEL3                                (2)
#define CHANNEL4                                (3)
#define CHANNEL5                                (4)
#define MAX_CHANNEL                             (CHANNEL5+1)

#define BIT_CONFIG0_TRANSFER                (0x01)
	#define CONFIG0_TRANSFER_PERI2MEM           (0x00)
	#define CONFIG0_TRANSFER_MEM2PERI           (0x01)

typedef struct
{
	unsigned char   Channel;
	unsigned int    SourceAddr;
	unsigned int    DestinationAddr;
	unsigned int    TotalBytes;
	unsigned char   Config0;
	void            (*CallBack)(void);
}SDMA;

//Init stucture SDMA with default value
extern void DMAStructClear(SDMA *dma);
//Configure and start DMA transfer
extern void DMAConfig(SDMA *dma);
//Initialise DMA block and internal structure
extern void DMAInit(void);
//Return total of bytes yet to be transfer
extern signed int DMAByteRemainGet(unsigned char Channel);
