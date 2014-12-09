#ifndef boardModule_h
#define boardModule_h

#include "graphics.h"

/*----------Enumerated Types-----------*/

typedef enum buttonState { DEAD, ALIVE } buttonState;
typedef enum mode {LINEAR, ZHASH, BHASH } mode;
/*----------Symbolic Constants-----------*/

#define HALFWAY		3	//! Halfway point of board height
#define MAXCOL      7       //!width of board
#define MAXROW      8       //!height of board
#define MINCOORD    0       //!smallest coordinate allowed

/*----------Typedefs----------*/

typedef struct boardQueueHead *BoardQueueHead;
typedef struct boardNode *BoardNode;
typedef struct button *Button;

/*----------Program Function Prototypes-----------*/
BoardNode createBoard(BoardNode ParentBoard);
void populateBoard(BoardNode newBoard, BoardNode ParentBoard);
void createQueue();
void setTargetMove(int tRow, int tCol);
void testTargets();
BoardQueueHead getQueue(BoardQueueHead newQueue);
BoardNode addToQueue(BoardNode newBoard);
void addButtonToBoard(BoardNode newBoard, int button, int row, int col);
void printQueue();
int getButtonStatus(BoardNode currentNode, int row, int col);
void setButtonDead(BoardNode board, int row, int col);
void setButtonAlive(BoardNode board, int row, int col);
void checkTarget(BoardNode currentBoard);
BoardNode getStartBoard();
int checkBounds(int row, int col);
BoardNode copyParentToChild(BoardNode Parent, BoardNode Child);
BoardNode compBoardWithList(BoardNode currBoard);
void freeBoard(BoardNode board);
int changeButton(BoardNode currBoard, int row, int col, buttonState newState);
BoardNode getFinalBoard();
BoardNode nextInList(BoardNode currentBoard);
void verifyNumQueueItems();
int getNumBoards();
void printBoard(char *string,BoardNode btp);
int recursiveSuccess(BoardNode currBoard);
void printSuccessSeries();
void graphicalPrintBoard(BoardNode currBoard);
int compareTwoBoards(BoardNode boardOne, BoardNode boardTwo);
void setMode(mode programMode);
void setNextBoard(BoardNode current, BoardNode next);
mode getMode();
void setBitID(BoardNode board, uint64_t bitID);
uint64_t getBitID(BoardNode board);
void freeQueue();
void freeBoardArray(BoardNode board);
void createBoardArray(BoardNode newBoard);
BoardNode getParent(BoardNode CurrBoard);
/*----------Test Function Prototypes-----------*/
int testStartOfQueue();
void *checkMalloc(void *malP);
int testCopyParentToChild(BoardNode Parent, BoardNode Child);

#endif
