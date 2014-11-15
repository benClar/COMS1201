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
#include ".headers/chamberStack.h"
#include ".headers/mazeGeneratorModule.h"
/*---------- Symbolic Constants  -----------*/

/*---------- Data Structures ----------*/

struct chamber {

	int top;
	int left;
	int bottom;
	int right;

};

struct chStack {

	Chamber first;
	Chamber last;
	int nChamber;

};

/*---------- Functions ----------*/

void partionMaze(MazeMap RndmMaze, int row, int col)	{
	int cWallCount=0, rWallCount =0;
	int colSet, rowSet;
	int rndmRowSp = rand()%getWidth(RndmMaze);
	//int rndmColSp = rand()%getWidth(RndmMaze);

	//!add random col with space
	for(rowSet = 0;rowSet < getHeight(RndmMaze) && cWallCount < 2 ;rowSet++)   {
		if(getBlockType(RndmMaze,rowSet,col) == WALL)  {
			cWallCount++;	
		}
    	addToGrid(RndmMaze,rowSet,col,'#','#');
    }

	//!add random row with space
	for(colSet = 0; colSet < getWidth(RndmMaze) && rWallCount < 3 ;colSet++)	{
		if(getBlockType(RndmMaze,row,colSet) == WALL)	{
			rWallCount++;
			if(rWallCount == 2)	{
				
			}
		}
		if(colSet == rndmRowSp)	{
			addToGrid(RndmMaze,row,colSet,' ','#');
		} else { 		
			addToGrid(RndmMaze,row,colSet,'#','#'); 
		}
	}
}

void wallMaze(MazeMap RndmMaze)	{

	int topRow = 0,
		bottomRow = (getHeight(RndmMaze)-1),
		leftSide = 0,
		rightSide = (getWidth(RndmMaze)-1),
		i,r,c;
		//!Setting Default Space
		for(r = 0; r < getWidth(RndmMaze); r++)	{
			for(c = 0; c < getWidth(RndmMaze); c++)	{
				addToGrid(RndmMaze,r,c,' ','#');	
			}
		}
		iprint(rightSide);
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
            addToGrid(RndmMaze,i,leftSide,'#','#');
        }
		//!Adding Right Wall
		for(i = 0; i < getHeight(RndmMaze); i++) {
            addToGrid(RndmMaze,i,rightSide,'#','#');
        }
		
}

