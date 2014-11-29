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

#include ".headers/debug.h"
#include ".headers/boardModule.h"

/*---------- Structures -----------*/

/*
 * pointing to the board queue. 
 */
struct boardQueueHead	{

	int currID; 	//! Current highest board ID
	int targetRow;	//!Target row of final destination
	int targetCol;	//!Target column of final Destination
	BoardNode start;	//!first Board in queue	


};

/*
 * Hold boards and related information
 */
struct boardNode	{

	int parentID;	//!ID of parent board node
	int boardID;	//!ID of this board node
	Button **board;
	BoardNode next; 	//!Next board in queue.  Deafults to NULL.

};

/*
 *Buttons that fill board
 */
struct button	{

	buttonState currState;		

};

/*---------- Functions ----------*/

void addBoard(int PID, Button **boardtoCopy)	{
	BoardNode checkQueue = getQueue(NULL)->start;
	BoardNode newBoard = (BoardNode) checkMalloc(malloc(sizeof(*newBoard)));
	populateBoard(newBoard,PID,boardtoCopy);

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
}

/*
 * populate board node's members
 */
void populateBoard(BoardNode newBoard, int PID, Button **boardToCopy )	{
	int row,col;
	newBoard->parentID = PID;
	newBoard->boardID = incrementID();
	//! creating space for pointer to each row
	newBoard->board = (Button**) checkMalloc(malloc(MAXROW * sizeof(*(newBoard->board))));

	//!proving this malloc is correct:
	if((MAXROW * sizeof(*(newBoard->board))) != (MAXROW * sizeof(struct button**)))	{
		fprintf(stderr,"Not mallocing enough space for 2d array of buttons\n");
		exit(1);
	}
	for(row = 0; row < MAXROW; row++)	{
		//! creating space for each row, which will point to structure
		newBoard->board[row] = (Button*)	checkMalloc(malloc(MAXCOL * sizeof(*(newBoard->board[row]))));

		//!proving this malloc is correct:
		if((MAXCOL * sizeof(*(newBoard->board[row]))) != (MAXCOL * sizeof(struct button**)))	{
			fprintf(stderr,"Not mallocing enough space for row of buttons\n");
			exit(1);
		}
		for(col = 0; col < MAXCOL; col++)	{
			//! pointer to structures in each element dereferenced.  Creating space for structure.
			newBoard->board[row][col] = (Button) checkMalloc(malloc(sizeof(*(newBoard->board[row][col]))));

			//!proving this malloc is correct:
			if(sizeof(*(newBoard->board[row][col])) != sizeof(struct button))	{
				fprintf(stderr,"Not mallocing enough space for button structure \n");
				exit(1);
			}
		}
	}
	newBoard->next = NULL;
}

int incrementID()	{

	return ++getQueue(NULL)->currID;

}


/*
 * Creates new queue
 */
void createQueue()	{

	BoardQueueHead newQueue = (BoardQueueHead) checkMalloc(malloc(sizeof(*newQueue)));
	newQueue->currID = 0;
	newQueue->targetRow = '\0';
	newQueue->targetCol = '\0';
	newQueue->start = NULL;
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

