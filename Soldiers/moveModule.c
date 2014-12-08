/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "moveModule.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Functions ----------*/

/*
 *Validates a move in a given direction from current position
 */
int validateMove(BoardNode currentBoard, int moveRow, int moveCol, int currRow, int currCol, int distance)  {
    if(distance > MAXDISTOCHECK) { return 1;}
    if(checkBounds(currRow+moveRow,currCol+moveCol))    {
        switch(distance)    {
            case    NEIGHDIS:
                    if(getButtonStatus(currentBoard,currRow+moveRow,currCol+moveCol) == ALIVE)  {
                        if(validateMove(currentBoard,moveRow,moveCol,currRow+moveRow,currCol+moveCol,++distance))   {
                            return 1;
                        }
                    }
                    break;
            case    MAXDISTOCHECK:
                    if(getButtonStatus(currentBoard,currRow+moveRow,currCol+moveCol) == DEAD)   {
                        if(validateMove(currentBoard,moveRow,moveCol,currRow+moveRow,currCol+moveCol,++distance))   {
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
 *Makes specified move in a recursive way
 */
BoardNode makeMove(BoardNode newBoard, int moveRow, int moveCol, int currRow, int currCol, moveStage currStep) {
    if(currStep == DONE)    { return newBoard; }
    switch(currStep)    {

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
 * Calls functions required for generating moves depending on program mode
 */
int generatePossibleMove(BoardNode currentBoard)    {

    int row, col;

    for(row = 0; row < MAXROW; row++)   {
        for(col = 0; col < MAXCOL; col++)   {
            if(getButtonStatus(currentBoard,row,col) == ALIVE)  {
                if(validateMove(currentBoard,DONTMV,MVRIGHT,row, col,NEIGHDIS)) {
					if(getMode() == ZHASH)	{
                   		generateUniqueBoardHash(currentBoard,DONTMV,MVRIGHT,row,col);
					} else if(getMode() == BHASH)	{
                   		generateUniqueBoardBitHash(currentBoard,DONTMV,MVRIGHT,row,col);
					} else {
                   		generateUniqueBoardWithMove(currentBoard,DONTMV,MVRIGHT,row,col);
					}
                } else if(validateMove(currentBoard,DONTMV,MVLEFT,row, col,NEIGHDIS))   {
					if(getMode() == ZHASH)	{
                    	generateUniqueBoardHash(currentBoard,DONTMV,MVLEFT,row,col);
					} else if(getMode() == BHASH)	{
                    	generateUniqueBoardBitHash(currentBoard,DONTMV,MVLEFT,row,col);
					} else {
                    	generateUniqueBoardWithMove(currentBoard,DONTMV,MVLEFT,row,col);
					}
                } else if(validateMove(currentBoard,MVUP,DONTMV,row,col,NEIGHDIS))  {
					if(getMode() == ZHASH)	{
                    	generateUniqueBoardHash(currentBoard,MVUP,DONTMV,row,col);
					} else if(getMode() == BHASH)	{
                    	generateUniqueBoardBitHash(currentBoard,MVUP,DONTMV,row,col);
					} else {
                    	generateUniqueBoardWithMove(currentBoard,MVUP,DONTMV,row,col);
					}
                }

	 			if(getMode() == LINEAR)	{
					if(validateMove(currentBoard,MVDOWN,DONTMV,row,col,NEIGHDIS))    {
       					   	generateUniqueBoardWithMove(currentBoard,MVDOWN,DONTMV,row,col);
      				}
				}
            }
        }
    }
    return 1;
}

/*
 * Extension: Checks and adds to hash table using a bit ID based hash function
 */
void generateUniqueBoardBitHash(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol)	{
	BoardNode generatedBoard;
	generatedBoard = makeMove(copyParentToChild(currentBoard,createBoard(currentBoard)),rowMove,colMove,currRow,currCol,DELETE);
	if(generateBitHashKey(generatedBoard))	{
		addToQueue(generatedBoard);
		checkTarget(generatedBoard);
	}
}

/*
 * Extension: Checks and adds to hash table using Zobrist hash function instead of linear linked list queue
 */
void generateUniqueBoardHash(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol)    {

	int hashKey;
    BoardNode generatedBoard;
	generatedBoard = makeMove(copyParentToChild(currentBoard,createBoard(currentBoard)),rowMove,colMove,currRow,currCol,DELETE);
	hashKey = generateHashKey(generatedBoard);
	if(hashBoard(hashKey,generatedBoard))	{
		addToQueue(generatedBoard);
		checkTarget(generatedBoard);
	}
}


/*
 *Basic: adds unqiue boards to linear linked list queue
 */
void generateUniqueBoardWithMove(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol)    {

    //!Create a  blank board, copy parent board to it, make the move,  check if it
    //!is unique, add it to queue
    BoardNode generatedBoard;

    if((generatedBoard = compBoardWithList(makeMove(copyParentToChild(currentBoard,createBoard(currentBoard)),rowMove,colMove,currRow,currCol,DELETE))) != NULL)    {
        addToQueue(generatedBoard);
        checkTarget(generatedBoard);
    } else {
        freeBoard(generatedBoard);
    }
}

