//
// mazeGeneratorModule.c
// Created by bc14627 on 2014/15/11
// tags: 
//
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
/*---------- Symbolic Constants  -----------*/

/*---------- Data Structures ----------*/

/*---------- Functions ----------*/

int partionMaze(MazeMap RndmMaze, int leftBound, int width, int stRow, int lowerBound)	{
//	if(lowerBound - stRow < 4)	{ return 0; }

//	int colStart,  //!! Randomised x pos for new col start
//		newRowStart,	//! randomised y pos for new row start
//		rowPos, //! y pos for new col start
//		colPos; //! x pos for new row
//	do {
//		colStart = rand()%(width-1); //!setting x pos for col
//	} while (colStart < (leftBound-1));   	
//
//	//!Drawing column
//	for(rowPos = stRow; getBlockType(RndmMaze,rowPos,colStart) != WALL; rowPos++)	{
//		setBlockType(RndmMaze,rowPos,colStart,WALL);		
//		printFullRoute(RndmMaze);
//	}
//	
//	do	{
//		newRowStart = rand()%(stRow - 1); //!setting y pos for row
//	} while ( (newRowStart > (rowPos - 1)));
//
//	//!drawing row
//	for(colPos = leftBound; getBlockType(RndmMaze,newRowStart,colPos) != WALL; colPos++)	{
//		setBlockType(RndmMaze,newRowStart,colPos,WALL);
//	} 

	//partionMaze(RndmMaze,newRowStart,colStart,stRow,newRowStart);	

	return 1;
			
}

void wallMaze(MazeMap RndmMaze)	{

	int topRow = 0,
		bottomRow = (getHeight(RndmMaze)-1),
		leftCol = 0,
		rightCol = (getWidth(RndmMaze)-1),
		i,r,c;
		//!Setting Default Space
		for(r = 0; r < getWidth(RndmMaze); r++)	{
			for(c = 0; c < getWidth(RndmMaze); c++)	{
				addToGrid(RndmMaze,r,c,' ','#');	
			}
		}
		//!Adding top wall
		for(i = 0; i < getWidth(RndmMaze); i++)	{
			addToGrid(RndmMaze,topRow,i,'#','#');	
		}
		//!Adding Bottom wall
		for(i = 0; i < getWidth(RndmMaze); i++) {
            addToGrid(RndmMaze,bottomRow,i,'#','#');
        }
		//!Adding Left Wall
		for(i = 0; i < getHeight(RndmMaze); i++) {
            addToGrid(RndmMaze,i,leftCol,'#','#');
        }
		//!Adding Right Wall
		for(i = 0; i < getHeight(RndmMaze); i++) {
            addToGrid(RndmMaze,i,rightCol,'#','#');
        }
		
}

