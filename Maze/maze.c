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

#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/maze.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	int eRow, eCol;
	argc--;
	MazeMap Maze = readMaze(argv[1]);
	printMap(Maze);
	if(findEntrance(Maze, &eRow, &eCol))	{
		exploreMaze(Maze,eRow,eCol);
	}	
	return 0;

}

/*---------- Functions ----------*/

int exploreMaze(MazeMap Maze, int row, int col)	{
	if (!mazeBoundaryCheck(Maze, row, col)) {  return 0;}	
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}
	if (detectExit(Maze,row,col)) {	
		setBlockType(Maze,row,col,EXITROUTE);  printMap(Maze); 
		return 1;
	}
		printMap(Maze);
		switch(getBlockType(Maze,row,col))	{
		case WALL:
				return 0;
				break;
		default:
				printMap(Maze);
				iprint(col);	
				iprint(row);
				setBlockType(Maze,row,col,EXITROUTE);
				if(exploreMaze(Maze,row+UP,col)) { return 1;}
				if(exploreMaze(Maze,row+DOWN,col)) { return 1;} 
				if(exploreMaze(Maze,row,col+LEFT)) { return 1; } 
				if(exploreMaze(Maze,row,col+RIGHT)) { return 1; }
				break;
	}
	
	setBlockType(Maze,row,col,DEADEND);

	return 0;	

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
