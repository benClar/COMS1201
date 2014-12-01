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

	int targetRow;	//!Target row of final destination
	int targetCol;	//!Target column of final Destination
	BoardNode start;	//!first Board in queue	


};

/*
 * Hold boards and related information
 */
struct boardNode	{

	BoardNode parentBoard; //!Link to parent board
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

/*
 * Returns true if button exists in given grid element
 */
int getButtonStatus(BoardNode currentNode, int row, int col)	{


	if(currentNode->board[row][col]->currState == ALIVE)	{
		return 1;
	}

		return 0;
}


BoardNode getStartBoard()	{

	return getQueue(NULL)->start;

}

/*
 * returns true if target destination has been filled with button
 */
int checkTarget(BoardNode currentBoard)	{

	if(currentBoard->board[getQueue(NULL)->targetRow][getQueue(NULL)->targetCol]->currState == ALIVE)	{
		return 1;
	}

	return 0;
}

int testCheckTarget(int row, int col)	{
	setTargetMove(row,col);
	if(checkTarget(getStartBoard()))	{
		return 1;
	} 	

	return 1;
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

BoardNode copyParentToChild(BoardNode Parent, BoardNode Child)	{

	int row;
	for(row = 0; row < MAXROW; row++)	{
		memcpy(&Child->board[row], &Parent->board[row], MAXCOL * sizeof(*(Parent->board[row])));
	}
	return Child;
}

int testCopyParentToChild(BoardNode Parent, BoardNode Child)	{

	int	row, col;
	for(row = 0; row < MAXROW; row++)	{
		for (col = 0; col < MAXCOL; col++)	{
			if(Parent->board[row][col]->currState != Child->board[row][col]->currState)	{
				fprintf(stderr,"Copy from parent to child is not exact replica: %d,%d differs\n",row,col);
				return 0;
			}
		}
	}

	if(Child->parentBoard != Parent)	{
		fprintf(stderr,"Child board is not linked to parent board\n");
		return 0;
	}

	return 1;
}

BoardNode addToQueue(BoardNode newBoard)	{

	BoardNode checkQueue = getQueue(NULL)->start;

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
	return newBoard;
}

/*
 * populate board node's members
 */
void populateBoard(BoardNode newBoard, BoardNode ParentBoard)	{
	int row,col;
	newBoard->parentBoard = ParentBoard;

	//!creating a 2d array of pointers to structures
	//!Here I am mallocing pointer to 1d array of pointers( to each row),
	//!which will in turn hold pointers to structures  
	newBoard->board = (Button**) checkMalloc(malloc(MAXROW * sizeof(*(newBoard->board))));

	//!proving this malloc is correct:
	if((MAXROW * sizeof(*(newBoard->board))) != (MAXROW * sizeof(struct button***)))	{
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
			newBoard->board[row][col]->currState = ALIVE;
			
			//!proving this malloc is correct:
			if(sizeof(*(newBoard->board[row][col])) != sizeof(struct button))	{
				fprintf(stderr,"Not mallocing enough space for button structure \n");
				exit(1);
			}
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
	board->board[row][col]->currState = ALIVE;
}

/*
 * Set button state to Dead
 */
void setButtonDead(BoardNode board, int row, int col)	{
	board->board[row][col]->currState = DEAD;
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
				if(boardToPrint->board[row][col]->currState == ALIVE)	{
					printf("1");
				} else if (boardToPrint->board[row][col]->currState == DEAD)	{
					printf("0");
				}
			}
		pNL();
		}
		boardToPrint = boardToPrint->next;
	}
}
