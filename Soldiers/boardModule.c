//
// boardModule.c
// Created by bclarke on 2014/29/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "boardModule.h"

/*---------- Structures -----------*/

/*
 * pointing to the board queue. 
 */
struct boardQueueHead	{
	
	int nItems;		//!Number of board in queue
	int targetRow;	//!Target row of final destination
	int targetCol;	//!Target column of final Destination
	BoardNode finalBoard; //!Board that fulfills criteria
	BoardNode start;	//!first Board in queue	


};

/*
 * Hold boards and related information
 */
struct boardNode	{

	BoardNode parentBoard; //!Link to parent board
	//Button **board;
	buttonState **board;
	BoardNode next; 	//!Next board in queue.  Deafults to NULL.

};

/*
 *Buttons that fill board
 */
struct button	{

	buttonState currState;		

};

/*---------- Functions ----------*/

/*
 *Returns memory address of final board or null if final board has not yet been found
 */
BoardNode getFinalBoard()	{

	return getQueue(NULL)->finalBoard;
}

/*
 * Returns true if button exists in given grid element
 */
int getButtonStatus(BoardNode currentNode, int row, int col)	{


	if(currentNode->board[row][col] == ALIVE)	{
		return 1;
	}

		return 0;
}

/*
 * Prints Successful Final Series
 */
void printSuccessSeries()	{
	int i = 0;
	printf("%i\n",i);
	BoardNode sBoard;
	printf("Success\n");
	for(sBoard = getQueue(NULL)->finalBoard;sBoard->parentBoard != NULL; sBoard = sBoard->parentBoard)	{
		printBoard("final Solution",sBoard);
		
	}
}

void graphicalPrintBoard(BoardNode sBoard)	{

	int row, col, xOffset, yOffset;
	for(row = 0, yOffset = 0; row < MAXROW; row++, yOffset+=1)	{
		for(col = 0, xOffset = 0; col < MAXCOL; col++, xOffset+=1)	{
			if(sBoard->board[row][col] == ALIVE)	{
				drawAliveButton(col+xOffset,row+yOffset);
				iprint(col+xOffset);
				iprint(row+yOffset);
			} else if(sBoard->board[row][col] == DEAD)	{
				drawDeadButton(col+xOffset,row+yOffset);
				iprint(col+xOffset);
				iprint(row+yOffset);
			}
		}
	}
		pauseForInput();

}


int recursiveSuccess(BoardNode currBoard)	{
	if(getFinalBoard() == NULL)	{return 0;}
	if(currBoard->parentBoard == NULL){ 
		//printBoard("Success",currBoard); 
		graphicalPrintBoard(currBoard);
		return 1;
	}

	if(recursiveSuccess(currBoard->parentBoard))	{
			//printBoard("success",currBoard);
		graphicalPrintBoard(currBoard);
		return 1;
	}	
	
	return 0;
}

/*
 * Returns first board in queue
 */
BoardNode getStartBoard()	{

	return getQueue(NULL)->start;

}

/*
 * returns true if target destination has been filled with button
 */
void checkTarget(BoardNode currentBoard)	{
	if(currentBoard->board[getQueue(NULL)->targetRow][getQueue(NULL)->targetCol] == ALIVE)	{
		getQueue(NULL)->finalBoard = currentBoard;
	}
}

BoardNode nextInList(BoardNode currentBoard)	{

	return currentBoard->next;	

}
/*
 * testing start of queue is default board
 */
int testStartOfQueue()	{

	BoardNode currNode = getQueue(NULL)->start;	
	int row, col;
	if(currNode != NULL)	{
		for(row = 0; row < MAXROW; row++)	{
			for(col = 0; col < MAXCOL; col++)	{
				if(row <= HALFWAY)	{
					if(getButtonStatus(currNode,row,col) != DEAD)	{
						fprintf(stderr,"%d,%d should be dead\n",row,col);
						return 0;
					}
				} else {
					if(getButtonStatus(currNode,row,col) != ALIVE)	{
						fprintf(stderr,"%d,%d should be alive\n",row,col);
						return 0;
					}
				}
			}
		}
	} else {
		fprintf(stderr,"queue empty\n");
		exit(1);
	}
	return 1;
}

