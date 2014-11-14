//
// maze.c
// Created by bclarke on 2014/10/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*---------- Custom Headers	-----------*/

#include ".headers/neillsdl2.h"
#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"
#include ".headers/printingModule.h"
#include ".headers/maze.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	SDL_Simplewin sw;
	RouteMode rMode;
	PathList exitCoords = createList();
	printf("Which mode would you like? \n 1 : Show full route with decision making(non graphical only). \n 2 : Step by step movement through maze with no wrong turns .\n 3 : Static route of maze\n");
	do { 
		scanInt((int*) &rMode);
	} while (rMode != DECISIONS && rMode != CORRECT && rMode != STATIC);
	int eRow, eCol, gMode = 0;
	argc--;
	if(argc > 1 && rMode != DECISIONS){
		if(!strcmp(argv[2],"SDL")){
			gMode = 1;
			Neill_SDL_Init(&sw);
		}
	}
	MazeMap Maze = readMaze(argv[1]);
	if(findEntrance(Maze, &eRow, &eCol))	{
		exploreMaze(Maze, eRow, eCol,gMode,sw,exitCoords,rMode);
		cleanList(Maze,exitCoords);
		printFull(Maze,sw,gMode,rMode);
		printCorrect(Maze,sw,gMode,rMode,exitCoords);
	}
		
	return 0;

}

/*---------- Functions ----------*/
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw,PathList exitCoords,RouteMode rMode)	{
	if (!mazeBoundaryCheck(Maze, row, col)) {  return 0;}	
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}
	if (detectExit(Maze,row,col)) {	
		setBlockType(Maze,row,col,EXITROUTE);  
		addNode(exitCoords,row,col);
		if(rMode == DECISIONS)	{
			printFullRoute(Maze);
		}
		return 1;
	}

		switch(getBlockType(Maze,row,col))	{
		case WALL:
				return 0;
				break;
		default:
				setBlockType(Maze,row,col,EXITROUTE);
				if(rMode == DECISIONS)	{
					printFullRoute(Maze);
				}	
				addNode(exitCoords,row,col);
				if(exploreMaze(Maze,row+UP,col,gMode,sw,exitCoords,rMode)) { return 1;}
				if(exploreMaze(Maze,row+DOWN,col,gMode,sw,exitCoords,rMode)) { return 1;} 
				if(exploreMaze(Maze,row,col+LEFT,gMode,sw,exitCoords,rMode)) { return 1; } 
				if(exploreMaze(Maze,row,col+RIGHT,gMode,sw,exitCoords,rMode)) { return 1; }
				break;
	}
	
	setBlockType(Maze,row,col,DEADEND);
	return 0;	

}

int printCorrect(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode,PathList exitCoords)	{

	if (rMode ==CORRECT)	{
		if(gMode)	{
			graphicalPrintRightRoute(Maze,exitCoords,sw);
			return 2;
		} else  {
			printCorrectRoute(Maze,exitCoords);
		}
	}

	return 1;
}

int printFull(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode)	{
	if(rMode == STATIC)	{
		if(gMode)	{
		while(!sw.finished)	{
			graphicalPrintFullRoute(Maze,sw);	
			Neill_SDL_Events(&sw);
		}
			return 2;
		} else {
			printFullRoute(Maze);
		}
	}
	return 1;
}

MazeMap readMaze(char fileLocation[])	{
	FILE *fp;
	char letter;
	int lineCount = 0, rowDim = 0, colDim = 0,row = 0, col = 0;
	MazeMap Maze = NULL;
	
	if ((fp = fopen(fileLocation,"r")) != NULL) {
		getFirstLine(fp,&rowDim,&colDim);
		Maze = createMap(rowDim,colDim);
		while((letter = getc(fp)) != EOF && lineCount < rowDim)	{
			if(letter == '\n')	{
				lineCount++;
			}
			addToGrid(Maze,&row,&col,letter,'#');	
		}
	} else {
		fprintf(stderr, "File does not exist\n");	
		exit(1);
	}
	return Maze;
}

int getFirstLine(FILE *fp,int *row, int *col)	{

	int count = 0, argCnt;
	while((argCnt = (fscanf(fp,"%d %d",row,col))) != EOF && count < 1)	{
		count++;	
	} 
	
	return argCnt;
}

void scanInt(int *toScan){

        while(!scanf("%d", toScan))       {
               clearInputBuf
               printf("please enter number\n");
        }
}

int findEntrance(MazeMap Maze,int *eRow, int *eCol) {

    int side,top;
    BOOL entranceFound;
    for(side = 0,entranceFound=FALSE; side < getHeight(Maze) && entranceFound == FALSE; side++)    {
        if((getBlock(Maze,side,LEFTSIDE)) == ' ') {
            entranceFound = TRUE;
        }
    }

    for(top = 0; top < getWidth(Maze); top++)  {
        if((getBlock(Maze,TOPSIDE,side)) == ' ')    {
            if(--side >= top)   {
                *eRow = TOPSIDE;
                *eCol = top;
                return(setBlockType(Maze,TOPSIDE,top,ENTRANCE));
            }
        }
    }
        *eRow = --side;
        *eCol = LEFTSIDE;
        return(setBlockType(Maze,side,LEFTSIDE,ENTRANCE));
}

int detectExit(MazeMap Maze, int row, int col)  {
    if((row == (getWidth(Maze) -1) || col == (getHeight(Maze) - 1) || row == 0 || col == 0)  && (getBlock(Maze,row,col) == ' ') && (getBlockType(Maze,row,col) != ENTRANCE) ) {
        return 1;
    }
    return 0;
}


