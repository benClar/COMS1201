//!
//
// Testing Module.  Due to the difficulties of fully testing the basic version's results,
// this testing module provides an interface to read in a set of test moves from the file
// and then explicitly check each element of a board for correctness following actioning 
// each one.  Additionally, I have included a flexible testing function to check values
// against what is expected and output script friendly specified error message to easily
// Identify which test has gone wrong.
//
//

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "testingModule.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Structures -----------*/

struct testData {
    int nMoves;     //!Number of moves
    int **dataArray;    //!array holding moves
};

/*---------- Functions ----------*/
/*
 * Suite of tests for this program
 */
void testing()  {
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
void testVal(int testResult,int expectedResult, char *description)  {

    if(testResult != expectedResult)    {
        fprintf(stderr,"###FAIL###: \"%s\" has failed \n",description);
        exit(1);
    } else {
        printf("\"%s\" : pass \n",description);
    }
}

/*
 *Initiate testing
 */
void startTest()    {
        testing();
        exit(0);
}

/*
 * Create Test Data Structure
 */
TestData createTestDataStr()    {

    TestData newTestData = (TestData) checkMalloc(malloc(sizeof(*newTestData)));
    newTestData->dataArray = NULL;
    newTestData->nMoves = 0;
	getTestData(newTestData);
    return newTestData;
}

/*
 *Returns address to test data structure
 */
TestData getTestData(TestData tData)	{

	static TestData currTdata;
	if(tData != NULL)	{
		currTdata = tData;
	}
	return currTdata;
}

/*
 *Add move type to test data structure
 */
void setTestDataArr(int row, int col, int move)	{

	getTestData(NULL)->dataArray[row][col] = move;

}

/*
 *Parsing third column of test data file input
 */
int parsingMoveType(char type, int *moveRow, int *moveCol)  {

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
            return 0;
    }
        return 1;
}

/*
 *Makes moves on one board and checks grid elements after to ensure correct state
 */
int testMakeMove(BoardNode testBoard)   {

    TestData tData = createTestDataStr();
    readMoveList(tData);
    int row,
    moveRow, moveCol,           //Set to desired row and col movement
    prevPosRow, prevPosCol,     //Saving old button position
    delButtonRow, delButtonCol, //Button to delete
    newPosRow, newPosCol;       //new Button
    for(row = 0; row < tData->nMoves; row++)    {
        if(!parsingMoveType(tData->dataArray[row][MOVETYPE],&moveRow,&moveCol)) {
            return 0;
        }

        prevPosRow = tData->dataArray[row][CURRROW];
        prevPosCol = tData->dataArray[row][CURRCOL];

        delButtonRow = tData->dataArray[row][CURRROW] + moveRow;
        delButtonCol = tData->dataArray[row][CURRCOL] + moveCol;

        newPosRow = tData->dataArray[row][CURRROW] + moveRow + moveRow;
        newPosCol = tData->dataArray[row][CURRCOL] + moveCol + moveCol;

        //! testing preconditions
        if(getButtonStatus(testBoard,delButtonRow,delButtonCol) != ALIVE)   {
            fprintf(stderr,"attempting to jump a non existent button\n");
            return 0;
        }

        if(getButtonStatus(testBoard,newPosRow,newPosCol) != DEAD) {
            fprintf(stderr,"attempting to jump to a non empty square\n");
            return 0;
        }
 
       if(getButtonStatus(testBoard,prevPosRow,prevPosCol) != ALIVE)   {
            fprintf(stderr,"No button to move\n");
            return 0;
        }

        makeMove(testBoard,moveRow,moveCol,tData->dataArray[row][CURRROW],tData->dataArray[row][CURRCOL],DELETE);

        //! testing post conditions
        if(getButtonStatus(testBoard,delButtonRow,delButtonCol) != DEAD)    {
            fprintf(stderr,"Button to be jumped has not been removed\n");
            return 0;
        }

        if(getButtonStatus(testBoard,newPosRow,newPosCol) != ALIVE) {
            fprintf(stderr,"Button has not been moved to correct place\n");
            return 0;
        }

        if(getButtonStatus(testBoard,prevPosRow,prevPosCol) != DEAD)    {
            fprintf(stderr,"Previous Position has not been removed\n");
            return 0;
        }
    }
    return 1;
}

/*
 *Increases the size of the array holding test data
 */
void growMoveDataArray(TestData testStr, int row)   {
    testStr->dataArray = (int**) checkMalloc(realloc(testStr->dataArray,(row + 1) * sizeof(int*)));
    testStr->dataArray[row] = (int*) checkMalloc(malloc(3*sizeof(int)));
    testStr->nMoves++;
}

/*
 *Reads list of moves in from file to dynamic array
 */
void readMoveList(TestData tData)   {

    FILE *fp;
    int move,row, col;
    if((fp = fopen("testMoves.txt","r")) != NULL)   {
        for(row = 0, col = 0; (move = getc(fp)) != EOF; )   {
            if(isalnum(move))   {
                if(col == 0)    {
                    growMoveDataArray(tData,row);
                }

                if( col < MOVETYPECOL)  {
                    move -= TOINT; 	//!converting first two columns to integer values
                }
                setTestDataArr(row,col,move);
                col++;
                if(col > MOVETYPECOL)   {
                    row++;
                    col = 0; 	//! resetting column
                }
            }
        }
    } else {
        fprintf(stderr,"sample move set data file does not exist \n");
    }
    fclose(fp);

}
