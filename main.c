	  /*---------------------------------------------
	   *linked list
	   * https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
	   */

#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "setup.h"
#include "positieBepaling.h"

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void){

 /* Chip errata */
  CHIP_Init();

  /* Initialize LEUART */
  setupLeuart();

  /* Setup DMA */
  setupDma();

  /* Initialize gpio */
  gpioSetup();

  /* Enable LCD without voltage boost */
    SegmentLCD_Init(false);
    SegmentLCD_Write("HELLO");
  //enable Atmel328p and RFID Reader
  GPIO_PinOutSet(gpioPortD, 3);

  char tempData[RX_BUFFER_SIZE];

  while (1) {
    /* On every wakeup enter EM2 again */
	  while(!dataReceived && !stopReceiving){
		  EMU_EnterEM2(true); //enter EM2 when waiting for data from LEUART
	  }
	  SegmentLCD_Number(length());

	  if(!stopReceiving){//get first token
		  SegmentLCD_Write("READING");
		 // SegmentLCD_Number(length());
		  for(int i = 0; i<rxBuffer.length; i++){ // verplaats waarden van ingelezen waarden van RFID naar temp data
	  	  		tempData[i]= rxBuffer.buffer[i];
	  	  	}
		  tempData[rxBuffer.length] = ',';
		  unsigned long long tempEPC1 = strtoull(strtok(tempData, ","),NULL,10); //splits waarden via comma
		  unsigned long long tempEPC2 = strtoull(strtok(NULL, ","),NULL,10); //tweede deel van EPC
		  int temprssi = atoi(strtok(NULL, ","));	// derde waarde is RSSi
		  if(tempEPC1>0 || tempEPC2>0){ // controleer op fouten
			  insertFirst(tempEPC1,tempEPC2, temprssi); // maakt nieuwe link aan en plaats in linked list
		  }
		  dataReceived = false;
	  }
	  if(stopReceiving){
		  //disable Atmel328p and RFID Reader
		  GPIO_PinOutClear(gpioPortD, 3);

		  //stopReceiving = false;
		  sort();

		  //getSmallestRssi();
		  getAverageRssi();

		  changeRssiToMillimeter();

		  stop++;
		  removeUnknowTagsAndFillPos();

		  if(length()<=5){
			  for(int i = 0;i<length();i++){
				  SegmentLCD_ARing(i,1);					//set number measured tags on ring
			  }
			  calculatePosWithCombination();				//evaluated version of fillPosAndDist() and trilateration();
			  break;
		  }
		  stopReceiving = false;
	  }
  }



  while(1){
	  if(stopReceiving){
		  SegmentLCD_Write("X-POS");
		  SegmentLCD_Number(pos[0][0]);
	  }
	  else{
		  SegmentLCD_Write("Y-POS");
		  SegmentLCD_Number(pos[0][1]);
	  }
  }
}
