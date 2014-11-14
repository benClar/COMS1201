/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/neillsdl2.h"
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

struct pathNode	{

	int row;
	int col;
	PathNode next;
};

struct pathList	{

	PathNode start;
	PathNode current;
	int nItems;

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
//	PathList newList = createList();
//	addNode(newList,10,10);
//	iprint(newList->current->row);
//	addNode(newList,30,30);
//	iprint(newList->current->row);
//	return 0;
//}

PathList createList()	{

	PathList NewList = (PathList) checkMalloc(malloc(sizeof(*NewList)));
	NewList->start = NewList->current = NULL;
	return NewList;
}

void addNode(PathList list, int addRow, int addCol)	{
	
	if(list->start == NULL)	{
		list->start = list->current = (PathNode) malloc(sizeof(*(list->start)));
	} else {
		list->current->next = (PathNode) checkMalloc(malloc(sizeof(*(list->current->next))));
		list->current = list->current->next;
	}

	list->current->row = addRow;
	list->current->col = addCol;
	list->current->next = NULL;
	list->nItems++;
}

void cleanList(MazeMap maze, PathList list)	{

	list->current = list->start;
//	iprint(list->current->row);
//	iprint(list->current->col);
	while(list->current->next != NULL)	{
		if(getBlockType(maze, list->start->row, list->start->col) == DEADEND)	{
			PathNode startTemp = list->start;
			list->current = list->start = list->start->next;
			free(startTemp);
		} if(getBlockType(maze,list->current->next->row, list->current->next->col) == DEADEND)	{
				PathNode nextTemp = list->current->next->next;
				free(list->current->next);
				list->current->next = nextTemp;
				
		} else {
			list->current = list->current->next; 	
		}
	}
}

int findEntrance(MazeMap maze,int *eRow, int *eCol)	{

	int side,top;
	BOOL entranceFound;
	for(side = 0,entranceFound=FALSE; side < maze->height && entranceFound == FALSE; side++)	{
		if((getBlock(maze,side,LEFTSIDE)) == ' ') {
			entranceFound = TRUE;
		}
	}

	for(top = 0; top < maze->width; top++)	{
		if((getBlock(maze,TOPSIDE,side)) == ' ')	{
		//determines if opening on top is closer than opening on side to left hand corner
			if(--side >= top)	{ 
				*eRow = TOPSIDE;
				*eCol = top;
				return(setBlockType(maze,TOPSIDE,top,ENTRANCE));
			}
		}
	}
		*eRow = --side;
		*eCol = LEFTSIDE;
		return(setBlockType(maze,side,LEFTSIDE,ENTRANCE));
}

int detectExit(MazeMap maze, int row, int col)	{
	if((row == maze->width -1 || col == maze->height - 1 || row == 0 || col == 0)  && (getBlock(maze,row,col) == ' ') && (getBlockType(maze,row,col) != ENTRANCE) )	{
		return 1;
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
		return newMaze;
	} else {
		fprintf(stderr, "No dimensions given for map size in first line of input file\n");	
		exit(1);
	}
}

int **createIntArray(int height, int width)	{

	int	i;
	int **array = (int**) checkMalloc(malloc(height * sizeof(int*)));
	for(i = 0; i < height; i++)	{
		array[i] = (int *) checkMalloc(malloc((width * sizeof(int))));
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

int addToGrid(MazeMap maze, int *row, int *col, char value, char wallCharacter)	{
	if (value != '\n')	{
		if(*col == maze->width )	{
			*row = *row +1;
			*col = 0;
		}
		maze->mazeGrid[(*row)][(*col)].block = value;
		
		if(maze->mazeGrid[(*row)][(*col)].block == wallCharacter)	{
			setBlockType(maze, *row, *col, WALL);
		} else {
			setBlockType(maze, *row, *col, MISC);	
		}
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

int printMap(MazeMap maze,PathList list)	{
	printf("printing...\n");
	int	r,c,count;
	list->current = list->start;
		while(list->current != NULL){
			for(r = 0,count = 0; r < getHeight(maze); r++)    {
				for(c = 0; c < getWidth(maze); c++)     {
					if((list->current != NULL ) && (r == list->current->row) && (c == list->current->col) && (!count))	{
						printf(" .");
						list->current = list->current->next;		
						count++;
					} else{  
						printf(" %c",getBlock(maze,r,c));
					}
				}
				pNL();
			}
		}
	return 1;
}

int printFullRoute(MazeMap maze)	{
	int r,c;
	for(r = 0; r < getHeight(maze); r++)	{
		for(c = 0; c < getWidth(maze); c++)	{
			if(getBlockType(maze,r,c)==EXITROUTE)	{
				printf(" .");
			} else{
				printf(" %c",getBlock(maze,r,c));
			}
		}
		pNL();
	}
	return 1;	
}

int graphicalPrintRightRoute(MazeMap maze,PathList list,SDL_Simplewin sw)	{

	int r,c,count;
	SDL_Rect rectangle;
        rectangle.w = RECTSIZE;
        rectangle.h = RECTSIZE;
	list->current = list->start;

	while(list->current != NULL){
		for(r=0, count = 0; r < getHeight(maze); r++)	{
			for(c = 0; c < getWidth(maze); c++)	{
				if((list->current != NULL ) && (r == list->current->row) && (c == list->current->col) && (!count))      {
					Neill_SDL_SetDrawColour(&sw,255,165,0);
                               		rectangle.x = (c*RECTSIZE);
                                	rectangle.y = (r*RECTSIZE);
                                	SDL_RenderFillRect(sw.renderer, &rectangle);
					list->current = list->current->next;
                                	count++;	
				} else if(getBlockType(maze,r,c)==WALL) {
                                	Neill_SDL_SetDrawColour(&sw,128,0,0);
                                	rectangle.x = (c*RECTSIZE);
                                	rectangle.y = (r*RECTSIZE);
                                	SDL_RenderFillRect(sw.renderer, &rectangle);
				} else {
                                	Neill_SDL_SetDrawColour(&sw,0,0,0);
                                	rectangle.x = (c*RECTSIZE);
					rectangle.y = (r*RECTSIZE);
					SDL_RenderFillRect(sw.renderer, &rectangle);
				}
			}
		}
        SDL_RenderPresent(sw.renderer);
        SDL_UpdateWindowSurface(sw.win);
	}
	return 1;
}

int graphicalPrintFullRoute(MazeMap maze,SDL_Simplewin sw)	{
	int	r,c;

	SDL_Rect rectangle;
	rectangle.w = RECTSIZE;
	rectangle.h = RECTSIZE;

	for(r = 0; r < getHeight(maze); r++)	{
		for(c = 0; c < getWidth(maze); c++)	{
			if(getBlockType(maze,r,c)==EXITROUTE)	{
				Neill_SDL_SetDrawColour(&sw,255,165,0);
				rectangle.x = (c*RECTSIZE);
				rectangle.y = (r*RECTSIZE);
				SDL_RenderFillRect(sw.renderer, &rectangle);
			} else if(getBlockType(maze,r,c)==WALL) {
		                Neill_SDL_SetDrawColour(&sw,128,0,0);
       		                rectangle.x = (c*RECTSIZE);
				rectangle.y = (r*RECTSIZE);
				SDL_RenderFillRect(sw.renderer, &rectangle);
			} else if (getBlockType(maze,r,c)==ENTRANCE) {
       		                Neill_SDL_SetDrawColour(&sw,0,128,0);
				rectangle.x = (c*RECTSIZE);
				rectangle.y = (r*RECTSIZE);	
				SDL_RenderFillRect(sw.renderer, &rectangle);
			} else {
       		                Neill_SDL_SetDrawColour(&sw,0,0,0);
				rectangle.x = (c*RECTSIZE);
				rectangle.y = (r*RECTSIZE);	
				SDL_RenderFillRect(sw.renderer, &rectangle);
			}
		}
	}
	//Update Window
	SDL_RenderPresent(sw.renderer);
	SDL_UpdateWindowSurface(sw.win);

	//Check if User has quit
	//Neill_SDL_Events(&sw);
	return 1;
}

int setBlockType(MazeMap maze, int row, int col, blockType newBT)	{
	maze->mazeGrid[row][col].bT = newBT;
	
	return 1;

}

int getBlockType(MazeMap maze, int row, int col)	{

	return maze->mazeGrid[row][col].bT;

}

int mazeBoundaryCheck(MazeMap maze, int row, int col) {

	if((row < maze->height && row >= 0) && (col < maze->width && col >= 0))	{
		return 1;
	}
		return 0;
}
