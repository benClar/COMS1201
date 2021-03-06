//! Maze.c
/*!
 *	Main File.  Reads maze files and explores them for an exit
 *	For this assignment I have fully implemeted the recursive exit route and the SDL output.  I have attempted the random maze generation, however it is not fully correct.
 *	To run: 1) make all
 *			2) .compiled/maze /FILEPATH
 *			3) .compiled/maze /FILEPATH SDL
 *			4) .compiled/maze RANDOM
 *			5) .compiled/maze RANDOM SDL
 */

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
/*---------- Custom Headers	-----------*/

#include "neillsdl2.h"
#include "debug.h"
#include "mazeDataFunctions.h"
#include "coordListModule.h"
#include "printingModule.h"
#include "mazeGeneratorModule.h"
#include "maze.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
 	srand(time(NULL));
	MazeMap Maze;
	SDL_Simplewin sw;
	RouteMode rMode;
	int eRow, eCol, gMode = 0;

	PathList exitCoords = createList();

	printf("Which mode would you like? \n 1 : Show full route with decision making.\n 2 : Step by step movement through maze with no wrong turns.\n 3 : Static route of maze.\n");
	do { 
		scanInt((int*) &rMode);
	} while (rMode != DECISIONS && rMode != CORRECT && rMode != STATIC);

	argc--;
	//! check is user wants to use SDL
	if(argc > 1){
		if(!strcmp(argv[2],"SDL")){
			gMode = 1;
			Neill_SDL_Init(&sw);
		}
	}
	
	if(!strcmp(argv[1],"RANDOM"))	{
		Maze = createRandomMaze();
	} else	{
		Maze = readMaze(argv[1]);
	}

	if(findEntrance(Maze, &eRow, &eCol))	{
		//! Entrance Found
		if(exploreMaze(Maze, eRow, eCol,gMode,sw,exitCoords,rMode)) {
			//!escape found
			cleanList(Maze,exitCoords); //!removes DEADENDS from linked list of exit path
			printFull(Maze,sw,gMode,rMode);
			printCorrect(Maze,sw,gMode,rMode,exitCoords);
		 } else {
			printFull(Maze,sw,gMode,rMode);
            printCorrect(Maze,sw,gMode,rMode,exitCoords);
		    fprintf(stderr, "Maze cannot be escaped\n");
       		exit(1);
		}
	} else {
	    fprintf(stderr, "Maze has no Entrance\n");
        exit(1);
	}
	
	freeListNodes(exitCoords);
	free(exitCoords);
	freeMazeBlocks(Maze);
	free(Maze);

	return 0;

}

/*---------- Functions ----------*/

/*
 * Creates Randomised Map
 */

MazeMap createRandomMaze()	{
	int randomW,randomH;
	printf("Enter height for maze:	\n");	
	rParam(&randomH,MINHEIGHT,MAXHEIGHT);
	printf("Enter Width for maze:	\n");	
	rParam(&randomW,MINWIDTH,MAXWIDTH);
	MazeMap Maze = createMap(randomW,randomH);
	wallMaze(Maze);
	partionMaze(Maze,0,randomH,0,randomW); 
	return Maze;
}

/*!
 *Validates user input for random maze generation
 */
void rParam(int *randomD, int compMin, int compMax)	{
    do  {
       scanInt(randomD);
    } while ((*randomD > compMax) || (*randomD <= compMin));
}

