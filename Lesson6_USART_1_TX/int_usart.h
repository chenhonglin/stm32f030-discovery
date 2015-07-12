
/*
	FILE NAME: int_usart.h
*/

#define NUM_TX_BUFFERS		(150)
#define NUM_RX_BUFFERS		(150)

#define SFR_OVER16_BAUD_9600		(0x1388)
#define SFR_OVER16_BAUD_115200	(0x1A1)

#define SFR_AFRH_9_01						((uint32_t)0x00000010)
#define SFR_AFRH_10_01					((uint32_t)0x00000100)

extern signed char USART1_Tx(unsigned char *buffers, unsigned int length);

extern void IntUSARTInit(void);
