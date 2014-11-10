/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"

/*---------- Data Structures ----------*/

struct mazeBlock {

	char block;

};

struct mazeMap	{

	struct mazeBlock **mazeGrid;
	int height;
	int width;
};

/*---------- Functions ----------*/

//int main()	{
//	int	i,c;
//	MazeMap test = createMap(10,10);
//	for(i = 0; i < 10; i++)	{
//		for (c = 0; c < 10; c++)	{
//			addToGrid(test,i,c,'c');
//			cprint(test->mazeGrid[i][c].block);
//		}
//		pNL();
//	}
//	return 0;
//}

MazeMap createMap(int height, int width)	{

	int i;
	if((height > 0) && (width > 0))	{
		MazeMap newMaze = (MazeMap) checkMalloc(malloc(sizeof(*newMaze)));
		newMaze->mazeGrid = (MazeBlock*) checkMalloc(malloc(height * sizeof(MazeBlock*)));
		for(i = 0; i < width; i++)	{
			newMaze->mazeGrid[i] = (MazeBlock) checkMalloc(malloc(width * sizeof(MazeBlock)));	
		}
		
		newMaze->height = height;
		newMaze->width = width;
	
		return newMaze;
	} else {
		fprintf(stderr, "No dimensions given for map size in first line of input file\n");	
		exit(1);
	}
}

void *checkMalloc(void *malP)	{

	if (malP == NULL)	{
		fprintf(stderr, "null address generated\n");
		exit(1);
	}
	return malP;		
}

int addToGrid(MazeMap maze, int row, int col, char value)	{
	
	maze->mazeGrid[row][col].block = value;
	
	return value;
}
