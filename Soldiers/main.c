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

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	BoardNode currentBoard;
	createQueue();
	checkArg(&argc,argv);
	userEnterTargetDestination(argv[1],argv[2]);	
	readDefaultMap();
	currentBoard = getStartBoard();
	for(currentBoard = getStartBoard(); currentBoard != NULL && getFinalBoard() == NULL; currentBoard=nextInList(currentBoard))	{
		if(getMode() == BHASH)	{
			bitDecoder(currentBoard);
			generatePossibleMove(currentBoard);
		} else {
			generatePossibleMove(currentBoard);
		}
	}

	freeHashingStructures();

	if(getFinalBoard() != NULL)	{
		if(getMode()==BHASH)	{
			decodeFinalSeries();
		}
	//	start_SDL();
		recursiveSuccess(getFinalBoard());
		freeQueue();
		return 0;
	} else {
		freeQueue();
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
	} else if(*argc > 2 && !strcmp("zhash",args[3])) {
		printf("Zobrist Hashing Mode Enabled\n");
		initZobrist();
		createHashTable();
		setMode(ZHASH);
	} else if(*argc > 2 && !strcmp("bhash",args[3]))	{
		printf("Bit Hashing Mode Enabled\n");
		//initBitHashValues();
		createBitHashTable();
		setMode(BHASH);
	}
	if(*argc > ARGCNT)	{
		fprintf(stderr,"Please run program with any of the following options: \n ./pOut test \n ./pOut [row] [col] \n ./pOut [row] [col] zhash\n ./pOut [row] [col] bhash\n");
		exit(1);
	}
}

/*
 *	User enters target destination for counter
 */
void userEnterTargetDestination(char *sCol, char *sRow)	{
	int row, col;
	row = rowToY(checkInt(atoi(checkEnteredString(sRow)),MAXROW+1,MINCOORD));
	col = checkInt(atoi(checkEnteredString(sCol)),MAXCOL+1,MINCOORD) - 1;
	setTargetMove(row,col);
}

int rowToY(int row)	{

		return(MAXROW - row);
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

/*
 * Further string validation
 */
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

/*
 *Reads in default map
 */
void readDefaultMap()	{
	BoardNode defBoard = addToQueue(createBoard(NULL));
	FILE *fp;
	int button, col, row, defaultSuccess = 0;
	while(!defaultSuccess)	{
		if((fp = fopen("defaultBoard.txt","r")) != NULL) {
			defaultSuccess = 1;
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
			fclose(fp);
		} else {
			fclose(fp);
			fprintf(stderr,"Default board file does not exist. Attempting to create one.\n");
			if((fp = fopen("defaultBoard.txt","w+")) != NULL) { 
				fprintf(fp,"0000000\n0000000\n0000000\n0000000\n1111111\n1111111\n1111111\n1111111");
				fclose(fp);
			} else {
				fprintf(stderr,"Creating new file failed.  Please place file in current directory called default.txt with following contents: \n 0000000\n0000000\n0000000\n0000000\n1111111\n1111111\n1111111\n1111111\n");
			}
		}
	}
	//!Extension: Encodes map if extension mode specified
	if(getMode() == BHASH)	{
		bitEncoder(defBoard);
	}
}
