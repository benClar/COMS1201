#ifndef moveModule_h
#define moveModule_h

#include "hashExtension.h"
#include "boardModule.h"

/*----------Enumerated Types-----------*/

typedef enum moveStage {JUMP, DELETE, DONE} moveStage;

/*----------Symbolic Constants-----------*/
#define MVRIGHT     1       //!Add to column to move right
#define MVLEFT      -1      //!Add to columns to move left
#define MVUP        -1      //!Add to row to move up
#define MVDOWN      1       //!add to row to move down
#define DONTMV      0       //!Add to row or columns to keep constant
#define MAXDISTOCHECK   2   //!Positions distance of 2 away from current position need to be checked to move
#define NEIGHDIS  1       //!Check Neighbour in adjacent cell

/*----------Structures----------*/



/*----------Function Prototypes-----------*/
BoardNode makeMove(BoardNode newBoard, int moveRow, int moveCol, int currRow, int currCol, moveStage currStep);
int validateMove(BoardNode currentBoard, int moveRow, int moveCol, int currRow, int currCol, int distance);
void generateUniqueBoardWithMove(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol);
int generatePossibleMove(BoardNode currentBoard);
void generateUniqueBoardHash(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol);
#endif
