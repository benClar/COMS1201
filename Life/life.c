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
#include ".headers/life.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/

int main(int argc, char **argv) {
	srand(time(NULL));
	int size, cycles, argCnt = argc - 1;
		
	mode m1 = setMode(argCnt, argv[1]);

	getSize(&size, &cycles);
	Lifeform **grid1 = makeGrid(size);
	Lifeform **grid2 = makeGrid(size);

	initGrid(grid1,size);
	initGrid(grid2,size);

	times(grid1,grid2,size,cycles,m1);
	return 0;

}

/*---------- Functions ----------*/

//switches off graphics in debug mode
mode setMode(int argc, char *mode)
{
	if (argc)	{
		if (strnCompare(mode,"debug")) {
			printf("***debug mode***\n");
			return DEBUG;	
		}
	}
	return GRAPHIC;
}


//get size and cycle number.
void getSize(int *size, int *cycles)	
{
	//add validation
	printf("Enter Size of grid desired:	\n");
	scanInt(size);
	printf("Enter Cycles desired:	\n");
	scanInt(cycles);

}

//validated scanf function for ints
void scanInt(int *toScan)	{

	while(!scanf("%d", toScan))       {
               cL
               printf("Please enter int\n");
	}	
}

//controls grids 
void times(Lifeform **gridOne, Lifeform **gridTwo, int size, int n, mode m1)
{
	int i, cycle=1;
	for (i = 0; i < n; i++)	{
		printf("cycle:	%d \n",i);
		if (cycle)	{
			if (m1)	{ //graphic mode
				printGrid(gridOne,size);
			}
			nextState(gridOne,gridTwo,size,m1);
			cycle--;
		} else	{
			if (m1)	{
				printGrid(gridTwo,size);
			}
			nextState(gridTwo,gridOne,size,m1);	
			cycle++;
		}
		delay(DELAY);	
	}	
}

//creating delay
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
    array = (Lifeform**) malloc(size*sizeof(Lifeform*));
	for (i = LOWERBOUND; i < size; i++)	{
		array[i] = malloc(size * (sizeof(Lifeform)));
	}

	return array;	
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
void printGrid(Lifeform **grid, int size)	{

	int	row, col;
	for(row = LOWERBOUND; row < size; row++)	{
		for (col = LOWERBOUND; col < size; col++)	{
			if (grid[row][col].s1 == ALIVE)	{
				printf(" * ");
			} else {
				printf("   ");
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

//compares strings returning true or false value 
int strnCompare(char stringOne[], char stringTwo[])     {

        int i, aC, bC;
        if (strlen(stringOne) == strlen(stringTwo))     {
                for(i = 0, aC = stringOne[i], bC = stringTwo[i]; aC != '\0' && bC != '\0'; i++, aC = stringOne[i], bC=stringTwo[i])     {
                                if (aC != bC)   {
                                        return 0;
                                }

                }
        } else {
		return 0;
	}
        return 1;
}
