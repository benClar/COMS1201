/*----------Enumerated Types-----------*/



/*----------Symbolic Constants-----------*/


/*----------Typedefs----------*/



/*----------Function Prototypes-----------*/
MazeMap readMaze(char fileLocation[]);
int toInt(char *string);
int gettingDimensions(char **dimensions, char letter,int charCount, int *rowLngth, int *colLngth);
int getFirstLine(FILE *fp,int *row, int *col);
