/*----------Enumerated Types-----------*/



/*----------Symbolic Constants-----------*/

#define LEFTSIDE    0   //Far lefthand side of maze
#define TOPSIDE     0
#define	MINHEIGHT	0	//Minimum height of random maze
#define MAXHEIGHT 	40 	//Maximum height of random maze
#define	MINWIDTH	0	//Minimum width of random maze
#define MAXWIDTH 	40 	//Maximum height of random maze
#define FMAXWIDTH	20	// Maximum width of read maze
#define FMAXHEIGHT	20	//Maximum height of read maze
/*----------Typedefs----------*/

typedef enum { UP = -1, DOWN = 1, RIGHT = 1, LEFT = -1} Directions; 
typedef enum { DECISIONS = 1, CORRECT = 2, STATIC = 3} RouteMode;
/*----------Function Prototypes-----------*/
MazeMap readMaze(char fileLocation[]);
int toInt(char *string);
int gettingDimensions(char **dimensions, char letter,int charCount, int *rowLngth, int *colLngth);
int getFirstLine(FILE *fp,int *width, int *height);
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw,PathList exitCoords,RouteMode rMode);
int **getCorrectPath(MazeMap Maze);
void scanInt(int *toScan);
int printCorrect(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode,PathList exitCoords);
void printFull(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode);
int findEntrance(MazeMap Maze,int *eRow, int *eCol);
int detectExit(MazeMap Maze, int row, int col);
void rParam(int *randomD, int compMin, int compMax);
int checkDim(int count, int dim);
MazeMap createRandomMaze();
