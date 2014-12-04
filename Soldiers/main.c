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

#include "boardModule.h"
#include "debug.h"
#include "main.h"

struct testData	{
	int nMoves; 	//!Number of moves
	int **dataArray;	//!array holding moves
};
/*---------- Main -----------*/

int main(int argc, char *argv[]){
	BoardNode currentBoard;
	checkArg(&argc);
	createQueue();
	userEnterTargetDestination(argv[1],argv[2]);	
	readDefaultMap();
	
//	testing();
	for(currentBoard = getStartBoard(); getFinalBoard() == NULL; currentBoard=nextInList(currentBoard))	{
		generatePossibleMove(currentBoard);
	}
	recursiveSuccess(getFinalBoard());
	return 0;

}

/*---------- Functions ----------*/

/*
 * Create Test Data Structure
 */
TestData createTestDataStr()	{

	TestData newTestData = (TestData) malloc(sizeof(*newTestData));
	newTestData->dataArray = NULL;
	newTestData->nMoves = 0;	
	return newTestData;
}


/*
 *Increases the size of the array holding test data 
 */
void growMoveDataArray(TestData testStr, int row)	{
	testStr->dataArray = (int**) realloc(testStr->dataArray,(row + 1) * sizeof(int*));
	testStr->dataArray[row] = (int*) malloc(3*sizeof(int));	
	testStr->nMoves++;
}	

/*
 * Ensures two arguments have been supplied
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
	testVal(testMakeMove(getStartBoard()),TRUE, "Test movement data on default board");
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

/*
 *Makes specified move in a recursive way
 */
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
 *Parsing third column of test data file input
 */
void parsingMoveType(char type, int *moveRow, int *moveCol)	{

   switch (type)    {
        case 'u':
            *moveRow = MVUP;
            *moveCol = DONTMV;
            break;
		case 'd':
			*moveRow = MVDOWN;
            *moveCol = DONTMV;
            break;
		case 'l':
            *moveRow = DONTMV;
            *moveCol = MVLEFT;
            break;
		case 'r':
            *moveRow = DONTMV;
            *moveCol = MVRIGHT;
            break;
        default:
            fprintf(stderr,"Unrecognised move direction\n");
            exit(1);
	}
}

/*
 *Makes moves on one board and checks grid elements after to ensure correct state
 */
int testMakeMove(BoardNode testBoard)	{

	TestData tData = createTestDataStr();
	readMoveList(tData);
	int row,
	moveRow, moveCol,			//Set to desired row and col movement
	prevPosRow, prevPosCol,		//Saving old button position
	delButtonRow, delButtonCol,	//Button to delete
	newPosRow, newPosCol;		//new Button
	for(row = 0; row < tData->nMoves; row++)	{
		parsingMoveType(tData->dataArray[row][MOVETYPE],&moveRow,&moveCol);

		prevPosRow = tData->dataArray[row][CURRROW];
		prevPosCol = tData->dataArray[row][CURRCOL];

		delButtonRow = tData->dataArray[row][CURRROW] + moveRow;
		delButtonCol = tData->dataArray[row][CURRCOL] + moveCol;

		newPosRow = tData->dataArray[row][CURRROW] + moveRow + moveRow;
		newPosCol = tData->dataArray[row][CURRCOL] + moveCol + moveCol;

		//! testing preconditions
		if(getButtonStatus(testBoard,delButtonRow,delButtonCol) != ALIVE)	{
			fprintf(stderr,"attempting to jump a non existent button\n");
			return 0;
		}
		
		if(getButtonStatus(testBoard,newPosRow,newPosCol) != DEAD) {
			fprintf(stderr,"attempting to jump to a non empty square\n");
			return 0;
		}
	
		if(getButtonStatus(testBoard,prevPosRow,prevPosCol) != ALIVE)	{
			fprintf(stderr,"No button to move\n");
			return 0;
		}
	
		makeMove(testBoard,moveRow,moveCol,tData->dataArray[row][CURRROW],tData->dataArray[row][CURRCOL],DELETE);
		
		//! testing post conditions
		if(getButtonStatus(testBoard,delButtonRow,delButtonCol) != DEAD)	{
			fprintf(stderr,"Button to be jumped has not been removed\n");
			return 0;
		}
	
		if(getButtonStatus(testBoard,newPosRow,newPosCol) != ALIVE)	{
			fprintf(stderr,"Button has not been moved to correct place\n");
			return 0;
		}
		
		if(getButtonStatus(testBoard,prevPosRow,prevPosCol) != DEAD)	{
			fprintf(stderr,"Previous Position has not been removed\n");
			return 0;
		}
	}
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

void readMoveList(TestData tData)	{

	FILE *fp;
	int move,row, col;
	if((fp = fopen("testMoves.txt","r")) != NULL)	{
		for(row = 0, col = 0; (move = getc(fp)) != EOF; )	{
			if(isalnum(move))	{
				if(col == 0)	{
					growMoveDataArray(tData,row);
				}

				if( col < MOVETYPECOL)	{
					move -= TOINT;
				}
				tData->dataArray[row][col] = move;
				col++;
				if(col > MOVETYPECOL)	{
					row++;
					col = 0; //! resetting column
				}
			}
		}	
	} else {
		fprintf(stderr,"sample move set data file does not exist \n");
	}	
	fclose(fp);

}