/*
 * creates empty board structure
 */
BoardNode createBoard(BoardNode ParentBoard)	{

	BoardNode newBoard = (BoardNode) checkMalloc(malloc(sizeof(*newBoard)));
	populateBoard(newBoard,ParentBoard);
	return newBoard;

}
void freeBoard(BoardNode board)	{

	free(board);

}

int changeButton(BoardNode currBoard, int row, int col, buttonState newState)	{

	if(currBoard->board[row][col]!= newState)	{
		currBoard->board[row][col] = newState;
	} else {
		fprintf(stderr,"Invalid move has been attempted\n");
		return 0;
	}

	return 1;

}
BoardNode copyParentToChild(BoardNode Parent, BoardNode Child)	{

	int row,col;
	for(row = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			Child->board[row][col] = Parent->board[row][col];
		}
	}
	return Child;
}

void printBoard(char *string,BoardNode btp)	{
	int row, col;
	printf("%s\n",string);
	for(row = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			printf("%d ",btp->board[row][col]);
		}
		pNL();
	}

}

BoardNode compBoardWithList(BoardNode currBoard)	{
 	BoardNode checkNode = getQueue(NULL)->start;
	int row, col, matching;
	if(checkNode == NULL)	{
		fprintf(stderr,"queue is empty\n");
		exit(1);
	}
	
	while(checkNode == getQueue(NULL)->start || checkNode->next != NULL)	{
		for(row = 0, matching = 0; row < MAXROW; row++)	{
			for(col = 0; col < MAXCOL; col++)	{
				if(checkNode->board[row][col] == currBoard->board[row][col])	{
					matching++;
				}
			}
			if(matching == (MAXCOL*MAXROW))	{
				return NULL;
			}
		}

		if (checkNode == getQueue(NULL)->start && checkNode->next == NULL ) { return currBoard;} 
		checkNode = checkNode->next; 
	}

	return currBoard;

}
/*
 *Ensures parent and child are exact copies and child is linked to parent
 */
int testCopyParentToChild(BoardNode Parent, BoardNode Child)	{

	int	row, col;
	for(row = 0; row < MAXROW; row++)	{
		for (col = 0; col < MAXCOL; col++)	{
			if(Parent->board[row][col] != Child->board[row][col])	{
				fprintf(stderr,"Copy from parent to child is not exact replica: %d,%d differs\n",row,col);
				return 0;
			}
		}
	}

	if(Child->parentBoard != Parent)	{
		fprintf(stderr,"Child board is not linked to parent board\n");
		return 0;
	}
	free(Child);
	return 1;
}

/*
 * Adds new board to end of queue
 */
BoardNode addToQueue(BoardNode newBoard)	{

	BoardNode checkQueue = getQueue(NULL)->start;
	if((getQueue(NULL)->nItems % 1000) == 0)	{
		printf("Number of items in Queue: %d \n",getQueue(NULL)->nItems);
	}
	if(checkQueue == NULL)	{
		//! If there are no boards.
		getQueue(NULL)->start = newBoard;
	} else {
		while(checkQueue->next != NULL)	{
			checkQueue = checkQueue->next;
		}
		//!Add new board to the end of the queue
		checkQueue->next = newBoard;
	}
	getQueue(NULL)->nItems++;
	return newBoard;
}

int getNumBoards()	{

	verifyNumQueueItems();
	return getQueue(NULL)->nItems;
}	

