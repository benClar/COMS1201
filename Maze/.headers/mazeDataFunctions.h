/*----------Enumerated Types-----------*/
//change to capital?
typedef enum { MISC, ENTRANCE, EXIT, WALL, EXITROUTE, DEADEND } blockType;
typedef enum {FALSE, TRUE} BOOL;
/*----------Symbolic Constants-----------*/

#define LEFTSIDE 	0	//Far lefthand side of maze
#define	TOPSIDE		0
#define TOPWALL		0 	//Top wall of maze
#define INCREMROW	-1 	//add to go up a row
#define DECREMROW 	1 	//add to go down a row
/*----------Typedefs----------*/

//typedef struct mazeBlock *MazeBlock;
typedef struct mazeMap *MazeMap;

/*----------Function Prototypes-----------*/
MazeMap createMap(int height, int width);
void *checkMalloc(void *malP);
int addToGrid(MazeMap maze, int *row, int *col, char value, char wallCharacter);
char **createStringArray(int height, int width);
char getBlock(MazeMap maze, int row, int col); 
int getWidth(MazeMap maze);
int getHeight(MazeMap maze);
int printMap(MazeMap maze);
int setBlockType(MazeMap maze, int row, int col,blockType newBT);
int getBlockType(MazeMap maze, int row, int col);
int mazeBoundaryCheck(MazeMap maze, int row, int col);
int findEntrance(MazeMap maze);
int detectExit(MazeMap maze, int row, int col);
