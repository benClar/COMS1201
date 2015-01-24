#include <SDL2/SDL.h>
#include "display.h"
#include "debug.h"

struct display	{

	SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect    rect;
	SDL_Rect screenRect;
    SDL_Event event;	
};

Display getDisplay(Display nDisplay)	{

	static Display cDisplay;
	
	if(nDisplay!= NULL)	{
		cDisplay = nDisplay;
	}

	return cDisplay;
}

void init_SDL()	{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) ERROR("SDL_Init has failed");

	Display d = checkMalloc(malloc(sizeof(*d)));
	getDisplay(d);

	d->window = SDL_CreateWindow("Turtle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	d->screenRect.x = d->screenRect.y = 0;
	d->screenRect.w = SCREEN_WIDTH;
	d->screenRect.h = SCREEN_HEIGHT;
	
	Neill_SDL_SetDrawColour(0, 0, 0);	
	SDL_RenderClear(d->renderer);
	SDL_RenderPresent(d->renderer);
}

void Neill_SDL_SetDrawColour(Uint8 r, Uint8 g, Uint8 b)
{
	Display d = getDisplay(NULL);
	SDL_SetRenderDrawColor(d->renderer, r, g, b, SDL_ALPHA_OPAQUE);

}

void drawLine(int xSource, int ySource, int xDest, int yDest)	{
	Display d = getDisplay(NULL);
	SDL_SetRenderDrawColor(d->renderer, 0, 0, 0,0);
	SDL_RenderDrawLine(d->renderer,xSource,ySource,xDest,yDest);
}

void clearFrame()	{
	Display d = getDisplay(NULL);
	SDL_RenderClear(d->renderer);
	SDL_SetRenderDrawColor(d->renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(d->renderer, &(d->screenRect));	
}

void endFrame() {
	Display d = getDisplay(NULL);
    SDL_RenderPresent(d->renderer);
	SDL_UpdateWindowSurface(d->window);
}

void Neill_SDL_RenderFillCircle(int cx, int cy, int r)
{
	Display d = getDisplay(NULL);
	SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);	
   for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(d->renderer, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(d->renderer, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
   }

}

void endAnimation()	{

		SDL_Event event;
		if((SDL_PollEvent(&event)))	{
			switch(event.type)	{
				case SDL_KEYDOWN:
						if(event.key.keysym.sym == SDLK_ESCAPE)	{
							SDL_Quit();
							exit(0);
						}
						break;
				default:
						break;
			}
		}
}



