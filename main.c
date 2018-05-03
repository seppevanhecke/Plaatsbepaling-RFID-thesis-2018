/***************************************************************************//**
 * @file main.c
 * @brief LEUART/DMA in EM2 example for EFM32WG_DK3750 starter kit
 * @version 5.2.2
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include "em_device.h"
#include "em_chip.h"
//#include "em_cmu.h"
#include "em_emu.h"
//#include "em_leuart.h"
//#include "em_dma.h"
//#include "dmactrl.h"
//#include "em_timer.h"
//#include "em_prs.h"
#include "setup.h"
#include "positieBepaling.h"






///*-----
// * test linked list
// */
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <math.h>
//
//#include "em_gpio.h"
//#include "em_int.h"
///*------------
// * einde test linked list
// */





//#define RX_TIMEOUT_MS     		2000

//#define TMR_SR_OPCODE_VERSION 0x03
//#define TMR_SR_OPCODE_GET_READ_TX_POWER 0x62



//uint32_t leuartif;
//uint32_t len;
//uint32_t baudrate;
//uint8_t opcode[1];
//bool dataReceived = false;
//bool stopReceiving = false;
//#define MAX_SUB_POSITIONS 10
//long pos[MAX_SUB_POSITIONS][2];
//long tagPos[3][2] = {{-1,-1},{-1,-1},{-1,-1}};
//long int tagDist[3] = {-1,-1,-1};
//int weight[3] = {1,1,1};

//struct rxBuffer// = {.startIndex = 0, .stopIndex = 0}
//{
//  volatile char buffer[RX_BUFFER_SIZE];
//  volatile int startIndex;  /* Position of first unread data */
//  volatile int stopIndex;   /* Position after last unread data */
//  volatile int length;
//} rxBuffer;
//
//struct RFIDData{
//	volatile unsigned long long EPC1; //EPC number of tag: deel 1
//	volatile unsigned long long EPC2; //EPC number of tag: deel 2
//	volatile int rssi; //Rssi Value of tag
//	volatile long dist;
//	volatile int xPos;
//	volatile int yPos;
//	struct RFIDData * next;
//}RFIDData_t;
//
//struct RFIDData *head = NULL;
////struct RFIDData *current = NULL;
////struct RFIDData *next = NULL;
//
///*
// * Database of tags
// * 		EPC1, 			EPC2, 		Xpos in mm, 	Ypos in mm
// */
//static const unsigned long long tags[5][4] ={
//	{7210110810811133,	 26835963937, 	0,				0			},		//00
//	{226002113418,		 28635963955, 	2680,			0			},		//01
//	{226002113418, 	     26435963923, 	0,		 		3300		},		//02
//	{226002113418, 		 28935963961, 	2680,			2720		},		//03
//	{226002113418, 		 26435963922, 	0,			 	0			}		//test
//};

/*-------------------------------------------------------------------------------------------------
 * test voor een gelinkte lijst van inkomende data
 * https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
 */


//void GPIO_ODD_IRQHandler(void)
//{
//	    GPIO_IntClear(GPIO_IntGet());
//  stopReceiving = !stopReceiving;
//
//}







