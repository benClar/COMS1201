//
// life.c
// Created by bclarke on 2014/17/10
// simulation of conways game of life   
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/
#include "neillsdl2.h"
#include "debug.h"
#include "life.h"

/*---------- Symbolic Constants  -----------*/
#define RECTSIZE 	3	
#define WHITE		255
#define BLACK		0
/*---------- Main -----------*/

int main(int argc, char **argv) {

	
	srand(time(NULL));
	int size, 		//size of the grid
		cycles,		//number of cycles to be done 
		argCnt = argc - 1; 
	
	mode m1 = setMode(argCnt, argv[1]);

	getSize(&size, &cycles);	//set size and cycle count from user input
	
	//Creating two grids of specified size to hold lifeforms
	Lifeform **grid1 = makeGrid(size);
	Lifeform **grid2 = makeGrid(size);
	
	//initialising both grids with lifeforms
	initGrid(grid1,size);
	initGrid(grid2,size);

	//rotate and update grids
	times(grid1,grid2,size,cycles,m1);
	
	atexit(SDL_Quit);

	free(grid1);
	free(grid2);

	return 0;

}

/*---------- Functions ----------*/

//switches off graphics in debug mode
mode setMode(int argc, char *mode)
{
	if (argc)	{
		if (!strcmp(mode,"debug")) {
			printf("***debug mode***\n");
			return DEBUG;	
		}
	}
	return GRAPHIC;
}


//get size and cycle number.
void getSize(int *size, int *cycles)	
{
	do { 
		printf("Enter Size of grid desired (max size is 50): \n");
		scanInt(size);
	}	while (!(*size <= MAXSIZE) && (*size >= 0)); 
	printf("Enter Cycles desired:	\n");
	scanInt(cycles);

}

//validated scanf function for ints
void scanInt(int *toScan)	{

	while(!scanf("%d", toScan))       {
	       clearInputBuf
               printf("Please enter int\n");
	}	
}

//controls grids 
void times(Lifeform **gridOne, Lifeform **gridTwo, int size, int n, mode m1)
{
		SDL_Simplewin sw;
       	Neill_SDL_Init(&sw);
		int i, cycle=1;
		for (i = 0; i < n && (!sw.finished); i++)	{
			printf("cycle:	%d \n",i);
			if (cycle)	{
				if (m1)	{ //graphic mode
					printGrid(gridOne,size, sw);
				}
				nextState(gridOne,gridTwo,size,m1);
				cycle--;
			} else	{
				if (m1)	{
					printGrid(gridTwo,size,sw);
				}
				nextState(gridTwo,gridOne,size,m1);	
				cycle++;
			}


			SDL_RenderPresent(sw.renderer);
			SDL_UpdateWindowSurface(sw.win);

			Neill_SDL_Events(&sw);
			delay(DELAY);	//delay between cycle prints
		}
}

//creating delay.  Adapted from http://c-for-dummies.com/blog/?p=69
void delay(int milliseconds)
{
    double interval;
    clock_t a,b;

    interval = milliseconds*(CLOCKS_PER_SEC/1000);
    a = b = clock();
    while( (a-b) < interval )	{
    	a = clock();
    }
}


//creating grid
Lifeform **makeGrid(int size)    {
	
	int	i;
    	Lifeform **array;
    	array = (Lifeform**) checkMal(malloc(size*sizeof(Lifeform*)));
	for (i = LOWERBOUND; i < size; i++)	{
		array[i] = (Lifeform*) checkMal(malloc(size * (sizeof(Lifeform))));
	}
	
	return array;	
}

//validating malloc
void *checkMal(void *point)	{

	if (point == NULL)	{
		fprintf(stderr, "null address generated");
		exit(1);
	}	

	return point;

}

