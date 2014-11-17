/*
 * Creates, accesses and modifies maze grid
 */

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"

/*---------- Data Structures ----------*/

/* \struct
 * Building block for maze
 */
struct mazeBlock {

	char block; /**< holds character that represents block*/
	blockType bT; /**< type of block */
};

/* \struct
 * Holds maze blocks and store dimension details
 */
struct mazeMap	{

	struct mazeBlock **mazeGrid; /**< array of pointers to blocks */
	int height; /**<height of maze.  Can only be set once during creation. */
	int width; /**< width of maze.  Can only be set once during creation. */
};

/*---------- Functions ----------*/

/*
 *creates structure for required size to hold maze blocks
 */
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
		return newMaze;
	} else {
		fprintf(stderr, "No dimensions given for map size in first line of input file\n");	
		exit(1);
	}
}

void freeMazeBlocks(MazeMap maze)	{

	int r;
	for(r =0; r < getHeight(maze); r++)	{
			free(maze->mazeGrid[r]);
	}
}

/*
 * Validates mallocs
 */
void *checkMalloc(void *malP)	{

	if (malP == NULL)	{
		fprintf(stderr, "null address generated\n");
		exit(1);
	}
	return malP;		
}

/*
 *Accepts characters to be added to maze
 *		Labels specified characters as walls
 */
int addToGrid(MazeMap maze, int row, int col, char value, char wallCharacter)	{
	if (value == '\n')	{
			return 0;
	}
	maze->mazeGrid[(row)][(col)].block = value;
		
	if(maze->mazeGrid[(row)][(col)].block == wallCharacter)	{
			setBlockType(maze, row, col, WALL);
		} else {
			setBlockType(maze, row, col, MISC);	
		}
	return value;
}

/*
 * Accessor for specified block character
 */
char getBlock(MazeMap maze, int row, int col)	{

	return maze->mazeGrid[row][col].block;
}
/*
 * Accessor for maze height
 */
int getHeight(MazeMap maze)	{

	return maze->height;
}

/*
 * Accessor for maze width
 */
int getWidth(MazeMap maze)	{

	return maze->width;
}

/*
 * Setter for type of specified block
 */
int setBlockType(MazeMap maze, int row, int col, blockType newBT)	{
	maze->mazeGrid[row][col].bT = newBT;
	
	return 1;

}

/*
 * Accessor for type of specified block
 */
int getBlockType(MazeMap maze, int row, int col)	{

	return maze->mazeGrid[row][col].bT;

}

/*
 * Checks given coordinate against maze height and width
 */
int mazeBoundaryCheck(MazeMap maze, int row, int col) {

	if((row < maze->height && row >= 0) && (col < maze->width && col >= 0))	{
		return 1;
	}
		return 0;
}