///*
// * Calculation of inverse of matrix
// * http://www.ccodechamp.com/c-program-to-find-inverse-of-matrix/
// */
//long getDeterminant(long **matrix, int orde){
//	long det = 0;
//	long **temp = NULL;
//	int s = 1,i,j,m,n,c;
//	if(orde == 1){
//		det = matrix[0][0];
//	}
//	else if(orde ==2){
//		det = matrix[0][0]*matrix[1][1]- matrix[1][0]*matrix[0][1];
//	}
//	else{
//		det = 0;
//		for(c = 0; c<orde;c++){
//			m = 0;
//			n = 0;
//			//allocate temp matrix
//			temp = malloc(orde*sizeof(long *));
//			for(i = 0;i<orde;i++){
//				temp[i] = malloc(orde*sizeof(long));
//			}
//			//end allocation of temp matrix
//			for(i=0;i<orde;i++){
//				for(j=0;j<orde;j++){
//					temp[i][j]=0;
//					if(i!=0 && j!=c){
//						temp[m][n] = matrix[i][j];
//						if(n<(orde-2))
//							n++;
//						else{
//							n = 0;
//							m++;
//						}
//					}
//				}
//
//			}
//			det = det+s*(matrix[0][c]* getDeterminant(temp,orde-1));
//			s=-1*s;
//			//free temp matrix
//			for(i = 0;i<orde;i++){
//				free(temp[i]);
//			}
//			free (temp);
//			//end free of temp matrix
//		}
//	}
//	return det;
//}
//
//
///*
// * calculate inverse given matrix with given orde and place in inverse matrix
// * return 1 if possible
// * return -1 if not
// */
//int getInverse(long **matrix, double **inverse, int orde){
//	double det = 0;
//	long **temp = NULL;
//	long **cofactor = NULL;
//	int p,q,i,j,m,n;
//
//	det = getDeterminant(matrix,orde);
//	if(det == 0){
//		return -1;
//	}
//	//allocate cofactor matrix
//	cofactor = malloc(orde*sizeof(long *));
//	for(i = 0;i<orde;i++){
//		cofactor[i] = malloc(orde*sizeof(long *));
//	}
//	//end allocation of cofactor matrix
//
//	for(q = 0;q<orde;q++){
//		for(p = 0; p<orde;p++){
//			m = 0;
//			n = 0;
//
//			//allocate temp matrix
//			temp = malloc(orde*sizeof(long *));
//			for(i = 0;i<orde;i++){
//				temp[i] = malloc(orde*sizeof(long));
//			}
//			//end allocation of temp matrix
//
//			for(i=0;i<orde;i++){
//				for(j=0;j<orde;j++){
//					if(i!=q && j!=p){
//						temp[m][n] = matrix[i][j];
//						if(n<(orde-2)){
//							n++;
//						}
//						else{
//							n = 0;
//							m++;
//						}
//					}
//				}
//			}
//			cofactor[p][q] = pow((double)-1,(double)q+p+2) * getDeterminant(temp,orde-1); // get cofactor and transpose in one step
//			//free temp matrix
//			for(i = 0;i<orde;i++){
//				free(temp[i]);
//			}
//			free (temp);
//			//end free of temp matrix
//		}
//	}
//
//	//finding transpose of matrix and devine by the determinat to get inverse matrix of matrix
//	for(i=0;i<orde;i++){
//		for(j = 0;j<orde;j++){
//			inverse[i][j] = cofactor[i][j]/det;
//		}
//	}
//	//free cofactor matrix
//	for(i = 0;i<orde;i++){
//		free(cofactor[i]);
//	}
//	free (cofactor);
//	//end free of cofactor matrix
//
//	return 1;
//}
//
//
//
///*
// * calculate the position with the use of trilateration
// *
// * pos 		--> calculated position 	[ x-pos y-pos]
// * tagPos 	--> position of tags 		[x-pos1 y-pos1,
// * 										 x-pos2 y-pos2,
// * 										 x-pos3 y-pos3]
// * tagDist 	--> distance from tag to pos [dist1, dist2, dist3]
// * count	--> count of different trilaterations
// */
//void trilateration(long pos[MAX_SUB_POSITIONS][2], long tagPos[3][2], long int tagDist[3], int count){
//	int dimension = 2;//sizeof(tagPos[0])/sizeof(tagPos[0][0]);
//	int numberOfTags = 3;//sizeof(tagPos)/(sizeof(tagPos[0]));
////	int numberOfDist = sizeof(tagDist)/(sizeof(tagDist[1]));
//	long **A = NULL;
//	double **invA = NULL;
//	long *B = NULL;
//	int i,j;
//
//
//	//if number of tags is not number of dist: return -1,-1 --> fault
////	if(numberOfDist != numberOfTags){
////		pos[0] = -1;
////		pos[1] = -1;
////		return;
////	}
//
//	//allocate A and B matrix
//	A = malloc(numberOfTags*sizeof(long *));
//	for(i = 0;i<numberOfTags;i++){
//		A[i] = malloc((dimension+1)*sizeof(long));
//	}
//	B = malloc(numberOfTags*sizeof(long));
//	//end allocation of A and B matrix
//
//
//	for(i = 0; i<numberOfTags;i++){
//		//A = 1  -2x   -2y  (-2z)
//		A[i][0] = 1;
//		for(j = 1;j<dimension+1;j++){
//			A[i][j] = tagPos[i][j-1]*(-2);
//		}
//		//B = dist^2-x^2-y^2 (-z^2)
//		B[i] = tagDist[i]*tagDist[i];
//		for(j = 0;j<dimension;j++){
//			B[i] = B[i] - tagPos[i][j]*tagPos[i][j];
//		}
//	}
//	if(numberOfTags == 3){
//		pos[count-1][0]=0;
//		pos[count-1][1]=0;
//		invA = malloc(numberOfTags*sizeof(double *));
//		for(i = 0;i<numberOfTags;i++){
//			invA[i] = malloc(numberOfTags*sizeof(double));
//		}
//		if(getInverse(A,invA,numberOfTags)==-1){
//		//fault in getInverse
//			//pos[0] = -1;
//			//pos[1] = -1;
//			//return;
//		}
//		/*
//		 * calculate coordinates from A*X = B
//		 * with X = {x^2 + y^2
//		 * 				x			--> pos[0]
//		 * 				y	   }	--> pos[1]
//		 * calculate X with invA*B = X
//		 */
//		for(i = 0;i<numberOfTags; i++){
//			pos[count-1][0] = pos[count-1][0]+ invA[1][i]*B[i]; //x-value of coordinates
//			pos[count-1][1] = pos[count-1][1]+ invA[2][i]*B[i]; //y-value of coordinates
//		}
//		return;
//	}
//	else{
//		pos[count-1][0] = -1;
//		pos[count-1][1] = -1;
//		return;
//	}
//
//
//}
//
//
//
//int getMillimeterLineair(int rssi)
//{
//static const  int lookup[61] =
//{
//	0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		0,
//	0,		0,		0,		0,		90,
//	194,	298,	402,	506,	611,
//	715,	819,	923,	1027,	1131,
//	1235,	1339,	1443,	1547,	1651,
//	1755,	1859,	1964,	2068,	2172,
//	2276,	2380,	2484,	2588,	2692
//};
//int mm = lookup[rssi];
//return mm;
//}
//
//int getMillimeterExp(int rssi)
//{
//static const  int lookup[61] =
//{
//	9,
//	10,		11,		12,		14,		15,
//	17,		19,		21,		23,		25,
//	28,		31,		35,		39,		43,
//	48,		53,		59,		65,		72,
//	80,		89,		99,		110,	122,
//	135,	150,	167,	185,	206,
//	229,	254,	282,	313,	347,
//	386,	428,	475,	528,	568,
//	651,	722,	802,	890,	988,
//	1097,	1218,	1353,	1502,	1668,
//	1851,	2056,	2282,	2534,	2813,
//	3124,	3468,	3850,	4275,	4746
//};
//int mm = lookup[rssi];
//return mm;
//}
//
//
//
//
//
////insert link at the first location
//void insertFirst(unsigned long long EPC1, unsigned long long EPC2, int rssi){
//	//create a link
//	struct RFIDData *link = (struct RFIDData*) malloc(sizeof(struct RFIDData));
//
//	link->EPC1 = EPC1;
//	link->EPC2 = EPC2;
//	link->rssi = rssi;
//
//	//point to old first node
//	link->next = head;
//
//	//point first node to the new first node
//	head = link;
//}
///*
// * compare EPC of two RFIDData structures
// * return -1 	if EPC of RFID1 is 	smaller 	then EPC of RFID2
// * return 0 	if EPC of RFID1 is 	equal 		to EPC of RFID2
// * return 1 	if EPC of RFID1 is 	greater 	then EPC of RFID2
// *
// * RFID1 EPC: AB
// * RFID2 EPC: CD
// * 		A>C		A=C		A<C
// * B>D	1		 1		-1
// * B=D 	1		 0		-1
// * B<D	1		 -1		-1
// */
//int compareEPC(struct RFIDData *RFID1, struct RFIDData *RFID2){
//	if(RFID1->EPC1 == RFID2->EPC1){
//		if(RFID1->EPC2 == RFID2->EPC2){
//			return 0;
//		}
//		else if(RFID1->EPC2 < RFID2->EPC2){
//			return -1;
//		}
//		else return 1;
//	}
//	else if(RFID1->EPC1 < RFID2->EPC1){
//		return -1;
//	}
//	else return 1;
//
//}
//
//
///*
// * delete first item
// */
//void deleteFirst(){
//	//save reference to first link
//	struct RFIDData *tempLink = head;
//
//	//mark next to first link as first
//	head = head->next;
//
//	//free deleted link
//	free(tempLink);
//}
///*Delete a specific next node in the linked list
// * current --> current node with the next to be deleted
// */
//void deleteNextNode(struct RFIDData *current){
//	struct RFIDData *next = current->next;
//	if(next != NULL){
//		if(next->next == NULL){
//			current->next = NULL;
//			free(next);
//		}
//		else{
//			current->next = next->next;
//			free(next);
//		}
//	}
//	return;
//}
///*
// *Zoek lengte van linked list
// */
//int length(){
//	int lenght = 0;
//	struct RFIDData *current;
//
//	for(current = head; current != NULL; current = current->next){
//		lenght++;
//	}
//	return lenght;
//}
//
///*
// * sorteer linked list op EPC
// */
//void sort(){
//	int i, j, k, tempRssi, compEPC;
//	unsigned long long tempEPC1, tempEPC2;
//	struct RFIDData *current;
//	struct RFIDData *next;
//
//	int size = length();
//	k = size-1;//-1 anders te ver --> verkeerde data in linked list
//
//	for(i = 0; i<size-1; i++,k--){
//		current = head;
//		next = head->next;
//
//		for(j=0; j<k; j++){
//			compEPC = compareEPC(current, next);
//			if(compEPC==1){
//				tempEPC1 = current->EPC1;
//				tempEPC2 = current->EPC2;
//				current->EPC1 = next->EPC1;
//				current->EPC2 = next->EPC2;
//				next->EPC1 = tempEPC1;
//				next->EPC2 = tempEPC2;
//
//				tempRssi = current->rssi;
//				current->rssi = next->rssi;
//				next->rssi = tempRssi;
//			}
//			else {
//				if(compEPC == 0){ // zelfde tag dan sorteren op rssi van klein naar groot
//					if(current->rssi <= next->rssi){
//						tempRssi = current->rssi;
//						current->rssi = next->rssi;
//						next->rssi = tempRssi;
//					}
//				}
//			}
//			current = current->next;
//			next = next->next;
//
//		}
//	}
//}
//
//void getSmallestRssi(){
//	struct RFIDData *current;
//	struct RFIDData *next;
//
//	current = head;
//	next = head->next;
//
//	while(current->next!=NULL){
//		while(compareEPC(current, next) == 0){
//			if(next->rssi <= current->rssi){
//				deleteNextNode(current); //delete next node after current
//				next = current->next;
//			}
//		}
//		if(current->next == NULL){
//			return;
//		}
//		current = current->next;
//		next = current->next;
//
//	}
//}
//
//void getAverageRssi(){
//	struct RFIDData *current;
//	struct RFIDData *next;
//	int sum;
//	int count;
//
//	current = head;
//	next = head->next;
//
//	while(current->next!=NULL){
//		sum = current->rssi;
//		count = 1;
//		while(compareEPC(current, next) == 0){
//			sum = sum + next->rssi;
//			count++;
//			deleteNextNode(current); //delete next node after current
//			next = current->next;
//		}
//
//		current->rssi = sum/count; // get average of one tag
//		if(current->next == NULL){
//			return;
//		}
//		current = current->next;
//		next = current->next;
//	}
//
//}
//
//void changeRssiToMillimeter(){
//	//int size = length();
//	struct RFIDData *current;
//	struct RFIDData *next;
//
//	current = head;
//	next = head->next;
//	do{
//		//current->dist = getMillimeterLineair(current->rssi*-1);
//		current->dist = getMillimeterExp(current->rssi*-1);
//		current = current->next;
//		next = current->next;
//	}
//	while(current!=NULL);
//
//}
//
///*
// * fill position matrix with the three first tags
// */
//void fillPosAndDist(){
//	unsigned long long EPC1, EPC2;
//	bool found;
//	int numberOfTags = sizeof(tags)/sizeof(tags[0]);
//	struct RFIDData *current = head;
//	//next = current-> next;
//	int j = 0;
//	for(int i = 0; i <3; i++){
//		EPC1 = current->EPC1;
//		EPC2 = current->EPC2;
//		do{
//			if(EPC1 == tags[j][0] && EPC2 == tags[j][1]){
//				tagPos[i][0] = tags[j][2];
//				tagPos[i][1] = tags[j][3];
//				tagDist[i] = current->dist;
//				found = true;
//			}
//			j++;
//		}
//		while(j<numberOfTags && !found);
//		if(!found){
//			//no tag found with correct EPC code
//			tagPos[i][0] = -1;
//			tagPos[i][1] = -1;
//			tagDist[i]   = -1;
//		}
//		j = 0;
//		found = false;
//		current = current->next;
//	}
//}
//
///*
// * deleted tags that are not in the taglist
// * fill in pos if known tag
// */
//
//void removeUnknowTagsAndFillPos(){
//	unsigned long long EPC1, EPC2;
//	bool found;
//	int numberOfTags = sizeof(tags)/sizeof(tags[0]);
//	struct RFIDData *current = NULL;
//	struct RFIDData *next = head;
//	int j = 0;
//
//	do{
//		EPC1 = next->EPC1;
//		EPC2 = next->EPC2;
//		found = false;
//		j = 0;
//
//		do{
//			if(EPC1 == tags[j][0] && EPC2 == tags[j][1]){
//				found = true;
//			}
//			j++;
//		}
//		while(j<numberOfTags && !found);
//		if(!found){
//		//no tag found with correct EPC code: delete tag
//			if(current != NULL){ // wrong tag is not the head tag
//				SegmentLCD_Write("DEL1");
//				deleteNextNode(current);
//				SegmentLCD_Write("DEL2");
//				next = current->next;
//			}
//			else{	//wrong tag is head tag
//				deleteFirst();
//				next = head;
//			}
//
//		}
//		else {		//tag found fill in xPos and yPos
//			next->xPos = tags[j-1][2]; //correction of j++
//			next->yPos = tags[j-1][3]; //correction of j++
//			current = next;
//			next = current->next;
//		}
//	}
//	while(next!=NULL);
//}
///*
// *
// */
//void calculatePosWithCombination(){
//	struct RFIDData *tag1 = head;
//	struct RFIDData *tag2 = head->next;
//	struct RFIDData *tag3 = head->next->next;
//	int xPos = 0;
//	int yPos = 0;
//	int count = 0;
//	int correctCount = 0;			//count of correct calculated values
//	if(tag1 == NULL){
//		pos[0][0] = -1;
//		pos[0][1] = -1;
//		return;
//	}
//	if(tag2 == NULL){//pos is tagPos of head
//		pos[0][0] = tag1->xPos;
//		pos[0][1] = tag1->yPos;
//		return;
//	}
//	/*
//	 * pos is distance average
//	 * example:
//	 * xpos: 900 ----------------------------------------------------- 1000
//	 * dist: 70														   40
//	 * solution  -----------------------------------963---------------
//	 */
//	if(tag3 == NULL){
//		pos[0][0] = tag1->xPos+(tag2->xPos-tag1->xPos)*((double)tag1->dist/(tag1->dist+tag2->dist));
//		pos[0][1] = tag1->yPos+(tag2->yPos-tag1->yPos)*((double)tag1->dist/(tag1->dist+tag2->dist));
//		return;
//	}
//	/*
//	 * calculate position with a combination of different groups of 3 tags
//	 * with the use of trilateration
//	 *
//	 * first fill in used tagpos and distances
//	 */
//	tagPos[0][0] = tag1->xPos;
//	tagPos[0][1] = tag1->yPos;
//	tagDist[0] = tag1->dist;
//	tagPos[1][0] = tag2->xPos;
//	tagPos[1][1] = tag2->yPos;
//	tagDist[1] = tag2->dist;
//	tagPos[2][0] = tag3->xPos;
//	tagPos[2][1] = tag3->yPos;
//	tagDist[2] = tag3->dist;
//
//	/*
//	 * tag1 -> tag2 -> tag3 -> tag4 -> tag5 -> NULL	  count
//	 *  +        +       +								1
//	 *  +        +              +						2
//	 *  +        +                      +				3
//	 *  +                +      +						4
//	 *  +                +              +				5
//	 *  +                       +       +				6
//	 *           +       +      +						7
//	 *           +       +              +				8
//	 *           +              +       +				9
//	 *                   +      +       +				10
//	 */
//	while(tag3!= NULL || tag2->next != tag3 || tag1->next != tag2){
//		while(tag3!=NULL || tag2->next!=tag3){
//			while(tag3!=NULL){
//				count++;
//				tagPos[2][0] = tag3->xPos;
//				tagPos[2][1] = tag3->yPos;
//				tagDist[2] = tag3->dist;
//				trilateration(pos, tagPos, tagDist, count);
//
//				tag3 = tag3->next;
//			}
//
//			tag2 = tag2->next;
//			tag3 = tag2->next;
//			tagPos[1][0] = tag2->xPos;
//			tagPos[1][1] = tag2->yPos;
//			tagDist[1] = tag2->dist;
//		}
//		tag1 = tag1->next;
//		tag2 = tag1->next;
//		tag3 = tag2->next;
//
//		tagPos[0][0] = tag1->xPos;
//		tagPos[0][1] = tag1->yPos;
//		tagDist[0] = tag1->dist;
//		tagPos[1][0] = tag2->xPos;
//		tagPos[1][1] = tag2->yPos;
//		tagDist[1] = tag2->dist;
//		tagPos[2][0] = tag3->xPos;
//		tagPos[2][1] = tag3->yPos;
//		tagDist[2] = tag3->dist;
//	}
//
//	//calculate average of measured values
//	for(int i = 0; i<count; i++){
//		if(pos[count][0]!=-1||pos[count][1]!=-1){		//count only correct calculated values --> xPos and yPos are not -1
//			xPos = xPos + pos[i][0];				//sum of correct calculated xPos values
//			yPos = yPos + pos[i][1];				//sum of correct calcualted yPos values
//			correctCount++;								//count of correct values
//		}
//	}
//	pos[0][0] = xPos/correctCount;					//take average of correct xPos values
//	pos[0][1] = yPos/correctCount;					//take average of correct yPos values
//	return;
//
//
//
//
//
//}
///*-------------------------------------------------------------------------
//
// * einde test gelinkte lijst van inkomende data
// */

