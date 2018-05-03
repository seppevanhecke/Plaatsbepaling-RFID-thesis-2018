/*
 * setup.c
 *
 *  Created on: 24-apr.-2018
 *      Author: seppe
 */

#include "setup.h"

volatile bool dataAvailable = false;


bool dataReceived = false;
bool stopReceiving = false;


/**************************************************************************//**
 * @brief LEUART IRQ handler
 *
 * When the signal frame is detected by the LEUART, this interrupt routine will
 * zero-terminate the char array, and
 * reset the DMA for new data.
 *
 *****************************************************************************/
void LEUART0_IRQHandler(void)
{
 	GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 1);
  /* Store and reset pending interupts */
  leuartif = LEUART_IntGet(LEUART0);
  LEUART_IntClear(LEUART0, LEUART_IntGet(LEUART0));


//  if(len==10){
//	  rxBuffer.length           = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
//	  /* Reactivate DMA */
//	      DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
//	                        true,            				/* Activate using primary descriptor */
//	                        false,           				/* No DMA burst */
//	                        NULL,            				/* Keep source */
//	                        NULL,            				/* Keep destination */
//	  					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
//	      dataReceived = true;
//  }

  /* Signal frame found. */
  if (leuartif & LEUART_IF_SIGF)
  {
    /* Zero-terminate rx buffer */
    len            = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
    rxBuffer.buffer[len - 1] = 0;
    rxBuffer.length = len-1;
    dataReceived = true;

    /* Reactivate DMA */
    DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
                      true,            				/* Activate using primary descriptor */
                      false,           				/* No DMA burst */
                      NULL,            				/* Keep source */
                      NULL,            				/* Keep destination */
					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
  }

  /* Signal frame found. */
  //if (leuartif & LEUART_IF_RXDATAV)
  //  {
	//  rxBuffer.length++;
//      /* Zero-terminate rx buffer */
//      len            = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
//      rxBuffer.length++;
//      rxBuffer.buffer[len - 1] = 0;
//
//      /* Reactivate DMA */
//      DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
//                        true,            				/* Activate using primary descriptor */
//                        false,           				/* No DMA burst */
//                        NULL,            				/* Keep source */
//                        NULL,            				/* Keep destination */
//  					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
//    }
  //GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
}

void GPIO_ODD_IRQHandler(void)
{
	    GPIO_IntClear(GPIO_IntGet());
  stopReceiving = !stopReceiving;

}
/**************************************************************************//**
 * @brief  Call-back called when RX is complete
 *****************************************************************************/
void rxDmaComplete(unsigned int channel, bool primary, void *user) /// komt hier enkel als de number of DMA transfer elements gedaan is
{
  (void) user;
  /* Reactivate DMA */
  DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
                          true,            				/* Activate using primary descriptor */
                          false,           				/* No DMA burst */
                          NULL,            				/* Keep source */
                          NULL,            				/* Keep destination */
    					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
}

/***************************************************************************//**
 * @brief  Setting up LEUART
 ******************************************************************************/
void setupLeuart(void)
{
//  LEUART_Reset(LEUART0);
//  NVIC_ClearPendingIRQ(LEUART0_IRQn);
//  unsigned long HFXOFrequency = 24000000u;
//  SystemHFXOClockSet(HFXOFrequency);

  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);

  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;
  //init.baudrate = 115200;

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);
  //CMU_ClockEnable(cmuSelect_CORELEDIV2, true);

  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
  //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
  CMU_ClockEnable(cmuClock_LEUART0, true);

  /* Do not prescale clock */
  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);

  //CMU_ClockDivSet(cmuClock_UART1, true);

  /* setting baudrate to 115200 for communication with the reader */
  //init.refFreq = cmuClock_LEUART0;

  /* Configure LEUART */
  init.enable = leuartDisable;

  LEUART_Init(LEUART0, &init);

//  baudrate = CMU_ClockFreqGet(cmuSelect_CORELEDIV2);

  //baudrate = LEUART_BaudrateGet(LEUART0);

  /* Enable pins at default location */
  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;

  /* Set RXDMAWU to wake up the DMA controller in EM2 */
  LEUART_RxDmaInEM2Enable(LEUART0, true);

  /* Set TXDMAWU to wake up the DMA controller in EM2 */ //************************************************************************ testje
 //   LEUART_TxDmaInEM2Enable(LEUART0, true);

    /* Set LEUART signal frame */
    LEUART0->SIGFRAME = ';'; //datablock eindigd met ;

    /* Enable LEUART Signal Frame Interrupt */
    LEUART_IntEnable(LEUART0, LEUART_IEN_SIGF);

    /* Enable LEUART RX Data Valid Interrupt */
    LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);


    /* Enable LEUART1 interrupt vector */
    NVIC_EnableIRQ(LEUART0_IRQn);

  /* Finally enable it */
  LEUART_Enable(LEUART0, leuartEnable);
}

