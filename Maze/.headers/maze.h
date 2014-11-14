/*----------Enumerated Types-----------*/



/*----------Symbolic Constants-----------*/


/*----------Typedefs----------*/

typedef enum { UP = -1, DOWN = 1, RIGHT = 1, LEFT = -1} Directions; 
typedef enum { DECISIONS = 1, CORRECT = 2, STATIC = 3} RouteMode;
/*----------Function Prototypes-----------*/
MazeMap readMaze(char fileLocation[]);
int toInt(char *string);
int gettingDimensions(char **dimensions, char letter,int charCount, int *rowLngth, int *colLngth);
int getFirstLine(FILE *fp,int *row, int *col);
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw,PathList exitCoords,RouteMode rMode);
int **getCorrectPath(MazeMap Maze);
void scanInt(int *toScan);
int printCorrect(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode,PathList exitCoords);
int printFull(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode);
