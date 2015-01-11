#include <SDL2/SDL_image.h>
#include "display.h"

struct display	{

	SDL_Window  *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Rect    rect;
	SDL_Rect screenRect;
    SDL_Event event;	
	SDL_Surface *screen;
};

Display getDisplay(Display nDisplay)	{

	static Display cDisplay;
	
	if(nDisplay!= NULL)	{
		cDisplay = nDisplay;
	}

	return cDisplay;
}

void init_SDL()	{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) ERROR("SDL_Init has failed");

	Display d = checkMalloc(malloc(sizeof(*d)));
	getDisplay(d);

	d->window = SDL_CreateWindow("Bash Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_YN);
	d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	d->screenRect.x = d->screenRect.y = 0;
	d->screenRect.w = SCREEN_WIDTH;
	d->screenRect.h = SCREEN_HEIGHT;
	d->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
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
	//SDL_FillRect(d->screen, &(d->screenRect),SDL_MapRGB(d->screen->format, 255, 0, 0));	
}

void endFrame() {
	Display d = getDisplay(NULL);
    SDL_RenderPresent(d->renderer);
}

void endAnimation()	{

		SDL_Event event;// = checkMalloc(calloc(1,sizeof(*event)));
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