///* Flag indicating that new data is available in RX ring buffer */
//volatile bool dataAvailable = false;
//
///* DMA callback structure */
//DMA_CB_TypeDef cb[DMA_CHANNELS];

///**************************************************************************//**
// * @brief  TIMER0 Interrupt handler
// * The TIMER OF interrupt is triggered when the RX line has been stationary for
// * longer than the timout period. This ISR then checks how many bytes have been
// * transferred and updates the rxStopIndex accordingly.
// *****************************************************************************/
//void TIMER0_IRQHandler(void)
//{
//  DMA_DESCRIPTOR_TypeDef *primDescr;
//  DMA_DESCRIPTOR_TypeDef *altDescr;
//  DMA_CB_TypeDef         *dmaCallback;
//
//  /* Clear interrupt flag */
//  TIMER_IntClear(TIMER0, TIMER_IFC_OF);
//
//  /* Get primary descriptor */
//  primDescr = ((DMA_DESCRIPTOR_TypeDef *)(DMA->CTRLBASE)) + DMA_CHANNEL_RX;
//
//  /* Get alternate descriptor */
//  altDescr = ((DMA_DESCRIPTOR_TypeDef *)(DMA->ALTCTRLBASE)) + DMA_CHANNEL_RX;
//
//  /* Get callback to check if primary or alternate structure is used */
//  dmaCallback = (DMA_CB_TypeDef *)(primDescr->USER);
//
//  /* Check in alternate or primary descriptors how many bytes were transferred */
//  /* and move rxStopIndex accordingly */
//  if (dmaCallback->primary)
//  {
//    rxBuffer.stopIndex = RX_BUFFER_SIZE/2 - ((primDescr->CTRL & _DMA_CTRL_N_MINUS_1_MASK) >>_DMA_CTRL_N_MINUS_1_SHIFT)-1;
//  }
//  else
//  {
//    rxBuffer.stopIndex = RX_BUFFER_SIZE - ((altDescr->CTRL & _DMA_CTRL_N_MINUS_1_MASK) >>_DMA_CTRL_N_MINUS_1_SHIFT)-1;
//  }
//
//  /* Indicate new data available */
//  dataAvailable = true;
//}

