/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define EXITCHNCE	2	
#define NOEXITS		3
#define EXITDEC		3	
#define CHAMBERSIZE	5
/*----------Structures----------*/

/*----------Function Prototypes-----------*/
void wallMaze(MazeMap RndmMaze);
int partionMaze(MazeMap RndmMaze, int leftBound, int width, int stRow, int lowerBound);
