/*
 * positieBepaling.h
 *
 *  Created on: 24-apr.-2018
 *      Author: seppe
 */

#ifndef SRC_POSITIEBEPALING_H_
#define SRC_POSITIEBEPALING_H_


//#include "setup.h"
/*-----
 * test linked list
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "em_gpio.h"
#include "em_int.h"
#include "setup.h"
/*------------
 * einde test linked list
 */

uint32_t baudrate;
uint8_t opcode[1];


#define MAX_SUB_POSITIONS 10
long pos[MAX_SUB_POSITIONS][2];
long tagPos[3][2];
long int tagDist[3];
int weight[3];



struct RFIDData{
	volatile unsigned long long EPC1; //EPC number of tag: deel 1
	volatile unsigned long long EPC2; //EPC number of tag: deel 2
	volatile int rssi; //Rssi Value of tag
	volatile long dist;
	volatile int xPos;
	volatile int yPos;
	struct RFIDData * next;
}RFIDData_t;

struct RFIDData *head;
//struct RFIDData *current = NULL;
//struct RFIDData *next = NULL;

/*
 * Database of tags
 * 		EPC1, 			EPC2, 		Xpos in mm, 	Ypos in mm
 */
static const unsigned long long tags[5][4] ={
	{7210110810811133,	 26835963937, 	0,				0			},		//00
	{226002113418,		 28635963955, 	2680,			0			},		//01
	{226002113418, 	     26435963923, 	0,		 		3300		},		//02
	{226002113418, 		 28935963961, 	2680,			2720		},		//03
	{226002113418, 		 26435963922, 	0,			 	0			}		//test
};


long getDeterminant(long **matrix, int orde);
int getInverse(long **matrix, double **inverse, int orde);
void trilateration(long pos[MAX_SUB_POSITIONS][2], long tagPos[3][2], long int tagDist[3], int count);
int getMillimeterLineair(int rssi);
int getMillimeterExp(int rssi);
void insertFirst(unsigned long long EPC1, unsigned long long EPC2, int rssi);
int compareEPC(struct RFIDData *RFID1, struct RFIDData *RFID2);
void deleteFirst();
void deleteNextNode(struct RFIDData *current);
int length();
void sort();
void getSmallestRssi();
void getAverageRssi();
void changeRssiToMillimeter();
void fillPosAndDist();
void removeUnknowTagsAndFillPos();
void calculatePosWithCombination();




#endif /* SRC_POSITIEBEPALING_H_ */