///**************************************************************************//**
// * @brief LEUART IRQ handler
// *
// * When the signal frame is detected by the LEUART, this interrupt routine will
// * zero-terminate the char array, and
// * reset the DMA for new data.
// *
// *****************************************************************************/
//void LEUART0_IRQHandler(void)
//{
// 	GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 1);
//  /* Store and reset pending interupts */
//  leuartif = LEUART_IntGet(LEUART0);
//  LEUART_IntClear(LEUART0, LEUART_IntGet(LEUART0));
//
//
////  if(len==10){
////	  rxBuffer.length           = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
////	  /* Reactivate DMA */
////	      DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
////	                        true,            				/* Activate using primary descriptor */
////	                        false,           				/* No DMA burst */
////	                        NULL,            				/* Keep source */
////	                        NULL,            				/* Keep destination */
////	  					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
////	      dataReceived = true;
////  }
//
//  /* Signal frame found. */
//  if (leuartif & LEUART_IF_SIGF)
//  {
//    /* Zero-terminate rx buffer */
//    len            = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
//    rxBuffer.buffer[len - 1] = 0;
//    rxBuffer.length = len-1;
//    dataReceived = true;
//
//    /* Reactivate DMA */
//    DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
//                      true,            				/* Activate using primary descriptor */
//                      false,           				/* No DMA burst */
//                      NULL,            				/* Keep source */
//                      NULL,            				/* Keep destination */
//					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
//  }
//
//  /* Signal frame found. */
//  //if (leuartif & LEUART_IF_RXDATAV)
//  //  {
//	//  rxBuffer.length++;
////      /* Zero-terminate rx buffer */
////      len            = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);
////      rxBuffer.length++;
////      rxBuffer.buffer[len - 1] = 0;
////
////      /* Reactivate DMA */
////      DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
////                        true,            				/* Activate using primary descriptor */
////                        false,           				/* No DMA burst */
////                        NULL,            				/* Keep source */
////                        NULL,            				/* Keep destination */
////  					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
////    }
//  //GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
//}

