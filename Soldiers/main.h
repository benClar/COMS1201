/*----------Enumerated Types-----------*/

typedef enum moveStage {JUMP, DELETE, DONE} moveStage;

/*----------Symbolic Constants-----------*/

#define EMPTY		0		//!button not present in space from default file
#define	FULL		1		//!button present in space from default file
#define TOINT		'0'		//!subtract to convert char number to integer
#define TRUE		1		//!For testing.  Expecting true value.
#define FALSE		0		//!For testing.  Expecting False value.
#define MVRIGHT		1		//!Add to column to move right
#define MVLEFT		-1		//!Add to columns to move left
#define MVUP		-1		//!Add to row to move up
#define MVDOWN		1		//!add to row to move down
#define DONTMV		0		//!Add to row or columns to keep constant
#define NEIGHDIS	1		//!Check Neighbour in adjacent cell
#define MAXDISTOCHECK	2	//!Positions distance of 2 away from current position need to be checked to move
#define ARGCNT		2		//! Number of arguments that should be passed into program
/*----------Typedefs----------*/



/*----------Function Prototypes-----------*/

int checkInt(int toCheck, int max, int min);
void readDefaultMap();
int generatePossibleMove(BoardNode currentBoard);
int validateMove(BoardNode currentBoard, int moveRow, int moveCol, int currRow, int currCol, int distance);
BoardNode makeMove(BoardNode newBoard, int moveRow, int moveCol, int currRow, int currCol, moveStage currStep);
void generateUniqueBoardWithMove(BoardNode currentBoard, int rowMove, int colMove, int currRow, int currCol);
BoardNode getFinalBoard();
void userEnterTargetDestination(char *x, char *y);
void checkArg(int *argc);
char *checkEnteredString(char *toCheck);
/*----------Testing Function Prototypes-----------*/
void testing();
void testVal(int testResult, int expected, char *description);
