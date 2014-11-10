/*----------Enumerated Types-----------*/



/*----------Symbolic Constants-----------*/



/*----------Typedefs----------*/

typedef struct mazeBlock *MazeBlock;
typedef struct mazeMap *MazeMap;

/*----------Function Prototypes-----------*/
MazeMap createMap(int height, int width);
void *checkMalloc(void *malP);
int addToGrid(MazeMap maze, int row, int col, char value);
