/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/
#define DELAY 	150 	//delay between frames


/*----------Function Prototypes-----------*/
int printFullRoute(MazeMap maze);
int graphicalPrintRightRoute(MazeMap maze,PathList list,SDL_Simplewin sw);
int graphicalPrintFullRoute(MazeMap maze,SDL_Simplewin sw);
int printCorrectRoute(MazeMap maze,PathList list);
void delay(int milliseconds);
