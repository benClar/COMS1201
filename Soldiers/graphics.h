#include "neillsdl2.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define RECTSIZE 30

/*----------TypeDefs-----------*/

typedef struct display *Display;

/*----------Function Prototypes-----------*/
void pauseForInput();
void drawAliveButton(int x, int y);
void drawDeadButton(int x, int y);
void createDisplay();
Display getDisplay(Display currDisplay);
void start_SDL();
void resetFinished();
