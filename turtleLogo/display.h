#include "libraryFunctions.h"

/*----------Enumerated Types-----------*/

/*----------TypeDefs-----------*/

typedef struct display *Display;

/*----------Symbolic Constants-----------*/

#define FULLSCREEN_YN   0
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   700

/*----------Structures----------*/



/*----------Function Prototypes-----------*/
Display getDisplay(Display nDisplay);
void init_SDL();
void drawLine(int xSource, int ySource, int xDest, int yDest);
void endFrame();
void endAnimation();
void clearFrame();
