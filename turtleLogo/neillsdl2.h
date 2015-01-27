#ifndef neillsdl2_h
#define neillsdl2_h

#include <math.h>
#include <SDL.h>

#define WWIDTH 1200 
#define WHEIGHT 800

#define FULLSCREEN_YN   0
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   700
#define HALF    2
#define DEGREES 360

#define SDL_8BITCOLOUR 256

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

// All info required for windows / renderer & event loop
/*struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
};*/

typedef struct SDL_simplewin *SDL_Simplewin;

void Neill_SDL_SetDrawColour(Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_Init();
void Neill_SDL_Events();
void Neill_SDL_SetDrawColour(Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);
SDL_Simplewin getSw(SDL_Simplewin sw);
void drawLine(int xSource, int ySource, int xDest, int yDest);
int checkFinished();
void endFrame();
#endif
