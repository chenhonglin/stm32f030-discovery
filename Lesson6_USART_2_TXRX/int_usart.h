
/*
	FILE NAME: int_usart.h
*/

#define NUM_TX_BUFFERS						(150)
#define NUM_RX_BUFFERS						(150)

#define SFR_OVER16_BAUD_9600				(0x1388)
#define SFR_OVER16_BAUD_115200				(0x1A1)

#define SFR_AFRH_9_01						((uint32_t)0x00000010)
#define SFR_AFRH_10_01						((uint32_t)0x00000100)

typedef struct
{
	unsigned char Buffers[NUM_RX_BUFFERS];	//buffer keeping received data
	unsigned int PtrCons;					//consuming data
	unsigned int PtrProd;					//receving incoming data
}SUSART_Buffers_RX;

extern unsigned int USART1_RxAvailable(void);
extern signed char USART1_Tx(unsigned char *buffers, unsigned int length);
extern unsigned int USART1_Rx(unsigned char *buffers, unsigned int length);

extern void IntUSARTInit(void);
