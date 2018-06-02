/*
 * setup.h
 *
 *  Created on: 24-apr.-2018
 *      Author: seppe
 */

#ifndef SRC_SETUP_H_
#define SRC_SETUP_H_


#include "em_leuart.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_int.h"
#include "em_lcd.h"
#include "segmentlcd.h"

/** LEUART Rx/Tx Port/Pin Location */
#define LEUART_LOCATION    0
#define LEUART_TXPORT      gpioPortD            /* LEUART transmission port */
#define LEUART_TXPIN       4                    /* LEUART transmission pin */
#define LEUART_RXPORT      gpioPortD            /* LEUART reception port */
#define LEUART_RXPIN       5                    /* LEUART reception pin */

#define RX_BUFFER_SIZE       	256 /* Even numbers only */
#define TX_BUFFER_SIZE       	RX_BUFFER_SIZE
#define DMA_CHANNEL_RX       	0
#define DMA_CHANNEL_TX       	1
#define DMA_CHANNELS         	2

/* DMA control block, must be aligned to 256. */
#if defined (__ICCARM__)
#pragma data_alignment=256
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2];
#elif defined (__CC_ARM)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2] __attribute__ ((aligned(256)));
#elif defined (__GNUC__)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMA_CHAN_COUNT * 2] __attribute__ ((aligned(256)));
#else
#error Undefined toolkit, need to define alignment
#endif

uint32_t leuartif;
uint32_t len;

struct rxBuffer// = {.startIndex = 0, .stopIndex = 0}
{
  volatile char buffer[RX_BUFFER_SIZE];
  volatile int startIndex;  /* Position of first unread data */
  volatile int stopIndex;   /* Position after last unread data */
  volatile int length;
} rxBuffer;


/* Flag indicating that new data is available in RX ring buffer */
volatile bool dataAvailable;


bool dataReceived;
bool stopReceiving;
/* DMA callback structure */
DMA_CB_TypeDef cb[DMA_CHANNELS];

void setupLeuart(void);
void setupDma(void);
void sendUartData(void *buffer, int bytes);
void GPIO_ODD_IRQHandler(void);
void LEUART0_IRQHandler(void);
void rxDmaComplete(unsigned int channel, bool primary, void *user);
void gpioSetup(void);


#endif /* SRC_SETUP_H_ */
