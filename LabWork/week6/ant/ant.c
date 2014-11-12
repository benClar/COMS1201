//
// ant.c
// Created by bclarke on 2014/07/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/*---------- Custom Headers	-----------*/
#include "neillsdl2.h"
#include ".headers/debug.h"
#include ".headers/ant.h"
/*---------- Main -----------*/

int main(int argc, char *argv[]){
	int x = (getMiddle(GRIDSIZE)-1), y =(getMiddle(GRIDSIZE)-1);
	int i;	
	Square **grid = makeGrid(GRIDSIZE);

	SDL_Simplewin sw;
	Neill_SDL_Init(&sw);

	initGrid(grid,GRIDSIZE);
	for (i = 0; i <1000000 && (!sw.finished); i++)	{
		iprint(i);
		SDL_Delay(MILLISECONDDELAY);
		printGrid(sw, grid,GRIDSIZE);
		directionTest(grid,&x,&y);
//		delay(200);
		SDL_RenderPresent(sw.renderer);
		SDL_UpdateWindowSurface(sw.win);

		Neill_SDL_Events(&sw);
	}
	return 0;

}

/*---------- Functions ----------*/

Square **makeGrid(int size) {

    int i;

    Square **array = (Square**) malloc(size*sizeof(Square*));
    for(i = 0; i < size; i++)   {
        array[i] = (Square*) malloc(size*sizeof(Square));
    }

    return array;

}

state rndmState(int range)	{

	//return (rand()%range);
	return 0;	

}

void initGrid(Square **grid, int size)	{

	int	row, col;
	
	for( row = 0; row < size; row++)	{
		for (col = 0; col < size; col++)	{
			grid[row][col].currState = rndmState(STATENUM);	
			grid[row][col].currDir = NORTH;
//			iprint(grid[row][col].currState);
		}
	}
}

int getMiddle(int size)	{

	return (size/2);

}

void directionTest(Square **grid, int *col, int *row)	{
	int x =*col; 
	int y =*row; 
	//printf("current square row : %d Col : %d\n",*row,*col);
	switch (grid[(*row)][(*col)].currDir) {

		case NORTH:
	//	printf("Facing North\n");
				testCurr(grid[(y)][(x)].currDir,grid,*col, --(*row));	
				break;

		case EAST:
	//	printf("Facing East\n");
				testCurr(grid[(y)][(x)].currDir,grid,++(*col), *(row)); 

				break;

		case SOUTH:
	//	printf("Facing South\n");
				testCurr(grid[(y)][(x)].currDir,grid,*col, ++(*row)); 

				break;

		case WEST:
	//	printf("Facing West\n");
				testCurr(grid[(y)][(x)].currDir,grid,--(*col), *row);
				break;

		default:
				break;

	}


}

void testCurr(direction current, Square **grid, int col, int row)	{

	grid[row][col].currDir=current;
//	printf("now I have moved to ");
//	printf("row: %d col: %d. Current ",row,col);

	switch(grid[row][col].currState)	{

		case BLACK:
//					printf("Square is black, turn left \n");
					grid[row][col].currState = WHITE;
					if(grid[row][col].currDir == NORTH)	{
						grid[row][col].currDir = WEST;
					} else {
						grid[row][col].currDir--;
					}
					break;

		case WHITE:
//					printf("square is white, turn right");
					grid[row][col].currState = BLACK;	
					if(grid[row][col].currDir == WEST)	{
						grid[row][col].currDir = NORTH;
					} else {
						grid[row][col].currDir++;
					}
//					printf(" so facing %d \n", grid[row][col].currDir);
					break;
		default:

					break;
	}
//	printf("After\n");
	if(grid[col][row].currDir == NORTH)	{
//		printf("Facing North\n");
	} else if(grid[col][row].currDir == EAST)	{
//		printf("Facing east\n");
	 
	} else if(grid[col][row].currDir == SOUTH)	{
//		printf("Facing south\n");

	} else if(grid[col][row].currDir == WEST)	{
//		printf("Facing west\n");
	}
//    iprint(grid[yPos][xPos].currState);
//    iprint(grid[yPos][xPos].currDir);
}

void printGrid(SDL_Simplewin sw, Square **grid, int size)	{
	
	SDL_Rect rectangle;
	rectangle.w = RECTSIZE;
	rectangle.h = RECTSIZE;

	int row, col;	
	for(row = 0; row < size; row++)	{
		for (col = 0; col < size; col++)	{
		//	iprint(grid[row][col].currState);
			switch (grid[row][col].currState)	{
				case	WHITE:
						//printf(" # ");
						Neill_SDL_SetDrawColour(&sw,255,255,255);
						rectangle.x =(WWIDTH -(col*RECTSIZE));
						rectangle.y = (WHEIGHT-(row*RECTSIZE)); 
						//iprint(rectangle.x);
						//iprint(rectangle.y);
						SDL_RenderFillRect(sw.renderer, &rectangle);
						break;

				case BLACK:
						Neill_SDL_SetDrawColour(&sw,0,0,0);
						rectangle.x = (WWIDTH -(col*RECTSIZE));
						rectangle.y = (WHEIGHT-(row*RECTSIZE)); 
						//iprint(rectangle.x);
						//iprint(rectangle.y);
						SDL_RenderFillRect(sw.renderer, &rectangle);
						//printf(" . ");
						break;
				default:
						break;			
			}
		}
	}
}

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
