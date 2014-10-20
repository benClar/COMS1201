//----------Enumerated Types-----------


typedef enum { ALIVE, DEAD } state;

/*---------- Symbolic Constants  -----------*/

#define LIFESTATE	2	//Number of states
#define AREASIZE 	3	//Three rows or columns need checked
#define LOWERBOUND	0	//Lower boundary of grid
#define OPTIMALPOP	3 	//Over this is overpopulation
#define UNDERPOP	2	//under this is underpopulation

//----------Structures----------

typedef struct	{

	state s1;

} Lifeform;

//----------Function Prototypes-----------

void times(Lifeform **gridOne, Lifeform **gridTwo, int size, int n );
Lifeform **makeGrid(int size);
void nextState(Lifeform **gridOne, Lifeform **gridTwo, int size);
int checkNeighbour(int curRow, int curCol, Lifeform **grid, int size);
void calcNewState(int liveCnt, Lifeform *current, Lifeform *next);
void printGrid(Lifeform **grid, int size);
void initGrid(Lifeform **grid, int size);
void setLife(Lifeform *life, state setState, int row, int col);
state genState();
int boundaryCheck(int coord, int size);
