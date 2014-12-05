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

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*struct testData	{
	int nMoves; 	//!Number of moves
	int **dataArray;	//!array holding moves
};*/
/*---------- Main -----------*/

int main(int argc, char *argv[]){
	start_SDL();
	BoardNode currentBoard;
	createQueue();
	readDefaultMap();
	checkArg(&argc,argv);
	
	userEnterTargetDestination(argv[1],argv[2]);	
	readDefaultMap();
	
	for(currentBoard = getStartBoard(); getFinalBoard() == NULL; currentBoard=nextInList(currentBoard))	{
		generatePossibleMove(currentBoard);
	}
	recursiveSuccess(getFinalBoard());
	return 0;

}

/*---------- Functions ----------*/


/*
 * Ensures two arguments have been supplied
 */
void checkArg(int *argc,char **args){
	(*argc)--;
	if(!strcmp("test",args[1])){
		startTest();
	}
	if(*argc != ARGCNT)	{
		fprintf(stderr,"Please input set of x,y coordinates of target position\n");
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
	iprint(row);
	iprint(col);
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