//generated next state of grid
void nextState(Lifeform **gridOne, Lifeform **gridTwo, int size, mode m1)	{

    int row, col, liveCnt;
    for(row = LOWERBOUND; row < size; row++) {
        for (col = LOWERBOUND; col < size; col++)    {
			liveCnt= checkNeighbour(row,col,gridOne, size);
			if (!m1)	{
				printf("Coordinates: %d, %d, status: %d, Neighbour count: %d\n",row,col, gridOne[row][col].s1,liveCnt);
			}
			calcNewState(liveCnt,&gridOne[row][col], &gridTwo[row][col]);
        }
    }
}

//Checks surrounding elements for life
int checkNeighbour(int curRow, int curCol, Lifeform **grid, int size)	{
	
	int rowCnt, colCnt, liveCnt=0,row, col;
    for(rowCnt=LOWERBOUND, row=curRow-1; rowCnt < AREASIZE; rowCnt++, row++) {
        for (colCnt=LOWERBOUND, col=curCol -1; colCnt < AREASIZE; colCnt++, col++)	{	
			if (row != curRow || col != curCol)	{
				if (boundaryCheck(row, size) && boundaryCheck(col, size))	{	
					if (grid[row][col].s1==ALIVE)	{
						liveCnt++;
					}
				} 
			} 
		}
	}
	return liveCnt;
}

//kills or births life depending on neighbour count
void calcNewState(int liveCnt, Lifeform *current, Lifeform *next)	{
	switch (current->s1)	{

	case ALIVE:
		if (liveCnt < UNDERPOP || liveCnt > OPTIMALPOP)	{
			next->s1= DEAD;
		} else {
			next->s1= ALIVE;
		}
		break;
	case DEAD:
		if (liveCnt == OPTIMALPOP)	{
			next->s1=ALIVE;
		} else {
			next->s1=DEAD;
		}
		break;
	default:
		break;
	}
}

//prints graphical representation of 2d array holding lifeforms
void printGrid(Lifeform **grid, int size, SDL_Simplewin sw)	{

	int	row, col;

	SDL_Rect rectangle;
	rectangle.w = RECTSIZE;
   	rectangle.h = RECTSIZE;
    SDL_Rect rectangle_clear;

    rectangle_clear.w = WWIDTH;
    rectangle_clear.h = WHEIGHT;
    rectangle_clear.x  = 0;
    rectangle_clear.y  = 0;
	Neill_SDL_SetDrawColour(&sw,0,0,0);
	SDL_RenderFillRect(sw.renderer,&rectangle_clear);
	for(row = LOWERBOUND; row < size; row++)	{
		for (col = LOWERBOUND; col < size; col++)	{
			if (grid[row][col].s1 == ALIVE)	{
				Neill_SDL_SetDrawColour(&sw,WHITE,WHITE,WHITE);
				rectangle.x = (WWIDTH-(col*RECTSIZE));
      			rectangle.y = (WHEIGHT-(row*RECTSIZE));
				SDL_RenderFillRect(sw.renderer, &rectangle);	
			} else {
				Neill_SDL_SetDrawColour(&sw,BLACK,BLACK,BLACK);
                rectangle.x = (WWIDTH-(col*RECTSIZE));
                rectangle.y = (WHEIGHT-(row*RECTSIZE));
				SDL_RenderFillRect(sw.renderer, &rectangle);
			}
		}
		pNL();
	}
	       	//Neill_SDL_SetDrawColour(&sw,0,0,0);
            //SDL_RenderFillRect(sw.renderer,&rectangle_clear);
}

//initialising new lifeform on the grid
void initGrid(Lifeform **grid, int size)	{

	int	row, col;

	for(row = LOWERBOUND; row < size; row ++)	{
		for(col = LOWERBOUND; col < size; col++)	{
			setLife(&grid[row][col],genState(), row, col);
		}
	}
}

//setting a lifeforms properties
void setLife(Lifeform *life, state setState, int row, int col)	{

	life->s1 = setState;
}


//random generation of state
state genState()	{

	if(rand()%LIFESTATE)	{
		return ALIVE;
	}

	return DEAD;
}

//check coordinate is inside array
int boundaryCheck(int coord, int size)	{

	if (coord >= LOWERBOUND && coord < size )	{
		return 1;

	}
	return 0;

}
