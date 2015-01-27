#include "neillsdl2.h"

struct SDL_simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
};

SDL_Simplewin getSw(SDL_Simplewin sw)	{
	static SDL_Simplewin cSw;
	if(sw != NULL)	{
		cSw = sw;
	}
	return cSw;
}


// Set up a simple (WIDTH, HEIGHT) window.
// Attempt to hide the renderer etc. from user.
void Neill_SDL_Init()
{
	SDL_Simplewin sw = malloc(sizeof(*sw));
	getSw(sw);
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	} 

   sw->finished = 0;
   
   sw->win= SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   // Set screen to black
   Neill_SDL_SetDrawColour(255, 255, 255);
   SDL_RenderClear(sw->renderer);
   SDL_RenderPresent(sw->renderer);

}

void endFrame()	{
	SDL_Simplewin sw = getSw(NULL);
    SDL_RenderPresent(sw->renderer);
    SDL_UpdateWindowSurface(sw->win);	
	Neill_SDL_Events();
}

// Gobble all events & ignore most
int Neill_SDL_Events()	{
	SDL_Simplewin sw = getSw(NULL);
   	SDL_Event event;
	while(SDL_PollEvent(&event))	{
		switch (event.type){
			case SDL_QUIT:
       	  	case SDL_MOUSEBUTTONDOWN:
      		case SDL_KEYDOWN:
       	  	sw->finished = 1;
    	}
	}
	return sw->finished;
}


int checkFinished()	{
	return getSw(NULL)->finished;
}


// Trivial wrapper to avoid complexities of renderer & alpha channels
void Neill_SDL_SetDrawColour(Uint8 r, Uint8 g, Uint8 b)
{

	SDL_Simplewin sw = getSw(NULL);
	SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);

}

// Filled Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

   for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
   }

}

void drawLine(int xSource, int ySource, int xDest, int yDest)	{
	SDL_Simplewin sw = getSw(NULL);
	SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(sw->renderer,xSource,ySource,xDest,yDest);
	//SDL_Delay(20);
	endFrame();
}

// Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

   double dx, dy;
   dx = floor(sqrt((2.0 * r ) ));
   SDL_RenderDrawLine(rend, cx-dx, cy+r, cx+dx, cy+r);
   SDL_RenderDrawLine(rend, cx-dx, cy-r, cx+dx, cy-r);
   for (dy = 1; dy <= r; dy += 1.0) {
        dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawPoint(rend, cx+dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx+dx, cy-r+dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy-r+dy);
   }

}
