//
// main.c
// Created by bclarke on 2014/29/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*struct testData	{
	int nMoves; 	//!Number of moves
	int **dataArray;	//!array holding moves
};*/
/*---------- Main -----------*/

int main(int argc, char *argv[]){
	BoardNode currentBoard;
	createQueue();
	readDefaultMap();
	checkArg(&argc,argv);
	initZobrist();
	createHashTable();
	userEnterTargetDestination(argv[1],argv[2]);	
	for(currentBoard = getStartBoard(); currentBoard != NULL && getFinalBoard() == NULL; currentBoard=nextInList(currentBoard))	{
		generatePossibleMove(currentBoard);
	}

	freeHashingStructures();

	if(getFinalBoard() != NULL)	{
		start_SDL();
		recursiveSuccess(getFinalBoard());
		freeQueue();
		return 0;
	} else {
//		freeQueue();
		fprintf(stderr,"Solution could not be found\n");
		return 1;
	}
}

/*---------- Functions ----------*/


/*
 * Ensures two arguments have been supplied
 */
void checkArg(int *argc,char **args){
	(*argc)--;
	if(*argc < 1)	{
		fprintf(stderr,"Please use test parameter to run testing or specify two x y coordinates\n");
		exit(1);
	}
	if(!strcmp("test",args[1])){
		startTest();
	} else if(*argc > 2 && !strcmp("hash",args[3])) {
		printf("Hash Mode Enabled\n");
		startHashing();
	}
	if(*argc > ARGCNT)	{
		fprintf(stderr,"Please run program with any of the following options: \n ./pOut test \n ./pOut [row] [col] \n ./pOut [row] [col] hash\n");
		exit(1);
	}
}

/*
 *	User enters target destination for counter
 */
void userEnterTargetDestination(char *sRow, char *sCol)	{
	int row, col;
	row = checkInt(atoi(checkEnteredString(sRow)),MAXROW,MINCOORD);
	col = checkInt(atoi(checkEnteredString(sCol)),MAXCOL,MINCOORD);
	setTargetMove(row,col);
}

/*
 *  Safely Scans integers from keyboard within specified range
 */
int checkInt(int toCheck, int max, int min) {

		if(toCheck >= max)	{
			printf("Please ensure integer entered is less than %d\n",max);
			exit(1);
		} else if (toCheck < min)	{
			printf("Please ensure integer entered is greater than %d\n",min);
			exit(1);
		}

		return toCheck;
}

char *checkEnteredString(char *toCheck)	{

	int ele;
	for(ele=0; toCheck[ele] != '\0'; ele++)	{
		if(toCheck[ele] < '0' || toCheck[ele] > '9')	{
			fprintf(stderr,"You have supplied a non integer as a target destination \n");
			exit(1);
		} 	
	}
		return toCheck;
}

void readDefaultMap()	{
	BoardNode defBoard = addToQueue(createBoard(NULL));
	FILE *fp;
	int button, col, row;
	if((fp = fopen("defaultBoard.txt","r")) != NULL) {
		for(col = 0, row = 0; (button = getc(fp)) != EOF ;)	{
			if(col == MAXCOL)	{
				col = 0; //resetting column
				row++;
			}
			if(button == '1' || button == '0')	{
				button -= TOINT; //!converting to integer
				addButtonToBoard(defBoard,button, row, col);
				col++;
			}
		}
	} else {
		fprintf(stderr,"Default board file does not exist\n");
	}
	fclose(fp);
}
