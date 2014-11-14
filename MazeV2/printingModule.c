//
// printingModule.c
// Created by bc14627 on 2014/14/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/neillsdl2.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"
#include ".headers/printingModule.h"
/*---------- Symbolic Constants  -----------*/



/*---------- Functions ----------*/
int printCorrectRoute(MazeMap maze,PathList list)   {
    int r,c,count;
	setCurr(list,getStart(list));
    //list->current = list->start;
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
        }
    return 1;
}

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
                    Neill_SDL_SetDrawColour(&sw,255,165,0);
                    rectangle.x = (c*RECTSIZE);
                    rectangle.y = (r*RECTSIZE);
                    SDL_RenderFillRect(sw.renderer, &rectangle);
					nextNode(list);
                                    count++;
                } else if(getBlockType(maze,r,c)==WALL) {
                    Neill_SDL_SetDrawColour(&sw,128,0,0);
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
        Neill_SDL_Events(&sw);
    }
    return 1;
}

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

    //Update Window
    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    return 1;
 }
