//
// sdlTest.c
// Created by bc14627 on 2014/03/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/sdlTest.h"
#include "neillsdl2.h"

/*---------- Symbolic Constants  -----------*/

#define RECTSIZE 20
#define MILLISECONDDELAY 1

/*---------- Main -----------*/
void gravity(double *modifier);
void collisionDet(double y, double x, double *dx, double *dy);
int main(){
	SDL_Rect rectangle;
	SDL_Simplewin sw;
	SDL_Rect rectangle_clear;

	rectangle.w = RECTSIZE;
   	rectangle.h = RECTSIZE;		

	rectangle_clear.w = WWIDTH;
	rectangle_clear.h = WHEIGHT;
	double x = 100;
	double y =100;
	double dy =-10;
	double dx =-10;
	

	int col1 = 100;	
	
	
	
	Neill_SDL_Init(&sw);
	do	{	
	//	gravity(&dx);
	//	gravity(&dy);
	//	x= x + dx;
		lfprint(dy);
		lfprint(x);
		y=y + dy;
		collisionDet(y,x, &dx, &dy);
		lfprint(x);	
		lfprint(dx);
		col1 = col1 + 10;
		

		SDL_Delay(MILLISECONDDELAY);
		Neill_SDL_SetDrawColour(&sw,col1,50,11);

		rectangle.x = x;
      	rectangle.y = y;
		rectangle_clear.x = 0;
		rectangle_clear.y = 0;
		SDL_RenderFillRect(sw.renderer, &rectangle);


		SDL_RenderPresent(sw.renderer);
      		SDL_UpdateWindowSurface(sw.win);	

		SDL_RenderFillRect(sw.renderer, &rectangle);

		Neill_SDL_SetDrawColour(&sw,0,0,0);
		SDL_RenderFillRect(sw.renderer,&rectangle_clear);

		Neill_SDL_Events(&sw);

	} while (!sw.finished);
	
	return 0;

}

/*---------- Functions ----------*/

void collisionDet(double y, double x, double *dx, double *dy)	{

                if (y > 570 || y < 5 ) {
                        *dy = -*dy;
						printf("collision detected \n");
						gravity(dy);
                }
                if (x > 770 || x < 5 ) {
                        *dx = -*dx;
						gravity(dx);
                }	

}

void gravity(double *modifier)	{

		if(*modifier > 0)	{
			*modifier = *modifier - 2;
		} else if(*modifier > 0)  {
			*modifier = *modifier + 2;	
		}
}
