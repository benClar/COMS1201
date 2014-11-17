//! mazeGeneratorModule.c
/*
 * Partial implementation of maze generator.  May segfault (give it another go!). SDL compatible. 
 */

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/

#include ".headers/neillsdl2.h"
#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"
#include ".headers/printingModule.h"
#include ".headers/mazeGeneratorModule.h"

/*---------- Functions ----------*/

int partionMaze(MazeMap RndmMaze, 
	int leftBound,  //! Lefthand Boundary of current chamber
	int width,		//! RIght hand boundary of current chamber 
	int stRow,		//! top boundary of current chamber 
	int lowerBound)	{ //!Lower boundary of current chamber
	if(lowerBound - stRow < CHAMBERSIZE)	{ return 0; }
	if(width - leftBound < CHAMBERSIZE) { return 0; }

	int colStart,  //! Randomised x pos for new col start
		newRowStart,	//! randomised y pos for new row start
		rowPos, //! y pos for new col start
		colPos; //! x pos for new row

	do {
		colStart = rand()%(width-2); //!setting x pos for col
	} while (colStart <= (leftBound+1));   	

	//!Drawing column
	for(rowPos = stRow+1; getBlockType(RndmMaze,rowPos,colStart) != WALL && (lowerBound - stRow > CHAMBERSIZE); rowPos++)	{
		addToGrid(RndmMaze,rowPos,colStart,'#','#');
	}

	do	{
		newRowStart = rand()%(lowerBound - 2); //!setting y pos for row
	} while ( newRowStart <= (stRow + 1));

	//!drawing row of first half
	for(colPos = leftBound+1; getBlockType(RndmMaze,newRowStart,colPos) != WALL && (width - leftBound > CHAMBERSIZE); colPos++)	{
		addToGrid(RndmMaze,newRowStart,colPos,'#','#');
	}

	//! top left chamber
	partionMaze(RndmMaze,leftBound, colStart, stRow, newRowStart);	

	//! bottom left chamber
	partionMaze(RndmMaze,leftBound,colStart,newRowStart,rowPos);

	//!drawing row of second half
	for(colPos = colPos+1; getBlockType(RndmMaze,newRowStart,colPos) != WALL && (width - leftBound > CHAMBERSIZE); colPos++)  {
        addToGrid(RndmMaze,newRowStart,colPos,'#','#');
    }
	//!top right chamber
	partionMaze(RndmMaze,colStart,colPos,stRow,newRowStart);

	//!bottom right chamber	
	partionMaze(RndmMaze,colStart,colPos,newRowStart,rowPos);

	return 1;
			
}

void wallMaze(MazeMap RndmMaze)	{

	int topRow = 0,
		bottomRow = (getHeight(RndmMaze)-1),
		leftCol = 0,
		rightCol = (getWidth(RndmMaze)-1),
		i,r,c;
	int entrance1 = rand()%getWidth(RndmMaze),
		entrance2 = rand()%getHeight(RndmMaze),
		entranceDec = rand()%2;
	int exitCnt = 0; 

		//!Setting Default Space
		for(r = 0; r < getWidth(RndmMaze); r++)	{
			for(c = 0; c < getWidth(RndmMaze); c++)	{
				addToGrid(RndmMaze,r,c,' ','#');	
			}
		}
		//!Adding top wall
		for(i = 0; i < getWidth(RndmMaze); i++)	{
			if((entranceDec) && i == entrance1 && (i != leftCol))	{
				addToGrid(RndmMaze,topRow,i,' ','#');
			}else {
				addToGrid(RndmMaze,topRow,i,'#','#');
			} 
		}
		//!Adding Bottom wall
		for(i = 0; i < getWidth(RndmMaze); i++) {
			if (((rand()%EXITDEC) == EXITCHNCE  ) && exitCnt < NOEXITS && (i != 0) ) {
					addToGrid(RndmMaze,bottomRow,i,' ','#');
					exitCnt++;
            } else {
				addToGrid(RndmMaze,bottomRow,i,'#','#');
			}
        }
		//!Adding Left Wall
		for(i = 0; i < getHeight(RndmMaze); i++) {
			if((!entranceDec) && i == entrance2 && (i != topRow))	{
				addToGrid(RndmMaze,i,leftCol,' ','#');
			} else	{	
				addToGrid(RndmMaze,i,leftCol,'#','#');
			}
        }
		//!Adding Right Wall
		for(i = 0; i < getHeight(RndmMaze); i++) {
			if (((rand()%EXITDEC) == EXITCHNCE) && (exitCnt < NOEXITS) && (i != 0) ) {
				addToGrid(RndmMaze,i,rightCol,' ','#');	
					exitCnt++;
			} else {
   	        	 addToGrid(RndmMaze,i,rightCol,'#','#');
			}
        }
}

