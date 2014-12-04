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
	BoardNode currentBoard;
	checkArg(&argc);
	createQueue();
	userEnterTargetDestination(argv[1],argv[2]);	
	readDefaultMap();
	printQueue();
	
	//testing();
	for(currentBoard = getStartBoard(); getFinalBoard() == NULL; currentBoard=nextInList(currentBoard))	{
		generatePossibleMove(currentBoard);
	}
	printQueue();
	printSuccessSeries();
	return 0;

}

/*---------- Functions ----------*/

/*
 *
 */
void checkArg(int *argc){
	(*argc)--;
	if(*argc != ARGCNT)	{
		fprintf(stderr,"Please input set of x,y coordinates of target position\n");
		exit(1);
	}
}
/*
 * Suite of tests for this program
 */
void testing()	{
	printf("*** Entering Suite of Tests ***\n");

	testVal(testStartOfQueue(), TRUE,"First board in queue has default button layout");
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

	int row, col;

	for(row = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			if(getButtonStatus(currentBoard,row,col) == ALIVE)	{
				if(validateMove(currentBoard,DONTMV,MVRIGHT,row, col,NEIGHDIS))	{
					generateUniqueBoardWithMove(currentBoard,DONTMV,MVRIGHT,row,col);
				} else if(validateMove(currentBoard,DONTMV,MVLEFT,row, col,NEIGHDIS))	{
					generateUniqueBoardWithMove(currentBoard,DONTMV,MVLEFT,row,col);
				} else if(validateMove(currentBoard,MVUP,DONTMV,row,col,NEIGHDIS))	{
					generateUniqueBoardWithMove(currentBoard,MVUP,DONTMV,row,col);
				} else if(validateMove(currentBoard,MVDOWN,DONTMV,row,col,NEIGHDIS))	{
					generateUniqueBoardWithMove(currentBoard,MVDOWN,DONTMV,row,col);
				}
			}
		}
	}
	return 1;
}

void generateUniqueBoardWithMove(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol)	{

	//!Create a  blank board, copy parent board to it, make the move,  check if it
	//!is unique, add it to queue 
	BoardNode generatedBoard;

	if((generatedBoard = compBoardWithList(makeMove(copyParentToChild(currentBoard,createBoard(currentBoard)),rowMove,colMove,currRow,currCol,DELETE))) != NULL)	{
		addToQueue(generatedBoard);	
		checkTarget(generatedBoard);
	} else {
		freeBoard(generatedBoard);
	}


	}

BoardNode makeMove(BoardNode newBoard, int moveRow, int moveCol, int currRow, int currCol, moveStage currStep) {
	if(currStep == DONE)	{ return newBoard; }
	switch(currStep)	{
	
	case DELETE:
				changeButton(newBoard,currRow,currCol,DEAD); //!removing current button
				currStep = JUMP;
				changeButton(newBoard,currRow+moveRow,currCol+moveCol,DEAD); //!deleting button to be jumped
				break;
	case JUMP:
				changeButton(newBoard,currRow+moveRow,currCol+moveCol,ALIVE); //! placing button in new spot
				currStep = DONE;
				break;
	default:
				fprintf(stderr,"Unrecognised step\n");
				break;
	}

	return(makeMove(newBoard,moveRow,moveCol,currRow+moveRow,currCol+moveCol,currStep));
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
void userEnterTargetDestination(char *x, char *y)	{
	int row, col;
	row = checkInt(atoi(checkEnteredString(y)),MAXROW,MINCOORD);
	col = checkInt(atoi(checkEnteredString(x)),MAXCOL,MINCOORD) - 1;
	iprint(row);
	iprint(col);
	setTargetMove(row,col);
}

/*
 *  Safely Scans integers from keyboard within specified range
 */
int checkInt(int toCheck, int max, int min) {

		if(toCheck > max)	{
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