/*!
 * Explores the maze recursively, testing each non-wall element to detect if it is the exit
 */
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw,PathList exitCoords,RouteMode rMode)	{
	Neill_SDL_Events(&sw);
	if(sw.finished) { return 1; }
	if (!mazeBoundaryCheck(Maze, row, col)) {  return 0;}	
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}
	if(getBlockType(Maze,row,col) == DEADEND) { return 0;}
	//! Detecting if exit
	if (detectExit(Maze,row,col)) {	
		setBlockType(Maze,row,col,EXITROUTE);  
		addNode(exitCoords,row,col);
		if(rMode == DECISIONS)	{
			printFull(Maze,sw,gMode,rMode);
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
					printFull(Maze,sw,gMode,rMode);
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

/*!
 * Manages interface to printing module if user wants correct decisions explore mode
 */
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

/*!
 *Manages interface to printing module if user wants to see only full exit path
 */
void printFull(MazeMap Maze, SDL_Simplewin sw, int gMode,RouteMode rMode)	{
	delay(DELAY);
	if(rMode == STATIC && (gMode))	{
        while(!sw.finished)   {
			graphicalPrintFullRoute(Maze,sw);
            Neill_SDL_Events(&sw);
        }
	} else if (rMode == DECISIONS && gMode)	{
			graphicalPrintFullRoute(Maze,sw);	
	} else if ((rMode == DECISIONS || rMode == STATIC) && (!gMode)) {
			printFullRoute(Maze);
	}
}

/*!
 * Reads maze from file and sends each character to Maze Grid Module for processing
 */
MazeMap readMaze(char fileLocation[])	{
	FILE *fp;
	char letter;
	int lineCount = 0, width = 0, height = 0, col = 0;
	MazeMap Maze = NULL;
	
	if ((fp = fopen(fileLocation,"r")) != NULL) {
		getFirstLine(fp,&width,&height); //!Reading in Dimensions
		if (width > FMAXWIDTH || height > FMAXHEIGHT)	{
			fprintf(stderr,"Dimensions too big: %d %d\n",width,height);
			exit(1);
		}
		Maze = createMap(width,height);
		while((letter = getc(fp)) != EOF && lineCount < height)	{
			if(letter == '\n')	{
				if(!checkDim(col,width))	{
					fprintf(stderr, "row %d is %d long, when dimensions state it should be %d\n",lineCount+1,col,width);
					exit(1);
				}
				lineCount++;
				col = 0;
			}  else {
				addToGrid(Maze,lineCount,col,letter,'#');	//!	adding to grid,defines wall char
				col++;
			}
		}
	} else {
		fprintf(stderr, "File does not exist\n");	
		exit(1);
	}
	if((!checkDim(lineCount,height)))	{
		fprintf(stderr, "Height of maze (%d)  does not match given size (%d)\n",lineCount,height);
        exit(1);
	}
	return Maze;
}

/*!
 * Validates dimensions given in file
 */
int checkDim(int count, int dim)	{

	if( count == dim)	{
		return 1;
	}

	return 0;	
}

/*!
 *Reads first line as integer to get maze dimensions
 */
int getFirstLine(FILE *fp,int *width, int *height)	{

	int count = 0, argCnt;
	while((argCnt = (fscanf(fp,"%d %d",width,height))) != EOF && count < 1)	{
		count++;	
	} 
	
	return argCnt;
}

/*!
 *Safely Scans integers from keyboard
 */
void scanInt(int *toScan){

        while(!scanf("%d", toScan))       {
               clearInputBuf
               printf("please enter number\n");
        }
}

/*!
 *Searches maze for Entrance cooridinates
 */
int findEntrance(MazeMap Maze,int *eRow, int *eCol) {

    int side,top;
    BOOL entranceFound;
    for(side = 0,entranceFound=FALSE; side < getHeight(Maze) && entranceFound == FALSE; side++)    {
        if((getBlock(Maze,side,LEFTSIDE)) == ' ') {
            entranceFound = TRUE;
        }
    }

	//! testing if there is a closer entrance on top of maze
    for(top = 0; top < getWidth(Maze); top++)  {
        if((getBlock(Maze,TOPSIDE,top)) == ' ')    {
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

/*!
 *Tests current position to determine if it is an exit
 */
int detectExit(MazeMap Maze, int row, int col)  {
    if((row == (getHeight(Maze) -1) || col == (getWidth(Maze) - 1) || row == 0 || col == 0)  && (getBlock(Maze,row,col) == ' ') && (getBlockType(Maze,row,col) != ENTRANCE) ) {
        return 1;
    }
    return 0;
}