///**************************************************************************//**
// * @brief  Call-back called when RX is complete
// *****************************************************************************/
//void rxDmaComplete(unsigned int channel, bool primary, void *user) /// komt hier enkel als de number of DMA transfer elements gedaan is
//{
//  (void) user;
//  /* Reactivate DMA */
//  DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
//                          true,            				/* Activate using primary descriptor */
//                          false,           				/* No DMA burst */
//                          NULL,            				/* Keep source */
//                          NULL,            				/* Keep destination */
//    					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
//}

///***************************************************************************//**
// * @brief  Setting up LEUART
// ******************************************************************************/
//void setupLeuart(void)
//{
////  LEUART_Reset(LEUART0);
////  NVIC_ClearPendingIRQ(LEUART0_IRQn);
////  unsigned long HFXOFrequency = 24000000u;
////  SystemHFXOClockSet(HFXOFrequency);
//
//  /* Enable peripheral clocks */
//  CMU_ClockEnable(cmuClock_HFPER, true);
//  /* Configure GPIO pins */
//  CMU_ClockEnable(cmuClock_GPIO, true);
//  /* To avoid false start, configure output as high */
//  GPIO_PinModeSet(LEUART_TXPORT, LEUART_TXPIN, gpioModePushPull, 1);
//  GPIO_PinModeSet(LEUART_RXPORT, LEUART_RXPIN, gpioModeInput, 0);
//
//  LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;
//  //init.baudrate = 115200;
//
//  /* Enable CORE LE clock in order to access LE modules */
//  CMU_ClockEnable(cmuClock_CORELE, true);
//  //CMU_ClockEnable(cmuSelect_CORELEDIV2, true);
//
//  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
//  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
//  //CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
//  CMU_ClockEnable(cmuClock_LEUART0, true);
//
//  /* Do not prescale clock */
//  CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1);
//
//  //CMU_ClockDivSet(cmuClock_UART1, true);
//
//  /* setting baudrate to 115200 for communication with the reader */
//  //init.refFreq = cmuClock_LEUART0;
//
//  /* Configure LEUART */
//  init.enable = leuartDisable;
//
//  LEUART_Init(LEUART0, &init);
//
//  baudrate = CMU_ClockFreqGet(cmuSelect_CORELEDIV2);
//
//  baudrate = LEUART_BaudrateGet(LEUART0);
//
//  /* Enable pins at default location */
//  LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_LOCATION;
//
//  /* Set RXDMAWU to wake up the DMA controller in EM2 */
//  LEUART_RxDmaInEM2Enable(LEUART0, true);
//
//  /* Set TXDMAWU to wake up the DMA controller in EM2 */ //************************************************************************ testje
// //   LEUART_TxDmaInEM2Enable(LEUART0, true);
//
//    /* Set LEUART signal frame */
//    LEUART0->SIGFRAME = ';'; //datablock eindigd met ;
//
//    /* Enable LEUART Signal Frame Interrupt */
//    LEUART_IntEnable(LEUART0, LEUART_IEN_SIGF);
//
//    /* Enable LEUART RX Data Valid Interrupt */
//    LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);
//
//
//    /* Enable LEUART1 interrupt vector */
//    NVIC_EnableIRQ(LEUART0_IRQn);
//
//  /* Finally enable it */
//  LEUART_Enable(LEUART0, leuartEnable);
//}
//
///***************************************************************************//**
// * @brief  Setup DMA
// *
// * @details
// *   This function initializes DMA controller.
// *   It configures the DMA channel to be used for LEUART0 transmit
// *   and receive. The primary descriptor for channel0 is configured for
// *   a single data byte transfer. For continous data reception and transmission
// *   using LEUART DMA loopmode is enabled for channel0.
// *   In the end DMA transfer cycle is configured to basicMode where
// *   the channel source address, destination address, and
// *   the transfercount per dma cycle have been specified.
// *
// ******************************************************************************/
//void setupDma(void)
//{
//  /* DMA configuration structs */
//  DMA_Init_TypeDef       dmaInit;
//  DMA_CfgChannel_TypeDef rxchannelCfg;
//  DMA_CfgChannel_TypeDef txchannelCfg;
//  DMA_CfgDescr_TypeDef   rxdescrCfg;
//  DMA_CfgDescr_TypeDef   txdescrCfg;
//  DMA_CfgLoop_TypeDef    loopCfg;
//
//  /* Initializing the DMA */
//  dmaInit.hprot        = 0;						/* No descriptor protection */
//  dmaInit.controlBlock = dmaControlBlock;		/* DMA control block alligned to 256 */
//  DMA_Init(&dmaInit);
//
//  /*** Setting up RX DMA ***/
//
//  /* Setting up call-back function */
//  	  cb[DMA_CHANNEL_RX].cbFunc  = rxDmaComplete;
//  	  cb[DMA_CHANNEL_RX].userPtr = NULL;
//
//  /* Setting up channel */
//  rxchannelCfg.highPri   = false; 				/* Normal priority */
//  rxchannelCfg.enableInt = true; 				/* No interupt enabled for callback functions */
//
//  /* Configure channel 0 */
//  /*Setting up DMA transfer trigger request*/
//  rxchannelCfg.select = DMAREQ_LEUART0_RXDATAV;	/* Set LEUART0 RX data avalible as source of DMA signals */
//  rxchannelCfg.cb     = &(cb[DMA_CHANNEL_RX]);				/* No callback funtion */
//  DMA_CfgChannel(DMA_CHANNEL_RX, &rxchannelCfg);
//
//  /* Setting up channel descriptor */
//  rxdescrCfg.dstInc = dmaDataInc1;			/* Increment destination address by one byte */
//  rxdescrCfg.srcInc = dmaDataIncNone;		/* Do no increment source address  */
//  rxdescrCfg.size   = dmaDataSize1;			/* Data size is one byte */
//  rxdescrCfg.arbRate = dmaArbitrate1;		/* Rearbitrate for each byte recieved*/
//  rxdescrCfg.hprot   = 0;					/* No read/write source protection */
//
//  /* Configure primary descriptor  */
//  DMA_CfgDescr(DMA_CHANNEL_RX, true, &rxdescrCfg);
//  DMA_CfgDescr(DMA_CHANNEL_RX, false, &rxdescrCfg);
//
//  /* Starting the transfer. Using Basic Mode */
//  DMA_ActivateBasic(DMA_CHANNEL_RX,                /* Activate channel selected */
//                    true,                       /* Use primary descriptor */
//                    false,                      /* No DMA burst */
//                    (void *) &rxBuffer.buffer,  /* Destination address */
//                    (void *) &LEUART0->RXDATA,  /* Source address*/
//					RX_BUFFER_SIZE - 1);             /* Size of buffer minus1 */
//
//  /*** Setting up TX DMA ***/
//
//
//  	   /* Setting up channel */
//  	  txchannelCfg.highPri   = false;
//  	  txchannelCfg.enableInt = false;
//  	  txchannelCfg.select    = DMAREQ_LEUART0_TXBL;
//  	  txchannelCfg.cb        = NULL;
//  	  DMA_CfgChannel(DMA_CHANNEL_TX, &txchannelCfg);
//
//  	  /* Setting up channel descriptor */
//  	  txdescrCfg.dstInc  = dmaDataIncNone;
//  	  txdescrCfg.srcInc  = dmaDataInc1;
//  	  txdescrCfg.size    = dmaDataSize1;
//  	  txdescrCfg.arbRate = dmaArbitrate1;
//  	  txdescrCfg.hprot   = 0;
//  	  DMA_CfgDescr(DMA_CHANNEL_TX, true, &txdescrCfg);
//
////  	  /* Activate basic dma cycle using channel0 */
////  	  DMA_ActivateBasic(0,
////  	                    true,
////  	                    false,
////  						(void *)&LEUART0->TXDATA,
////  	                    (void *)&LEUART0->RXDATA,
////  	                    0);
//
//  /* Configure loop transfer mode */
//  loopCfg.enable = true;
//  loopCfg.nMinus1 = 0;  /* Single transfer per DMA cycle*/
//  DMA_CfgLoop(0, &loopCfg);
//}
//
///**************************************************************************//**
// * @brief  Start DMA Transmit of UART Data
// *****************************************************************************/
//void sendUartData(void *buffer, int bytes)
//{
////  /* Wait until channel becomes available */
////  while(DMA_ChannelEnabled(DMA_CHANNEL_TX));
////
////  /* Activate DMA channel for TX */
////  DMA_ActivateBasic(DMA_CHANNEL_TX,
////                    true,
////                    false,
////                    (void *)&(UART0->TXDATA),
////                    buffer,
////                    bytes - 1);
//
//	/* Wait until channel becomes available */
//	  //while(DMA_ChannelEnabled(DMA_CHANNEL_TX));
//
//	  /* Activate DMA channel for TX */
//	  DMA_ActivateBasic(DMA_CHANNEL_TX,
//	                    true,
//	                    false,
//	                    (void *)&(LEUART0->TXDATA),
//	                    buffer,
//	                    bytes - 1);
//}

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
 // char *tempArray[2];
  //int start =0;
  int stop = 0;
