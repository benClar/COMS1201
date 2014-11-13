/*----------Enumerated Types-----------*/



/*----------Symbolic Constants-----------*/


/*----------Typedefs----------*/

typedef enum { UP = -1, DOWN = 1, RIGHT = 1, LEFT = -1} Directions; 

/*----------Function Prototypes-----------*/
MazeMap readMaze(char fileLocation[]);
int toInt(char *string);
int gettingDimensions(char **dimensions, char letter,int charCount, int *rowLngth, int *colLngth);
int getFirstLine(FILE *fp,int *row, int *col);
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw);
int printing(MazeMap Maze, SDL_Simplewin sw, int gMode);
int **getCorrectPath(MazeMap Maze);
