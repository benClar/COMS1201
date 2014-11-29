/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define EXITCHNCE	2	//chance t	
#define NOEXITS		3	//number of exits to be created in maze
#define EXITDEC		3	//
#define CHAMBERSIZE	5	//minimum width/ height of a chamber 
/*----------Structures----------*/

/*----------Function Prototypes-----------*/
void wallMaze(MazeMap RndmMaze);
int partionMaze(MazeMap RndmMaze, int leftBound, int width, int stRow, int lowerBound);
int boundCheck(int bound, int val);
void setEnt(MazeMap maze, int start, int end, int axis, blockType block);