//
//  for(int i = 0; i<RX_BUFFER_SIZE; i++){
//  		getal[i]= 1;
//  	}
//
//  opcode[0] = TMR_SR_OPCODE_VERSION;
//  sendUartData(&opcode,1);
//  while(rxBuffer.length ==0){
//
//
//  EMU_EnterEM2(true);
//  LEUART_Enable(LEUART0, leuartDisable);
//  rxBuffer.length = rxBuffer.buffer[1];
//  }
//  for(int i = 0; i<rxBuffer.length; i++){
//  		getal[i]= rxBuffer.buffer[i];
//  	}
//  sendUartData(&getal,rxBuffer.length);
//
//  DMA_ActivateBasic(DMA_CHANNEL_RX,     			/* Activate DMA channel 0 */
//                            true,            				/* Activate using primary descriptor */
//                            false,           				/* No DMA burst */
//                            NULL,            				/* Keep source */
//                            NULL,            				/* Keep destination */
//      					  RX_BUFFER_SIZE - 1);    		/* Number of DMA transfer elements (minus 1) */
//

  while (1) {
    /* On every wakeup enter EM2 again */
	//TIMER0_IRQHandler();

	  //dataReceived = false;
	  //LEUART1->CTRL &= LEUART_CTRL_RXDMAWU;
	  while(!dataReceived && !stopReceiving){
		  EMU_EnterEM2(true); //enter EM2 when waiting for data from LEUART
	  }
	  SegmentLCD_Number(length());
	  //LEUART1->CTRL &= ~LEUART_CTRL_RXDMAWU;
	  /*---------------------------------------------
	   * test linked list
	   * https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
	   */
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
	  /*-----------------------
	   * einde test linked list
	   */

//	  for(int i = 0; i<rxBuffer.length; i++){
//	  		getal[stop][i]= rxBuffer.buffer[i];
//	  	}
	  stop++;
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

		  /*if(length()>=3){
			  SegmentLCD_Write("E TAGS");
			  fillPosAndDist();
			  trilateration(pos, tagPos, tagDist, 1);

			  break;
		  }
		  SegmentLCD_Write("NE TAGS");
		  */


		  if(length()<=5){
			  for(int i = 0;i<length();i++){
				  SegmentLCD_ARing(i,1);					//set number measured tags on ring
			  }
			  calculatePosWithCombination();				//evaluated version of fillPosAndDist() and trilateration();
			  break;
		  }



		  stopReceiving = false;
	  }




//	start = rxBuffer.startIndex;
//	stop = rxBuffer.stopIndex;
	 //rxBuffer.length            = RX_BUFFER_SIZE - 1 - ((dmaControlBlock->CTRL >> 4) & 0x3FF);


	//sendUartData(&getal,32);
	//sendUartData(&start,1);
	//sendUartData(&stop,1);
	//LEUART_Tx(&LEUART0, stop);
	//rxBuffer.startIndex = rxBuffer.stopIndex;
	//sendUartData(&rxBuffer.stopIndex, sizeof(rxBuffer.stopIndex));
	  //INT_Disable();
	 //EMU_EnterEM2(true);
	 // INT_Enable();
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
