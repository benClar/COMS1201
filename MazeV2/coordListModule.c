//
// CoordListModule.c
// Created by bc14627 on 2014/14/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"

/*---------- Data Structures ----------*/

struct pathNode {

    int row;
    int col;
    PathNode next;
};

struct pathList {

    PathNode start;
    PathNode current;
    int nItems;

};
/*---------- Functions ----------*/

PathList createList()   {

    PathList NewList = (PathList) checkMalloc(malloc(sizeof(*NewList)));
    NewList->start = NewList->current = NULL;
    return NewList;
}

void addNode(PathList list, int addRow, int addCol) {

    if(list->start == NULL) {
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

PathNode getStart(PathList list)	{

	return list->start;

}

PathNode getCurr(PathList list)	{

	return list->current;
}

int getCurrRow(PathList list)	{
	
	return list->current->row;

}

int getCurrCol(PathList list)	{

	return list->current->col;

}

void setStart(PathList list, PathNode node)	{
	
	list->start = node;
}

void setCurr(PathList list, PathNode node)	{

	list->current = node;

}

void nextNode(PathList list)	{

	list->current = list->current->next;

}

void cleanList(MazeMap maze, PathList list) {

    list->current = list->start;
    while(list->current->next != NULL)  {
        if(getBlockType(maze, list->start->row, list->start->col) == DEADEND)   {
            PathNode startTemp = list->start;
            list->current = list->start = list->start->next;
            free(startTemp);
        } if(getBlockType(maze,list->current->next->row, list->current->next->col) == DEADEND)  {
                PathNode nextTemp = list->current->next->next;
                free(list->current->next);
                list->current->next = nextTemp;

        } else {
            list->current = list->current->next;
        }
    }
}
