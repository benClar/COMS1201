/*----------Enumerated Types-----------*/
typedef enum { MISC, ENTRANCE, EXIT, WALL, EXITROUTE, DEADEND, ROW, COL } blockType;
typedef enum {FALSE, TRUE} BOOL;
/*----------Symbolic Constants-----------*/
#define RECTSIZE	20	//rectangle size
/*----------Typedefs----------*/

typedef struct mazeMap *MazeMap;
/*----------Function Prototypes-----------*/
MazeMap createMap(int height, int width);
void *checkMalloc(void *malP);
int addToGrid(MazeMap maze, int row, int col, char value, char wallCharacter);
int **createIntArray(int height, int width);
char getBlock(MazeMap maze, int row, int col); 
int getWidth(MazeMap maze);
int getHeight(MazeMap maze);
int setBlockType(MazeMap maze, int row, int col,blockType newBT);
int getBlockType(MazeMap maze, int row, int col);
int mazeBoundaryCheck(MazeMap maze, int row, int col);	
void freeMazeBlocks(MazeMap maze);
