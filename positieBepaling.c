/*
 * positieBepaling.c
 *
 *  Created on: 24-apr.-2018
 *      Author: seppe
 */

#include "positieBepaling.h"
struct RFIDData *head = NULL;
long tagPos[3][2] = {{-1,-1},{-1,-1},{-1,-1}};
long int tagDist[3] = {-1,-1,-1};
int weight[3] = {1,1,1};


/*
 * Calculation of inverse of matrix
 * http://www.ccodechamp.com/c-program-to-find-inverse-of-matrix/
 */
long getDeterminant(long **matrix, int orde){
	long det = 0;
	long **temp = NULL;
	int s = 1,i,j,m,n,c;
	if(orde == 1){
		det = matrix[0][0];
	}
	else if(orde ==2){
		det = matrix[0][0]*matrix[1][1]- matrix[1][0]*matrix[0][1];
	}
	else{
		det = 0;
		for(c = 0; c<orde;c++){
			m = 0;
			n = 0;
			//allocate temp matrix
			temp = malloc(orde*sizeof(long *));
			for(i = 0;i<orde;i++){
				temp[i] = malloc(orde*sizeof(long));
			}
			//end allocation of temp matrix
			for(i=0;i<orde;i++){
				for(j=0;j<orde;j++){
					temp[i][j]=0;
					if(i!=0 && j!=c){
						temp[m][n] = matrix[i][j];
						if(n<(orde-2))
							n++;
						else{
							n = 0;
							m++;
						}
					}
				}

			}
			det = det+s*(matrix[0][c]* getDeterminant(temp,orde-1));
			s=-1*s;
			//free temp matrix
			for(i = 0;i<orde;i++){
				free(temp[i]);
			}
			free (temp);
			//end free of temp matrix
		}
	}
	return det;
}


/*
 * calculate inverse given matrix with given orde and place in inverse matrix
 * return 1 if possible
 * return -1 if not
 */
int getInverse(long **matrix, double **inverse, int orde){
	double det = 0;
	long **temp = NULL;
	long **cofactor = NULL;
	int p,q,i,j,m,n;

	det = getDeterminant(matrix,orde);
	if(det == 0){
		return -1;
	}
	//allocate cofactor matrix
	cofactor = malloc(orde*sizeof(long *));
	for(i = 0;i<orde;i++){
		cofactor[i] = malloc(orde*sizeof(long *));
	}
	//end allocation of cofactor matrix

	for(q = 0;q<orde;q++){
		for(p = 0; p<orde;p++){
			m = 0;
			n = 0;

			//allocate temp matrix
			temp = malloc(orde*sizeof(long *));
			for(i = 0;i<orde;i++){
				temp[i] = malloc(orde*sizeof(long));
			}
			//end allocation of temp matrix

			for(i=0;i<orde;i++){
				for(j=0;j<orde;j++){
					if(i!=q && j!=p){
						temp[m][n] = matrix[i][j];
						if(n<(orde-2)){
							n++;
						}
						else{
							n = 0;
							m++;
						}
					}
				}
			}
			cofactor[p][q] = pow((double)-1,(double)q+p+2) * getDeterminant(temp,orde-1); // get cofactor and transpose in one step
			//free temp matrix
			for(i = 0;i<orde;i++){
				free(temp[i]);
			}
			free (temp);
			//end free of temp matrix
		}
	}

	//finding transpose of matrix and devine by the determinat to get inverse matrix of matrix
	for(i=0;i<orde;i++){
		for(j = 0;j<orde;j++){
			inverse[i][j] = cofactor[i][j]/det;
		}
	}
	//free cofactor matrix
	for(i = 0;i<orde;i++){
		free(cofactor[i]);
	}
	free (cofactor);
	//end free of cofactor matrix

	return 1;
}



/*
 * calculate the position with the use of trilateration
 *
 * pos 		--> calculated position 	[ x-pos y-pos]
 * tagPos 	--> position of tags 		[x-pos1 y-pos1,
 * 										 x-pos2 y-pos2,
 * 										 x-pos3 y-pos3]
 * tagDist 	--> distance from tag to pos [dist1, dist2, dist3]
 * count	--> count of different trilaterations
 */
