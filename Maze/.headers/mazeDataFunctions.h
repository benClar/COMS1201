/*----------Enumerated Types-----------*/
//change to capital?
typedef enum { MISC, ENTRANCE, EXIT, WALL, EXITROUTE, DEADEND } blockType;
typedef enum {FALSE, TRUE} BOOL;
/*----------Symbolic Constants-----------*/
#define RECTSIZE	20	//rectangle size
#define LEFTSIDE 	0	//Far lefthand side of maze
#define	TOPSIDE		0
#define TOPWALL		0 	//Top wall of maze
#define INCREMROW	-1 	//add to go up a row
#define DECREMROW 	1 	//add to go down a row
/*----------Typedefs----------*/

//typedef struct mazeBlock *MazeBlock;
typedef struct mazeMap *MazeMap;
typedef struct pathList *PathList;
typedef struct pathNode *PathNode;
/*----------Function Prototypes-----------*/
MazeMap createMap(int height, int width);
void *checkMalloc(void *malP);
int addToGrid(MazeMap maze, int *row, int *col, char value, char wallCharacter);
int **createIntArray(int height, int width);
char getBlock(MazeMap maze, int row, int col); 
int getWidth(MazeMap maze);
int getHeight(MazeMap maze);
int printCorrectRoute(MazeMap maze,PathList list);
int setBlockType(MazeMap maze, int row, int col,blockType newBT);
int getBlockType(MazeMap maze, int row, int col);
int mazeBoundaryCheck(MazeMap maze, int row, int col);
int findEntrance(MazeMap maze,int *eRow, int *eCol);
int detectExit(MazeMap maze, int row, int col);
int graphicalPrintFullRoute(MazeMap maze,SDL_Simplewin sw);
PathList createList();
void addNode(PathList list, int addRow, int addCol);
void cleanList(MazeMap maze, PathList list);
int printFullRoute(MazeMap maze);
int graphicalPrintRightRoute(MazeMap maze,PathList list,SDL_Simplewin sw);
