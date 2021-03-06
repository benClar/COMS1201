//! boardModule.c: Basic
// 
//This module holds the queue of unique boards that have been generated. 
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
	BoardNode end; 	//!last board in queue
	mode runMode; 	//!Whether the program is running in linear or hash mode
};


/*
 * Hold boards and related information
 */
struct boardNode	{

	BoardNode parentBoard; //!Link to parent board
	buttonState **board; 	//!2d array containing board elements
	uint64_t bitID; 	//!64 bit ID for board: Extension
	BoardNode next; 	//!Next board in queue.  Deafults to NULL.

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
 *Frees array holding board
 */
void freeBoardArray(BoardNode board)	{

		free(board->board);
		board->board = NULL;
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

void freeQueue()	{

	BoardQueueHead q = getQueue(NULL);
	BoardNode b;
	for(b = q->start; b->next != NULL; b = b->next)	{
		free(b);
	}
	free(b); //! free last board	
}

BoardNode getParent(BoardNode CurrBoard)	{
	
	return CurrBoard->parentBoard;

}

void setNextBoard(BoardNode current, BoardNode next)	{

	current->next = next;
}

void graphicalPrintBoard(BoardNode sBoard)	{

	int row, col, xOffset, yOffset;
	for(row = 0, yOffset = 0; row < MAXROW; row++, yOffset+=1)	{
		for(col = 0, xOffset = 0; col < MAXCOL; col++, xOffset+=1)	{
			if(sBoard->board[row][col] == ALIVE)	{
				drawAliveButton(col+xOffset,row+yOffset);
			} else if(sBoard->board[row][col] == DEAD)	{
				drawDeadButton(col+xOffset,row+yOffset);
			}
		}
	}
		pauseForInput();

}


int recursiveSuccess(BoardNode currBoard)	{
	if(getFinalBoard() == NULL)	{return 0;}
	if(currBoard->parentBoard == NULL){ 
		printBoard("Success",currBoard); 
		graphicalPrintBoard(currBoard);
		return 1;
	}

	if(recursiveSuccess(currBoard->parentBoard))	{
		printBoard("success",currBoard);
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
	if(checkNode == NULL)	{
		fprintf(stderr,"queue is empty\n");
		exit(1);
	}
	
	while(checkNode == getQueue(NULL)->start || checkNode->next != NULL)	{

		if(!compareTwoBoards(checkNode,currBoard))	{
			return NULL;
		}

		if (checkNode == getQueue(NULL)->start && checkNode->next == NULL ) { return currBoard;} 
		checkNode = checkNode->next; 
	}

	return currBoard;

}

int compareTwoBoards(BoardNode boardOne, BoardNode boardTwo)	{
	int row, col, matching;

	for(row = 0, matching = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			if(boardOne->board[row][col] == boardTwo->board[row][col])	{
				matching++;
			}
		}
	}

	if(matching == (MAXCOL*MAXROW))	{
		return 0;
	}

	return 1;
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

	if((getQueue(NULL)->nItems % 100000) == 0)	{
		printf("Number of items in Queue: %d \n",getQueue(NULL)->nItems);
	}
	if(checkQueue == NULL)	{
		//! If there are no boards.
		getQueue(NULL)->end = getQueue(NULL)->start = newBoard;
	} else {
		getQueue(NULL)->end->next = newBoard;
		getQueue(NULL)->end = getQueue(NULL)->end->next;
	}
	getQueue(NULL)->nItems++;
	return newBoard;
}

int getNumBoards()	{

	verifyNumQueueItems();
	return getQueue(NULL)->nItems;
}	

/*
 *Validation module comparing the actual number of boards to the number the counter thinks there is
 */
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
	newBoard->parentBoard = ParentBoard;
	newBoard->bitID = '\0';
	createBoardArray(newBoard);
	newBoard->next = NULL;
}

void createBoardArray(BoardNode newBoard)	{

	int row, col;

    newBoard->board = (buttonState**) checkMalloc(malloc(MAXROW * sizeof(buttonState*)));

    for(row = 0; row < MAXROW; row++)   {
        newBoard->board[row] = (buttonState*) checkMalloc(malloc(MAXCOL*sizeof(buttonState)));
        for(col = 0; col < MAXCOL; col++)   {
            newBoard->board[row][col] = DEAD;
        }
    }		

}

void setBitID(BoardNode board, uint64_t bitID)	{

	board->bitID = bitID;

}

uint64_t getBitID(BoardNode board)	{

	return board->bitID;
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
	newQueue->runMode = LINEAR;
	getQueue(newQueue);
}

/*
 * changes program mode
 */
void setMode(mode programMode)	{

	getQueue(NULL)->runMode = programMode;
}

/*
 * Determine Mode of the program
 */

mode getMode()	{

	return getQueue(NULL)->runMode;

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

/*
 *Validation for Malloc
 */
void *checkMalloc(void *malP)   {

    if (malP == NULL)   {
        fprintf(stderr, "null address generated\n");
        exit(1);
    }
    return malP;
}

/*
 *Adds a button to the board depending on value
 */
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
 * Print Entire queue: Debugging
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