void trilateration(long pos[MAX_SUB_POSITIONS][2], long tagPos[3][2], long int tagDist[3], int count){
	int dimension = 2;//sizeof(tagPos[0])/sizeof(tagPos[0][0]);
	int numberOfTags = 3;//sizeof(tagPos)/(sizeof(tagPos[0]));
//	int numberOfDist = sizeof(tagDist)/(sizeof(tagDist[1]));
	long **A = NULL;
	double **invA = NULL;
	long *B = NULL;
	int i,j;


	//if number of tags is not number of dist: return -1,-1 --> fault
//	if(numberOfDist != numberOfTags){
//		pos[0] = -1;
//		pos[1] = -1;
//		return;
//	}

	//allocate A and B matrix
	A = malloc(numberOfTags*sizeof(long *));
	for(i = 0;i<numberOfTags;i++){
		A[i] = malloc((dimension+1)*sizeof(long));
	}
	B = malloc(numberOfTags*sizeof(long));
	//end allocation of A and B matrix


	for(i = 0; i<numberOfTags;i++){
		//A = 1  -2x   -2y  (-2z)
		A[i][0] = 1;
		for(j = 1;j<dimension+1;j++){
			A[i][j] = tagPos[i][j-1]*(-2);
		}
		//B = dist^2-x^2-y^2 (-z^2)
		B[i] = tagDist[i]*tagDist[i];
		for(j = 0;j<dimension;j++){
			B[i] = B[i] - tagPos[i][j]*tagPos[i][j];
		}
	}
	if(numberOfTags == 3){
		pos[count-1][0]=0;
		pos[count-1][1]=0;
		invA = malloc(numberOfTags*sizeof(double *));
		for(i = 0;i<numberOfTags;i++){
			invA[i] = malloc(numberOfTags*sizeof(double));
		}
		if(getInverse(A,invA,numberOfTags)==-1){
		//fault in getInverse
			//pos[0] = -1;
			//pos[1] = -1;
			//return;
		}
		/*
		 * calculate coordinates from A*X = B
		 * with X = {x^2 + y^2
		 * 				x			--> pos[0]
		 * 				y	   }	--> pos[1]
		 * calculate X with invA*B = X
		 */
		for(i = 0;i<numberOfTags; i++){
			pos[count-1][0] = pos[count-1][0]+ invA[1][i]*B[i]; //x-value of coordinates
			pos[count-1][1] = pos[count-1][1]+ invA[2][i]*B[i]; //y-value of coordinates
		}
		return;
	}
	else{
		pos[count-1][0] = -1;
		pos[count-1][1] = -1;
		return;
	}


}



int getMillimeterLineair(int rssi)
{
static const  int lookup[61] =
{
	0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,
	0,		0,		0,		0,		90,
	194,	298,	402,	506,	611,
	715,	819,	923,	1027,	1131,
	1235,	1339,	1443,	1547,	1651,
	1755,	1859,	1964,	2068,	2172,
	2276,	2380,	2484,	2588,	2692
};
int mm = lookup[rssi];
return mm;
}

int getMillimeterExp(int rssi)
{
static const  int lookup[61] =
{
	9,
	10,		11,		12,		14,		15,
	17,		19,		21,		23,		25,
	28,		31,		35,		39,		43,
	48,		53,		59,		65,		72,
	80,		89,		99,		110,	122,
	135,	150,	167,	185,	206,
	229,	254,	282,	313,	347,
	386,	428,	475,	528,	568,
	651,	722,	802,	890,	988,
	1097,	1218,	1353,	1502,	1668,
	1851,	2056,	2282,	2534,	2813,
	3124,	3468,	3850,	4275,	4746
};
int mm;
if(rssi>0&&rssi<61){
	mm = lookup[rssi];
}
else{
	mm = -1;
}
return mm;
}





//insert link at the first location
void insertFirst(unsigned long long EPC1, unsigned long long EPC2, int rssi){
	//create a link
	struct RFIDData *link = (struct RFIDData*) malloc(sizeof(struct RFIDData));

	link->EPC1 = EPC1;
	link->EPC2 = EPC2;
	link->rssi = rssi;

	//point to old first node
	link->next = head;

	//point first node to the new first node
	head = link;
}
/*
 * compare EPC of two RFIDData structures
 * return -1 	if EPC of RFID1 is 	smaller 	then EPC of RFID2
 * return 0 	if EPC of RFID1 is 	equal 		to EPC of RFID2
 * return 1 	if EPC of RFID1 is 	greater 	then EPC of RFID2
 *
 * RFID1 EPC: AB
 * RFID2 EPC: CD
 * 		A>C		A=C		A<C
 * B>D	1		 1		-1
 * B=D 	1		 0		-1
 * B<D	1		 -1		-1
 */
int compareEPC(struct RFIDData *RFID1, struct RFIDData *RFID2){
	if(RFID1->EPC1 == RFID2->EPC1){
		if(RFID1->EPC2 == RFID2->EPC2){
			return 0;
		}
		else if(RFID1->EPC2 < RFID2->EPC2){
			return -1;
		}
		else return 1;
	}
	else if(RFID1->EPC1 < RFID2->EPC1){
		return -1;
	}
	else return 1;

}


/*
 * delete first item
 */
