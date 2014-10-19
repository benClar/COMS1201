//
// life.c
// Created by benji on 2014/17/10
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/defFunct.h"
#include ".headers/life.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/

int main(){
	srand(time(NULL));
	int size = 25;
	Lifeform **grid1 = makeGrid(size);
	Lifeform **grid2 = makeGrid(size);

	initGrid(grid1,size);
	initGrid(grid2,size);

	times(grid1,grid2,size,10);
	return 0;

}

/*---------- Functions ----------*/

void times(Lifeform **gridOne, Lifeform **gridTwo, int size, int n )
{
	int	i, cycle=1;
	for (i = 0; i < n; i++)	{
		printf("cycle:	%d \n",i);
		if (cycle)	{
			iprint(cycle);
			printGrid(gridOne,size);
			nextState(gridOne,gridTwo,size);
			cycle--;
		} else	{
			printGrid(gridTwo,size);
			nextState(gridTwo,gridOne,size);	
			cycle++;
		}
		pNL();
	}	


}


//creating grid
Lifeform **makeGrid(int size)    {
	
	int	i;
    Lifeform **array;
    array = (Lifeform**) malloc(size*sizeof(Lifeform*));
	for (i = LOWERBOUND; i < size; i++)	{
		array[i] = malloc(size * (sizeof(Lifeform)));
	}

	return array;	
}

//generated next state of grid
void nextState(Lifeform **gridOne, Lifeform **gridTwo, int size)	{

    int row, col, liveCnt;
    for(row = LOWERBOUND; row < size; row++) {
        for (col = LOWERBOUND; col < size; col++)    {
			liveCnt= checkNeighbour(row,col,gridOne, size);
	//		printf("status: %d, live neighbours of %d, %d:	%d\n",gridOne[row][col].s1,row,col,liveCnt);
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

void printGrid(Lifeform **grid, int size)	{

	int	row, col;
	for(row = LOWERBOUND; row < size; row++)	{
		for (col = LOWERBOUND; col < size; col++)	{
			if (grid[row][col].s1 == ALIVE)	{
				printf(" X ");
			} else {
				printf(" - ");
			}
		}
		pNL();
	} 
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
	life->x=row;
	life->y=col;
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
