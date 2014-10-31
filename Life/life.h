/*----------Enumerated Types-----------*/


typedef enum { ALIVE, DEAD } state;
typedef enum { DEBUG, GRAPHIC } mode;


/*---------- Symbolic Constants  -----------*/

#define	MAXSIZE		50	//max grid size
#define LIFESTATE	2	//Number of states
#define AREASIZE 	3	//Three rows or columns need checked
#define LOWERBOUND	0	//Lower boundary of grid
#define OPTIMALPOP	3 	//Over this is overpopulation
#define UNDERPOP	2	//under this is underpopulation
#define DELAY		150 	//delay between frames
/*----------Structures----------*/

typedef struct	{

	state s1;	//Lifeform declared as structure for future flexibility

} Lifeform;

/*----------Function Prototypes-----------*/
mode setMode(int argc, char *mode);
void getSize(int *size, int *cycles);
void times(Lifeform **gridOne, Lifeform **gridTwo, int size, int n, mode m1);
Lifeform **makeGrid(int size);
void nextState(Lifeform **gridOne, Lifeform **gridTwo, int size, mode m1);
int checkNeighbour(int curRow, int curCol, Lifeform **grid, int size);
void calcNewState(int liveCnt, Lifeform *current, Lifeform *next);
void printGrid(Lifeform **grid, int size);
void initGrid(Lifeform **grid, int size);
void setLife(Lifeform *life, state setState, int row, int col);
state genState();
int boundaryCheck(int coord, int size);
void delay(int milliseconds);
void scanInt(int *toScan);
int strnCompare(char stringOne[], char stringTwo[]);
void *checkMal(void *point);