void deleteFirst(){
	//save reference to first link
	struct RFIDData *tempLink = head;

	//mark next to first link as first
	head = head->next;

	//free deleted link
	free(tempLink);
}
/*Delete a specific next node in the linked list
 * current --> current node with the next to be deleted
 */
void deleteNextNode(struct RFIDData *current){
	struct RFIDData *next = current->next;
	if(next != NULL){
		if(next->next == NULL){
			current->next = NULL;
			free(next);
		}
		else{
			current->next = next->next;
			free(next);
		}
	}
	return;
}
/*
 *Zoek lengte van linked list
 */
int length(){
	int lenght = 0;
	struct RFIDData *current;

	for(current = head; current != NULL; current = current->next){
		lenght++;
	}
	return lenght;
}

/*
 * sorteer linked list op EPC
 */
void sort(){
	int i, j, k, tempRssi, compEPC;
	unsigned long long tempEPC1, tempEPC2;
	struct RFIDData *current;
	struct RFIDData *next;

	int size = length();
	k = size-1;//-1 anders te ver --> verkeerde data in linked list

	for(i = 0; i<size-1; i++,k--){
		current = head;
		next = head->next;

		for(j=0; j<k; j++){
			compEPC = compareEPC(current, next);
			if(compEPC==1){
				tempEPC1 = current->EPC1;
				tempEPC2 = current->EPC2;
				current->EPC1 = next->EPC1;
				current->EPC2 = next->EPC2;
				next->EPC1 = tempEPC1;
				next->EPC2 = tempEPC2;

				tempRssi = current->rssi;
				current->rssi = next->rssi;
				next->rssi = tempRssi;
			}
			else {
				if(compEPC == 0){ // zelfde tag dan sorteren op rssi van klein naar groot
					if(current->rssi <= next->rssi){
						tempRssi = current->rssi;
						current->rssi = next->rssi;
						next->rssi = tempRssi;
					}
				}
			}
			current = current->next;
			next = next->next;

		}
	}
}

void getSmallestRssi(){
	struct RFIDData *current;
	struct RFIDData *next;

	current = head;
	next = head->next;

	while(current->next!=NULL){
		while(compareEPC(current, next) == 0){
			if(next->rssi <= current->rssi){
				deleteNextNode(current); //delete next node after current
				next = current->next;
			}
		}
		if(current->next == NULL){
			return;
		}
		current = current->next;
		next = current->next;

	}
}

void getAverageRssi(){
	struct RFIDData *current;
	struct RFIDData *next;
	int sum;
	int count;

	current = head;
	next = head->next;

	while(current->next!=NULL){
		sum = current->rssi;
		count = 1;
		while(compareEPC(current, next) == 0){
			sum = sum + next->rssi;
			count++;
			deleteNextNode(current); //delete next node after current
			next = current->next;
		}

		current->rssi = sum/count; // get average of one tag
		if(current->next == NULL){
			return;
		}
		current = current->next;
		next = current->next;
	}

}

void changeRssiToMillimeter(){
	//int size = length();
	struct RFIDData *current;
	//struct RFIDData *next;

	current = head;
	//next = head->next;
	do{

		//current->dist = getMillimeterLineair(current->rssi*-1);
		current->dist = getMillimeterExp(current->rssi*-1);
		current = current->next;
		//next = current->next;
	}
	while(current!=NULL);

}

/*
 * fill position matrix with the three first tags
 */
void fillPosAndDist(){
	unsigned long long EPC1, EPC2;
	bool found;
	int numberOfTags = sizeof(tags)/sizeof(tags[0]);
	struct RFIDData *current = head;
	//next = current-> next;
	int j = 0;
	for(int i = 0; i <3; i++){
		EPC1 = current->EPC1;
		EPC2 = current->EPC2;
		do{
			if(EPC1 == tags[j][0] && EPC2 == tags[j][1]){
				tagPos[i][0] = tags[j][2];
				tagPos[i][1] = tags[j][3];
				tagDist[i] = current->dist;
				found = true;
			}
			j++;
		}
		while(j<numberOfTags && !found);
		if(!found){
			//no tag found with correct EPC code
			tagPos[i][0] = -1;
			tagPos[i][1] = -1;
			tagDist[i]   = -1;
		}
		j = 0;
		found = false;
		current = current->next;
	}
}

/*
 * deleted tags that are not in the taglist
 * fill in pos if known tag
 */

