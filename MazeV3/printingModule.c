//! printingModule.c
/*!
 *  Prints output to screen depending on selected mode
 */
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/neillsdl2.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"
#include ".headers/printingModule.h"

/*---------- Functions ----------*/

/*
 * Prints full correct route coordinates stored in route list
 */
int printCorrectRoute(MazeMap maze,PathList list)   {
    int r,c,count;
	setCurr(list,getStart(list));
        while(getCurr(list) != NULL){
            for(r = 0,count = 0; r < getHeight(maze); r++)    {
                for(c = 0; c < getWidth(maze); c++)     {
                    if((getCurr(list) != NULL ) && (r == getCurrRow(list)) && (c == getCurrCol(list)) && (!count))  {
                        printf(" .");
						nextNode(list);
                        count++;
                    } else{
                        printf(" %c",getBlock(maze,r,c));
                    }
                }
                pNL();
            }
				delay(DELAY);
        }
    return 1;
}

/*
 * Console output of known correct route at point when called
 */
int printFullRoute(MazeMap maze)    {
    int r,c;
    for(r = 0; r < getHeight(maze); r++)    {
        for(c = 0; c < getWidth(maze); c++) {
            if(getBlockType(maze,r,c)==EXITROUTE)   {
                printf(" .");
            } else{
                printf(" %c",getBlock(maze,r,c));
            }
        }
        pNL();
    }
    return 1;
}

/*
 *Graphical Output of path coorindates stored in route list
 */
int graphicalPrintRightRoute(MazeMap maze,PathList list,SDL_Simplewin sw)   {

    int r,c,count;
    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;
	setCurr(list,getStart(list));
    while(getCurr(list) != NULL && (!sw.finished)){
        for(r=0, count = 0; r < getHeight(maze); r++)   {
            for(c = 0; c < getWidth(maze); c++) {
                if((getCurr(list) != NULL ) && (r == getCurrRow(list)) && (c == getCurrCol(list)) && (!count))      {
                    Neill_SDL_SetDrawColour(&sw,255,165,0); //!yellow
                    rectangle.x = (c*RECTSIZE);
                    rectangle.y = (r*RECTSIZE);
                    SDL_RenderFillRect(sw.renderer, &rectangle);
					nextNode(list);
                                    count++;
                } else if(getBlockType(maze,r,c)==WALL) {
                    Neill_SDL_SetDrawColour(&sw,128,0,0); //!red 
                    rectangle.x = (c*RECTSIZE);
                    rectangle.y = (r*RECTSIZE);
                    SDL_RenderFillRect(sw.renderer, &rectangle);
                } else {
                    Neill_SDL_SetDrawColour(&sw,0,0,0); //!black
                    rectangle.x = (c*RECTSIZE);
                    rectangle.y = (r*RECTSIZE);
                    SDL_RenderFillRect(sw.renderer, &rectangle);
                }
            }
        }
        SDL_RenderPresent(sw.renderer);
        SDL_UpdateWindowSurface(sw.win);
        Neill_SDL_Events(&sw);
		delay(DELAY);
    }
    return 1;
}

/*
 * graphical print of known current route when called
 */
int graphicalPrintFullRoute(MazeMap maze,SDL_Simplewin sw)  {
    int r,c;

    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;

    for(r = 0; r < getHeight(maze); r++)    {
        for(c = 0; c < getWidth(maze); c++) {
            if(getBlockType(maze,r,c)==EXITROUTE)   {
                Neill_SDL_SetDrawColour(&sw,255,165,0);
                rectangle.x = (c*RECTSIZE);
                rectangle.y = (r*RECTSIZE);
                SDL_RenderFillRect(sw.renderer, &rectangle);
            } else if(getBlockType(maze,r,c)==WALL) {
                Neill_SDL_SetDrawColour(&sw,128,0,0);
                rectangle.x = (c*RECTSIZE);
                rectangle.y = (r*RECTSIZE);
                SDL_RenderFillRect(sw.renderer, &rectangle);
            } else if (getBlockType(maze,r,c)==ENTRANCE) {
                Neill_SDL_SetDrawColour(&sw,0,128,0);
                rectangle.x = (c*RECTSIZE);
                rectangle.y = (r*RECTSIZE);
                SDL_RenderFillRect(sw.renderer, &rectangle);
            } else {
                Neill_SDL_SetDrawColour(&sw,0,0,0);
                rectangle.x = (c*RECTSIZE);
                rectangle.y = (r*RECTSIZE);
                SDL_RenderFillRect(sw.renderer, &rectangle);
            }
        }
    }

    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    return 1;
}

//creating delay.  Adapted from http://c-for-dummies.com/blog/?p=69
void delay(int milliseconds)
{
    double interval;
    clock_t a,b;

    interval = milliseconds*(CLOCKS_PER_SEC/1000);
    a = b = clock();
    while( (a-b) < interval )   {
        a = clock();
    }
}

