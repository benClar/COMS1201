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
	argc--;
	MazeMap Maze = readMaze(argv[1]);
	printMap(Maze);
	iprint(findEntrance(Maze));
	//setExits(Maze,(getHeight(Maze)-1),(getWidth(Maze)-1),INCREMROW); //setting Exit
	//exploreMaze(Maze,setExits(Maze,TOPWALL,LEFTSIDE,DECREMROW),LEFTSIDE);//setting Entrance
	//getBlockType(Maze,(setExits(Maze,(getHeight(Maze)-1),(getWidth(Maze)-1),INCREMROW)),(getWidth(Maze)-1)); displays type of exit
	return 0;

}

/*---------- Functions ----------*/

int exploreMaze(MazeMap Maze, int row, int col)	{
	
	if (!mazeBoundaryCheck(Maze, row, col)) { return 0;}	
	if (getBlockType(Maze,row,col) == EXIT) { setBlockType(Maze,row,col,EXITROUTE); printMap(Maze); printf("exit\n"); return 1;}
	if (getBlockType(Maze,row,col) == EXITROUTE) { return 0;}

	printMap(Maze);

	switch (getBlockType(Maze,row,col))	{
		case WALL:
				return 0;
				break;
		default:
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
	//char **dimensions=createStringArray(2,3);
	FILE *fp;
	char letter;
	int lineCount = 0,scanned = 0, rowDim = 0, colDim = 0,row = 0, col = 0;
	MazeMap Maze = NULL;
	if ((fp = fopen(fileLocation,"r")) != NULL) {
		while((letter = getc(fp)) != EOF)	{
			if(letter == '\n')	{
				lineCount++;
			}
			//First line only
			if((!scanned) && (!lineCount) && (isdigit(letter)))	{	
					//getFirstLine(fp,&rowDim,&colDim);
					rowDim = colDim = 10;	
					//iprint(rowDim);
					//iprint(colDim);
					//gettingDimensions(dimensions,letter,charCount++, &rowDim, &colDim);
					Maze = createMap(rowDim,colDim);
					scanned++;
			} else if(lineCount && scanned) {
				//build map
					addToGrid(Maze,&row,&col,letter,'#');	
			}
				
		}
	} else {
		fprintf(stderr, "File does not exist\n");	
		exit(1);
	}
	
	return Maze;
}

int getFirstLine(FILE *fp,int *row, int *col)	{

	int testr, testc;
	int count = 0, argCnt;
	while((argCnt = (fscanf(fp,"%d %d",&testr,&testc))) != EOF && count < 1)	{
		count++;	
		iprint(testr);
		iprint(testc);
		//iprint(*row);
		//iprint(*col);
	} 
	
	return argCnt;
}

int gettingDimensions(char **dimensions, char letter,int charCount, int *rowLngth, int *colLngth)	{
		switch (charCount)	{
			case 0:
				dimensions[0][0] = letter;	
				break;
			case 1:
				dimensions[0][1] = letter;
				dimensions[0][2] = '\0';
				*rowLngth = atoi(dimensions[0]);
				break;
			case 2:
				dimensions[1][0] = letter;
				break;
			case 3:
				dimensions[1][1] = letter;
				dimensions[1][2] = '\0';
				*colLngth = atoi(dimensions[1]);
				break;
			default:
				break;
		}

	return 1;
}