void removeUnknowTagsAndFillPos(){
	unsigned long long EPC1, EPC2;
	bool found;
	int numberOfTags = sizeof(tags)/sizeof(tags[0]);
	struct RFIDData *current = NULL;
	struct RFIDData *next = head;
	int j = 0;

	do{
		EPC1 = next->EPC1;
		EPC2 = next->EPC2;
		found = false;
		j = 0;

		do{
			if(EPC1 == tags[j][0] && EPC2 == tags[j][1]){
				found = true;
			}
			j++;
		}
		while(j<numberOfTags && !found);
		if(!found){
		//no tag found with correct EPC code: delete tag
			if(current != NULL){ // wrong tag is not the head tag
				SegmentLCD_Write("DEL1");
				deleteNextNode(current);
				SegmentLCD_Write("DEL2");
				next = current->next;
			}
			else{	//wrong tag is head tag
				deleteFirst();
				next = head;
			}

		}
		else {		//tag found fill in xPos and yPos
			next->xPos = tags[j-1][2]; //correction of j++
			next->yPos = tags[j-1][3]; //correction of j++
			current = next;
			next = current->next;
		}
	}
	while(next!=NULL);
}
/*
 *
 */
void calculatePosWithCombination(){
	struct RFIDData *tag1 = head;
	struct RFIDData *tag2 = head->next;
	struct RFIDData *tag3 = head->next->next;
	int xPos = 0;
	int yPos = 0;
	int count = 0;
	int correctCount = 0;			//count of correct calculated values
	if(tag1 == NULL){
		pos[0][0] = -1;
		pos[0][1] = -1;
		return;
	}
	if(tag2 == NULL){//pos is tagPos of head
		pos[0][0] = tag1->xPos;
		pos[0][1] = tag1->yPos;
		return;
	}
	/*
	 * pos is distance average
	 * example:
	 * xpos: 900 ----------------------------------------------------- 1000
	 * dist: 70														   40
	 * solution  -----------------------------------963---------------
	 */
	if(tag3 == NULL){
		pos[0][0] = tag1->xPos+(tag2->xPos-tag1->xPos)*((double)tag1->dist/(tag1->dist+tag2->dist));
		pos[0][1] = tag1->yPos+(tag2->yPos-tag1->yPos)*((double)tag1->dist/(tag1->dist+tag2->dist));
		return;
	}
	/*
	 * calculate position with a combination of different groups of 3 tags
	 * with the use of trilateration
	 *
	 * first fill in used tagpos and distances
	 */
	tagPos[0][0] = tag1->xPos;
	tagPos[0][1] = tag1->yPos;
	tagDist[0] = tag1->dist;
	tagPos[1][0] = tag2->xPos;
	tagPos[1][1] = tag2->yPos;
	tagDist[1] = tag2->dist;
	tagPos[2][0] = tag3->xPos;
	tagPos[2][1] = tag3->yPos;
	tagDist[2] = tag3->dist;

	/*
	 * tag1 -> tag2 -> tag3 -> tag4 -> tag5 -> NULL	  count
	 *  +        +       +								1
	 *  +        +              +						2
	 *  +        +                      +				3
	 *  +                +      +						4
	 *  +                +              +				5
	 *  +                       +       +				6
	 *           +       +      +						7
	 *           +       +              +				8
	 *           +              +       +				9
	 *                   +      +       +				10
	 */
	while(tag3!= NULL || tag2->next != tag3 || tag1->next != tag2){
		while(tag3!=NULL || tag2->next!=tag3){
			while(tag3!=NULL){
				count++;
				tagPos[2][0] = tag3->xPos;
				tagPos[2][1] = tag3->yPos;
				tagDist[2] = tag3->dist;
				trilateration(pos, tagPos, tagDist, count);

				tag3 = tag3->next;
			}

			tag2 = tag2->next;
			tag3 = tag2->next;
			tagPos[1][0] = tag2->xPos;
			tagPos[1][1] = tag2->yPos;
			tagDist[1] = tag2->dist;
		}
		tag1 = tag1->next;
		tag2 = tag1->next;
		tag3 = tag2->next;

		tagPos[0][0] = tag1->xPos;
		tagPos[0][1] = tag1->yPos;
		tagDist[0] = tag1->dist;
		tagPos[1][0] = tag2->xPos;
		tagPos[1][1] = tag2->yPos;
		tagDist[1] = tag2->dist;
		tagPos[2][0] = tag3->xPos;
		tagPos[2][1] = tag3->yPos;
		tagDist[2] = tag3->dist;
	}

	//calculate average of measured values
	for(int i = 0; i<count; i++){
		if(pos[count][0]!=-1||pos[count][1]!=-1){		//count only correct calculated values --> xPos and yPos are not -1
			xPos = xPos + pos[i][0];				//sum of correct calculated xPos values
			yPos = yPos + pos[i][1];				//sum of correct calcualted yPos values
			correctCount++;								//count of correct values
		}
	}
	pos[0][0] = xPos/correctCount;					//take average of correct xPos values
	pos[0][1] = yPos/correctCount;					//take average of correct yPos values
	return;





}
/*-------------------------------------------------------------------------

 * einde test gelinkte lijst van inkomende data
 */



