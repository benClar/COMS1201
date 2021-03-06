//! Maze.c
/*!
 *	Main File.  Reads maze files and explores them for an exit
 */

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
#include ".headers/mazeGeneratorModule.h"
#include ".headers/maze.h"

//TODO:
//	deal with no exit mazes
//	look at graphic print : not very DRY
//	Random Generate 
//

/*---------- Main -----------*/

int main(int argc, char *argv[]){
 	//srand(time(NULL));
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
/*!
 * Explores the maze recursively, testing each non-wall element to detect if it is the exit
 */
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw,PathList exitCoords,RouteMode rMode)	{
	if (!mazeBoundaryCheck(Maze, row, col)) {  return 0;}	
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}
	//! Detecting if exit
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

/*!
 * Reads maze from file and sends each character to Maze Grid Module for processing
 */
MazeMap readMaze(char fileLocation[])	{
	FILE *fp;
	char letter;
	int lineCount = 0, rowDim = 0, colDim = 0, col = 0;
	MazeMap Maze = NULL;
	
	if ((fp = fopen(fileLocation,"r")) != NULL) {
		getFirstLine(fp,&rowDim,&colDim); //!Reading in Dimensions
		Maze = createMap(rowDim,colDim);
		while((letter = getc(fp)) != EOF && lineCount < rowDim)	{
			if(letter == '\n')	{
				lineCount++;
				col = 0;
			}  else {
				addToGrid(Maze,lineCount,col,letter,'#');	//!adding to grid,defines wall char
				col++;
			}
		}
	} else {
		fprintf(stderr, "File does not exist\n");	
		exit(1);
	}
	return Maze;
}

//MazeMap readMaze(char fileLocation[])	{
//	FILE *fp;
//	char letter;
//	int lineCount = 0, rowDim = 0, colDim = 0,row = 0, col = 0;
//	MazeMap Maze = NULL;
//	
//	if ((fp = fopen(fileLocation,"r")) != NULL) {
//		getFirstLine(fp,&rowDim,&colDim); //!Reading in Dimensions
//		Maze = createMap(rowDim,colDim);
//		while((letter = getc(fp)) != EOF && lineCount < rowDim)	{
//			if(letter == '\n')	{
//				lineCount++;
//			} 
//			addToGrid(Maze,&row,&col,letter,'#');	//!adding to grid, defining what is a wall charactor
//		}
//	} else {
//		fprintf(stderr, "File does not exist\n");	
//		exit(1);
//	}
//	return Maze;
//}

/*!
 *Reads first line as integer to get maze dimensions
 */
int getFirstLine(FILE *fp,int *row, int *col)	{

	int count = 0, argCnt;
	while((argCnt = (fscanf(fp,"%d %d",row,col))) != EOF && count < 1)	{
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
	
	//! testing if there is a close entrance on top of maze
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

/*!
 *Tests current position to determine if it is an exit
 */
int detectExit(MazeMap Maze, int row, int col)  {
    if((row == (getWidth(Maze) -1) || col == (getHeight(Maze) - 1) || row == 0 || col == 0)  && (getBlock(Maze,row,col) == ' ') && (getBlockType(Maze,row,col) != ENTRANCE) ) {
        return 1;
    }
    return 0;
}


