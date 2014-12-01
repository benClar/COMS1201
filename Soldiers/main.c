//
// main.c
// Created by bclarke on 2014/29/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "boardModule.h"
#include "debug.h"
#include "main.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	
	createQueue();
	//userEnterTargetDestination();	
	readDefaultMap();
	testing();
	setTargetMove(0,0);
	//generatePossibleMove(getStartBoard());
	//copyParentToChild(getStartBoard(),addToQueue(createBoard(getStartBoard())));
	return 0;

}

/*---------- Functions ----------*/

/*
 * Suite of tests for this program
 */
void testing()	{
	printf("*** Entering Suite of Tests ***\n");

	testVal(testStartOfQueue(), TRUE,"First board in queue has default button layout");
	testVal(testCheckTarget(4,3), TRUE,"target is 3,3: should be alive");
	testVal(checkBounds(10,3), FALSE,"row 9 is out of height bounds");
	testVal(checkBounds(0,3), TRUE, "0,3 is in bounds");
	testVal(checkBounds(0,8), FALSE, "column 8 is out of length bounds");
	testVal(validateMove(getStartBoard(),MVUP,DONTMV,5,4,NEIGHDIS),TRUE, "5,4 to 3,4 is a valid move");
	testVal(validateMove(getStartBoard(),MVDOWN,DONTMV,5,4,NEIGHDIS),FALSE, "5,4 to 7,4 is not a valid move");
	testVal(testCopyParentToChild(getStartBoard(),copyParentToChild(getStartBoard(),addToQueue(createBoard(getStartBoard())))),TRUE, "Copying parent board to child board");
	printf("*** Testing finished ***\n");
}

/*
 * Tests return value of test functions against expected value
 */
void testVal(int testResult,int expectedResult, char *description)	{

	if(testResult != expectedResult)	{
		fprintf(stderr,"###FAIL###: \"%s\" has failed \n",description);
	} else {
		printf("\"%s\" : pass \n",description);
	}
}


int generatePossibleMove(BoardNode currentBoard)	{

	//! return true if target position has been reached
	if(checkTarget(currentBoard)) { return 1; }

	int row, col;

	for(row = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			if(getButtonStatus(currentBoard,row,col) == ALIVE)	{
				if(validateMove(currentBoard,DONTMV,MVRIGHT,row, col,NEIGHDIS))	{
					//!Create a  blank board, copy parent board to it, add it to the queue, 
					//!make the move then generate boards with possible moves again
					//generatePossibleMove(makeMove(addToQueue(createBoard(currentBoard))));
				} else if(validateMove(currentBoard,DONTMV,MVLEFT,row, col,NEIGHDIS))	{
	
				} else if(validateMove(currentBoard,MVUP,DONTMV,row,col,NEIGHDIS))	{

				} else if(validateMove(currentBoard,MVDOWN,DONTMV,row,col,NEIGHDIS))	{
			
				}
				//! no possible moves 
			}
		}
	}
	return 0;
}

int removeJumpedButton() {
return 1;
}

int makeMove(int currRow, int currCol) {
return 1;
}

/*
 *Validates a move in a given direction from current position
 */
int validateMove(BoardNode currentBoard, int moveRow, int moveCol, int currRow, int currCol, int distance)	{
	if(distance > MAXDISTOCHECK) { return 1;}
	if(checkBounds(currRow+moveRow,currCol+moveCol))	{
		switch(distance)	{
			case	NEIGHDIS:
					if(getButtonStatus(currentBoard,currRow+moveRow,currCol+moveCol) == ALIVE)	{
						if(validateMove(currentBoard,moveRow,moveCol,currRow+moveRow,currCol+moveCol,++distance))	{
							return 1;
						}
					}
					break;
			case	MAXDISTOCHECK:
					if(getButtonStatus(currentBoard,currRow+moveRow,currCol+moveCol) == DEAD)	{
						if(validateMove(currentBoard,moveRow,moveCol,currRow+moveRow,currCol+moveCol,++distance))	{
							return 1;
						}
					}
					break;
			default:	
		
					return 0;
		}
	}
	return 0;
}

/*
 *	User enters target destination for counter
 */
void userEnterTargetDestination()	{
	int row, col;	
	printf("Please enter your target x coordinate:	");
	scanInt(&col,MAXCOL,MINCOORD);
	pNL();
	printf("Please enter your target y coordinate:	");
	scanInt(&row,MAXROW,MINCOORD);
	setTargetMove(row,col);
}

/*
 *  Safely Scans integers from keyboard within specified range
 */
void scanInt(int *toScan, int max, int min) {

	do  {
        while(!scanf("%d", toScan))       {
               clearInputBuf
               printf("please enter integer\n");
        }
		if(*toScan > max)	{
			printf("Please ensure integer entered is less than %d\n",max);
		} else if (*toScan < min)	{
			printf("Please ensure integer entered is greater than %d\n",min);
		}
	} while (*toScan > max || *toScan < min);

}

void readDefaultMap()	{
	BoardNode defBoard = addToQueue(createBoard(NULL));
	FILE *fp;
	int button, col, row;
	if((fp = fopen("defaultBoard.txt","r")) != NULL) {
		for(col = 0, row = 0; (button = getc(fp)) != EOF ;)	{
			if(col == MAXCOL)	{
				col = 0;
				row++;
			}
			if(button == '1' || button == '0')	{
				button -= TOINT; //!converting to integer
				addButtonToBoard(defBoard,button, row, col);
				col++;
			}
		}
	}
}
