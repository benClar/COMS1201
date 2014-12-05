/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "graphics.h"

/*---------- Functions ----------*/


struct display	{

	SDL_Simplewin sw;

};

void pauseForInput()	{
	Display d = getDisplay(NULL);
	while(!d->sw.finished)	{
		Neill_SDL_Events(&d->sw);
	}
	resetFinished();

}

void resetFinished()	{

		Display d = getDisplay(NULL);
		d->sw.finished = 0;

}

void createDisplay()	{

	Display newDisplay = (Display) malloc(sizeof(*newDisplay));
	getDisplay(newDisplay);

}

void start_SDL()	{
	createDisplay();
	Display d = getDisplay(NULL);
	Neill_SDL_Init(&d->sw);
	Neill_SDL_SetDrawColour(&d->sw, 0, 0, 0);
	SDL_RenderClear(d->sw.renderer);

}

Display getDisplay(Display currDisplay)	{

	static Display cDisplay;
	if(currDisplay != NULL)	{
		cDisplay = currDisplay;
	}

	return cDisplay;

}
void drawAliveButton(int x, int y)	{
	
	Display d = getDisplay(NULL);
	SDL_Rect rectangle;
	rectangle.w = RECTSIZE;
	rectangle.h = RECTSIZE;
	Neill_SDL_SetDrawColour(&d->sw,255,255,255);
	rectangle.x = x*RECTSIZE;
	rectangle.y = y*RECTSIZE;
	SDL_RenderFillRect(d->sw.renderer,&rectangle);
	Neill_SDL_SetDrawColour(&d->sw,0,0,0);
	Neill_SDL_RenderFillCircle(d->sw.renderer,(x*RECTSIZE) + RECTSIZE/2,(y*RECTSIZE) + RECTSIZE/2,RECTSIZE/2);
	SDL_RenderPresent(d->sw.renderer);
//	SDL_UpdateWindowSurface(d->sw.win);
}

void drawDeadButton(int x, int y)	{
	Display d = getDisplay(NULL);
	SDL_Rect rectangle;
	rectangle.w = RECTSIZE;
	rectangle.h = RECTSIZE;
	Neill_SDL_SetDrawColour(&d->sw,255,255,255);
	rectangle.x = x*RECTSIZE;
	rectangle.y = y*RECTSIZE;
	SDL_RenderFillRect(d->sw.renderer,&rectangle);
	SDL_RenderPresent(d->sw.renderer);
	SDL_UpdateWindowSurface(d->sw.win);
}
