#ifndef display_h
#define display_h

#include "libraryFunctions.h"
#include <SDL.h>

/*----------Enumerated Types-----------*/

/*----------TypeDefs-----------*/

typedef struct display *Display;

/*----------Symbolic Constants-----------*/

#define FULLSCREEN_YN   0
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   700
#define HALF	2
#define DEGREES	360
/*----------Structures----------*/



/*----------Function Prototypes-----------*/
Display getDisplay(Display nDisplay);
void init_SDL();
void drawLine(int xSource, int ySource, int xDest, int yDest);
void endFrame();
void endAnimation();
void clearFrame();
void Neill_SDL_RenderFillCircle(int cx, int cy, int r);
void Neill_SDL_SetDrawColour(Uint8 r, Uint8 g, Uint8 b);

#endif