void verifyNumQueueItems()	{

	BoardNode currBoard;
	int boardCount;	
	for(boardCount = 0, currBoard = getStartBoard(); currBoard != NULL; currBoard = currBoard->next, boardCount++)	{
		
	}
	if(boardCount != getQueue(NULL)->nItems)	{
		fprintf(stderr,"We have lost count of the number of boards. Program thinks there is %d but there is actually %d",getQueue(NULL)->nItems,boardCount);
		exit(1);
	}
}

/*
 * populate board node's members
 */
void populateBoard(BoardNode newBoard, BoardNode ParentBoard)	{
	int row,col;
	newBoard->parentBoard = ParentBoard;

	newBoard->board = (buttonState**) checkMalloc(malloc(MAXROW * sizeof(buttonState*)));
	
	for(row = 0; row < MAXROW; row++)	{
		newBoard->board[row] = (buttonState*) checkMalloc(malloc(MAXCOL*sizeof(buttonState)));
		for(col = 0; col < MAXCOL; col++)	{
			newBoard->board[row][col] = DEAD;
		}
	}
	newBoard->next = NULL;
}

/*
 * Creates new queue
 */
void createQueue()	{

	BoardQueueHead newQueue = (BoardQueueHead) checkMalloc(malloc(sizeof(*newQueue)));
	newQueue->targetRow = '\0';
	newQueue->targetCol = '\0';
	newQueue->start = NULL;
	newQueue->start = NULL;
	newQueue->nItems = 0;
	newQueue->finalBoard = NULL;
	getQueue(newQueue);
}

/*
 * Sets the target row and column 
 */
void setTargetMove(int tRow, int tCol)	{

	getQueue(NULL)->targetRow = tRow;
	getQueue(NULL)->targetCol = tCol;
	testTargets();
}

/*
 * Testing target row and col are valid
 */
void testTargets()	{

	if(getQueue(NULL)->targetRow > MAXROW || getQueue(NULL)->targetRow < MINCOORD)	{
		fprintf(stderr,"target row is out of bounds");
		exit(1);
	} else if(getQueue(NULL)->targetCol > MAXCOL || getQueue(NULL)->targetCol < MINCOORD)	{
		fprintf(stderr,"target col is out of bounds");
		exit(1);
	}
}

/*
 *Stores and returns current queue address
 */
BoardQueueHead getQueue(BoardQueueHead newQueue)	{

	static BoardQueueHead currQueue;

	if(newQueue != NULL)	{
		currQueue = newQueue;
	}

	return currQueue;
}

void *checkMalloc(void *malP)   {

    if (malP == NULL)   {
        fprintf(stderr, "null address generated\n");
        exit(1);
    }
    return malP;
}

void addButtonToBoard(BoardNode newBoard, int button, int row, int col)	{
	if(button == 1)	{
		setButtonAlive(newBoard,row,col);
	} else if(button == 0)	{
		setButtonDead(newBoard,row,col);
	}
}

/*
 * Set button state to Alive
 */
void setButtonAlive(BoardNode board, int row, int col)	{
	board->board[row][col] = ALIVE;
}

/*
 * Set button state to Dead
 */
void setButtonDead(BoardNode board, int row, int col)	{
	board->board[row][col] = DEAD;
}

/*
 * Boundary checks pair of grid coordinates
 */
int checkBounds(int row, int col)	{

	if((row < MAXROW && row >= 0) && (col < MAXCOL && col >= 0))	{
		return 1;
	}

	return 0;
}


/*
 * Print Entire queue
 */
void printQueue()	{
	
	BoardNode boardToPrint = getQueue(NULL)->start;
	int row, col, queueN = 0;
	while(boardToPrint != NULL)	{
		queueN++;
		printf("Queue Item %d\n",queueN);
		for(row = 0; row < MAXROW; row++)	{
			for(col = 0; col < MAXCOL; col++)	{
				printf("%d ",boardToPrint->board[row][col]);
			}
		pNL();
		}
		boardToPrint = boardToPrint->next;
	}
}
