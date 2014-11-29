/*----------Enumerated Types-----------*/
//change to capital?
typedef enum { MISC, ENTRANCE, EXIT, WALL, EXITROUTE, DEADEND } blockType;
typedef enum {FALSE, TRUE} BOOL;
/*----------Symbolic Constants-----------*/
#define RECTSIZE	20	//rectangle size
//#define LEFTSIDE 	0	//Far lefthand side of maze
//#define	TOPSIDE		0
//#define TOPWALL		0 	//Top wall of maze
/*----------Typedefs----------*/

//typedef struct mazeBlock *MazeBlock;
typedef struct mazeMap *MazeMap;
/*----------Function Prototypes-----------*/
MazeMap createMap(int height, int width);
void *checkMalloc(void *malP);
//int addToGrid(MazeMap maze, int *row, int *col, char value, char wallCharacter);
int addToGrid(MazeMap maze, int row, int col, char value, char wallCharacter);
int **createIntArray(int height, int width);
char getBlock(MazeMap maze, int row, int col); 
int getWidth(MazeMap maze);
int getHeight(MazeMap maze);
int setBlockType(MazeMap maze, int row, int col,blockType newBT);
int getBlockType(MazeMap maze, int row, int col);
int mazeBoundaryCheck(MazeMap maze, int row, int col);
