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
	blockType bT;
};

struct mazeMap	{

	struct mazeBlock **mazeGrid;
	int height;
	int width;
};

/*---------- Functions ----------*/

//int main()	{
//	int	r = 0,  c = 0, i,b;
//	MazeMap test = createMap(10,10);
//	for(i = 0; i < 100; i++)	{
//			addToGrid(test,&r,&c,'c');
//	}
	//for( i = 0; i < 10; i++)	{
//		for( b = 0; b < 10; b++)	{
//			cprint(test->mazeGrid[i][b].block);
//		}
//	pNL();
//	}
//	printMap(test);	
//	iprint(setEntrance(test));
	//return 0;
//}

int setExits(MazeMap maze, int rowStart, int side, int increment)	{
	for(; rowStart < maze->height; rowStart+=increment)	{
		if((getBlock(maze,rowStart,side)) == ' ')	{
			if(side == LEFTSIDE)	{
				setBlockType(maze,rowStart,side,ENTRANCE);
				return rowStart;
			} else if(side == (maze->width-1))	{
				setBlockType(maze,rowStart,side,EXIT);
				return rowStart;
			}	
		}
	}
	return 0;	
}

MazeMap createMap(int height, int width)	{

	int i;
	if((height > 0) && (width > 0))	{
		MazeMap newMaze = (MazeMap) checkMalloc(malloc(sizeof(*newMaze)));
		newMaze->mazeGrid = (struct mazeBlock**) checkMalloc(malloc(height * sizeof(struct mazeBlock*)));
		for(i = 0; i < width; i++)	{
			newMaze->mazeGrid[i] = (struct mazeBlock*) checkMalloc(malloc(width * sizeof(struct mazeBlock)));	
		}
		
		newMaze->height = height;
		newMaze->width = width;
		newMaze->mazeGrid[0][0].block = 'd';
		return newMaze;
	} else {
		fprintf(stderr, "No dimensions given for map size in first line of input file\n");	
		exit(1);
	}
}

char **createStringArray(int height, int width)	{

	int	i;
	char **array = (char**) checkMalloc(malloc(height * sizeof(char*)));
	for(i = 0; i < height; i++)	{
		array[i] = (char *) checkMalloc(malloc((width * sizeof(char))));
	}
	return array;
}

void *checkMalloc(void *malP)	{

	if (malP == NULL)	{
		fprintf(stderr, "null address generated\n");
		exit(1);
	}
	return malP;		
}

int addToGrid(MazeMap maze, int *row, int *col, char value)	{
	
	if (value != '\n')	{
		if(*col == maze->width )	{
			*row = *row +1;
			*col = 0;
		}
		maze->mazeGrid[(*row)][(*col)].block = value;
		*col = *col + 1;	
	}
	return value;
}

char getBlock(MazeMap maze, int row, int col)	{

	return maze->mazeGrid[row][col].block;
}

int getHeight(MazeMap maze)	{

	return maze->height;
}

int getWidth(MazeMap maze)	{

	return maze->width;
}

int printMap(MazeMap maze)	{

	int	r,c;
	for(r = 0; r < getHeight(maze); r++)	{
		for(c = 0; c < getWidth(maze); c++)	{
			printf("%c",getBlock(maze,r,c));
		}
		pNL();
	}
	return 1;
}

int setBlockType(MazeMap maze, int row, int col, blockType newBT)	{
	maze->mazeGrid[row][col].bT = newBT;
	return 1;

}