/***************************************************************************//**
 * @brief  Setup DMA
 *
 * @details
 *   This function initializes DMA controller.
 *   It configures the DMA channel to be used for LEUART0 transmit
 *   and receive. The primary descriptor for channel0 is configured for
 *   a single data byte transfer. For continous data reception and transmission
 *   using LEUART DMA loopmode is enabled for channel0.
 *   In the end DMA transfer cycle is configured to basicMode where
 *   the channel source address, destination address, and
 *   the transfercount per dma cycle have been specified.
 *
 ******************************************************************************/
void setupDma(void)
{
  /* DMA configuration structs */
  DMA_Init_TypeDef       dmaInit;
  DMA_CfgChannel_TypeDef rxchannelCfg;
  DMA_CfgChannel_TypeDef txchannelCfg;
  DMA_CfgDescr_TypeDef   rxdescrCfg;
  DMA_CfgDescr_TypeDef   txdescrCfg;
  DMA_CfgLoop_TypeDef    loopCfg;

  /* Initializing the DMA */
  dmaInit.hprot        = 0;						/* No descriptor protection */
  dmaInit.controlBlock = dmaControlBlock;		/* DMA control block alligned to 256 */
  DMA_Init(&dmaInit);

  /*** Setting up RX DMA ***/

  /* Setting up call-back function */
  	  cb[DMA_CHANNEL_RX].cbFunc  = rxDmaComplete;
  	  cb[DMA_CHANNEL_RX].userPtr = NULL;

  /* Setting up channel */
  rxchannelCfg.highPri   = false; 				/* Normal priority */
  rxchannelCfg.enableInt = true; 				/* No interupt enabled for callback functions */

  /* Configure channel 0 */
  /*Setting up DMA transfer trigger request*/
  rxchannelCfg.select = DMAREQ_LEUART0_RXDATAV;	/* Set LEUART0 RX data avalible as source of DMA signals */
  rxchannelCfg.cb     = &(cb[DMA_CHANNEL_RX]);				/* No callback funtion */
  DMA_CfgChannel(DMA_CHANNEL_RX, &rxchannelCfg);

  /* Setting up channel descriptor */
  rxdescrCfg.dstInc = dmaDataInc1;			/* Increment destination address by one byte */
  rxdescrCfg.srcInc = dmaDataIncNone;		/* Do no increment source address  */
  rxdescrCfg.size   = dmaDataSize1;			/* Data size is one byte */
  rxdescrCfg.arbRate = dmaArbitrate1;		/* Rearbitrate for each byte recieved*/
  rxdescrCfg.hprot   = 0;					/* No read/write source protection */

  /* Configure primary descriptor  */
  DMA_CfgDescr(DMA_CHANNEL_RX, true, &rxdescrCfg);
  DMA_CfgDescr(DMA_CHANNEL_RX, false, &rxdescrCfg);

  /* Starting the transfer. Using Basic Mode */
  DMA_ActivateBasic(DMA_CHANNEL_RX,                /* Activate channel selected */
                    true,                       /* Use primary descriptor */
                    false,                      /* No DMA burst */
                    (void *) &rxBuffer.buffer,  /* Destination address */
                    (void *) &LEUART0->RXDATA,  /* Source address*/
					RX_BUFFER_SIZE - 1);             /* Size of buffer minus1 */

  /*** Setting up TX DMA ***/


  	   /* Setting up channel */
  	  txchannelCfg.highPri   = false;
  	  txchannelCfg.enableInt = false;
  	  txchannelCfg.select    = DMAREQ_LEUART0_TXBL;
  	  txchannelCfg.cb        = NULL;
  	  DMA_CfgChannel(DMA_CHANNEL_TX, &txchannelCfg);

  	  /* Setting up channel descriptor */
  	  txdescrCfg.dstInc  = dmaDataIncNone;
  	  txdescrCfg.srcInc  = dmaDataInc1;
  	  txdescrCfg.size    = dmaDataSize1;
  	  txdescrCfg.arbRate = dmaArbitrate1;
  	  txdescrCfg.hprot   = 0;
  	  DMA_CfgDescr(DMA_CHANNEL_TX, true, &txdescrCfg);

//  	  /* Activate basic dma cycle using channel0 */
//  	  DMA_ActivateBasic(0,
//  	                    true,
//  	                    false,
//  						(void *)&LEUART0->TXDATA,
//  	                    (void *)&LEUART0->RXDATA,
//  	                    0);

  /* Configure loop transfer mode */
  loopCfg.enable = true;
  loopCfg.nMinus1 = 0;  /* Single transfer per DMA cycle*/
  DMA_CfgLoop(0, &loopCfg);
}

