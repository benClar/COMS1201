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
#include ".headers/maze.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	SDL_Simplewin sw;
	int eRow, eCol, gMode = 0;
	argc--;
	iprint(argc);
	if(argc > 1){
		if(!strcmp(argv[2],"SDL")){
			gMode = 1;
			Neill_SDL_Init(&sw);
		}
	}
	sprint(argv[2]);
	MazeMap Maze = readMaze(argv[1]);
	printMap(Maze);
	if(findEntrance(Maze, &eRow, &eCol))	{
		exploreMaze(Maze, eRow, eCol,gMode,sw);
	}
		
	return 0;

}

/*---------- Functions ----------*/
//int exploreMaze(MazeMap Maze, int row, int col, SDL_Simplewin sw, int gMode)	{
int exploreMaze(MazeMap Maze, int row, int col,int gMode, SDL_Simplewin sw)	{
	if (!mazeBoundaryCheck(Maze, row, col)) {  return 0;}	
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}
	if (detectExit(Maze,row,col)) {	
		setBlockType(Maze,row,col,EXITROUTE);  
		//printMap(Maze); 
		printing(Maze,sw, gMode);
		return 1;
	}

		switch(getBlockType(Maze,row,col))	{
		case WALL:
				return 0;
				break;
		default:
				printing(Maze,sw, gMode);
				setBlockType(Maze,row,col,EXITROUTE);
				printing(Maze,sw, gMode);
				if(exploreMaze(Maze,row+UP,col,gMode,sw)) { return 1;}
				if(exploreMaze(Maze,row+DOWN,col,gMode,sw)) { return 1;} 
				if(exploreMaze(Maze,row,col+LEFT,gMode,sw)) { return 1; } 
				if(exploreMaze(Maze,row,col+RIGHT,gMode,sw)) { return 1; }
				break;
	}
	
	setBlockType(Maze,row,col,DEADEND);

	return 0;	

}

int printing(MazeMap Maze, SDL_Simplewin sw, int gMode)	{
	if(gMode)	{
		graphicalPrint(Maze,sw);	
		return 2;
	} else {
		printMap(Maze);
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