/**************************************************************************//**
 * @brief  Start DMA Transmit of UART Data
 *****************************************************************************/
void sendUartData(void *buffer, int bytes)
{
//  /* Wait until channel becomes available */
//  while(DMA_ChannelEnabled(DMA_CHANNEL_TX));
//
//  /* Activate DMA channel for TX */
//  DMA_ActivateBasic(DMA_CHANNEL_TX,
//                    true,
//                    false,
//                    (void *)&(UART0->TXDATA),
//                    buffer,
//                    bytes - 1);

	/* Wait until channel becomes available */
	  //while(DMA_ChannelEnabled(DMA_CHANNEL_TX));

	  /* Activate DMA channel for TX */
	  DMA_ActivateBasic(DMA_CHANNEL_TX,
	                    true,
	                    false,
	                    (void *)&(LEUART0->TXDATA),
	                    buffer,
	                    bytes - 1);
}

/**************************************************************************//**
 * @brief  Setup Timer
 * Configure TIMER to reset and start counter every time the RX pin on
 * the UART has a falling edge. If the RX line is idle for longer than the
 * timeout period the overflow interrupt is called indicating that a full
 * message has been received.
 *****************************************************************************/
//void setupTimer(void)
//{
//  TIMER_Init_TypeDef   init   = TIMER_INIT_DEFAULT;
//  TIMER_InitCC_TypeDef initCc = TIMER_INITCC_DEFAULT;
//
//  /* Configure TIMER0 to set a DMA request on falling input on CC0 */
//  init.fallAction  = timerInputActionReloadStart ; /* Reload and Start TIMER on falling edge on CC0 input */ // geef aan dat de RX heeft een startsignaal gekregen
//  init.oneShot     = true;                         /* One shot, stop on overflow */
//  init.enable      = false;                        /* Do not start timer */
//  init.prescale    = timerPrescale1024;            /* Prescale by 1024 */
//  TIMER_Init(TIMER0, &init);
//
//  /* Configure CC0 to listen to PRS CH0 */
//  initCc.prsInput = true;
//  initCc.prsSel   = timerPRSSELCh0; //luistert naar channel 0 van de PRS
//  TIMER_InitCC(TIMER0, 0, &initCc);
//
//  /* Set TOP value according to timout value*/
//  TIMER_TopSet(TIMER0, (CMU_ClockFreqGet(cmuClock_TIMER0)/1000)*RX_TIMEOUT_MS/1024); //wacht 2 seconden nadat de laatste falling edge is geweest
//
//  /* Enable input sensing for PRS */
//  GPIO_InputSenseSet(GPIO_INSENSE_PRS, GPIO_INSENSE_PRS);
//
//  /* Configure pin 1 interrupt/PRS to port E. Setting is shared by interrupts and PRS */ //watch the RX pin of the LEUART0
//  GPIO_IntConfig(gpioPortD, 5, false, false, false);
//
//  /* Configure PRS channel 0 to listen to pin 5 PRS signals */
//  PRS_SourceSignalSet(0, PRS_CH_CTRL_SOURCESEL_GPIOL, PRS_CH_CTRL_SIGSEL_GPIOPIN5, prsEdgeOff); // channal: 0 , source is een GPIO met pinnummer 5
//
//  /* Generate interrupt on overflow (timeout) */
//  TIMER_IntEnable(TIMER0, TIMER_IEN_OF);
//  NVIC_EnableIRQ(TIMER0_IRQn);
//}

/***************************************************************************//**
 * @brief  Gpio setup. Setup button pins to trigger falling edge interrupts.
 *  Register callbacks for that interrupts.
 ******************************************************************************/
void gpioSetup(void)
{
  /* Enable GPIO in CMU */
  CMU_ClockEnable(cmuClock_GPIO, true);


  /* Configure PB9 and PB10 as input */
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  //GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0);

  //pin for enabling Atmel328p and RFID Reader
  GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 0);



  /* Set falling edge interrupt for both ports */
  GPIO_IntConfig(gpioPortB, 9, false, true, true);
  //GPIO_IntConfig(gpioPortD, 3, false, true, true);

  /* Enable interrupt in core for even and odd gpio interrupts */

    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    //NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    //NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}




