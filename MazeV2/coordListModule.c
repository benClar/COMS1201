//! CoordListModule.c
/*! 
 *	Path Node Data Structure.  Holds and manages list of grid coorindates that make up exit route
 */

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/mazeDataFunctions.h"
#include ".headers/coordListModule.h"

/*---------- Data Structures ----------*/

/*	\struct
 *  Node of list holding pair of coorindates.  Links to next Node.
 * 		Allows path data to be stored flexibly
 */
struct pathNode {

    int row;	/**< Row coordinate of desired path */
    int col;	/**< Column coordinate of desired path */
    PathNode next;	/**< Pointer to next set of coordinates of path */
};

/* /struct
 * Stores addresses of starting node and current node.  Stores number of items in list.
 */
struct pathList {

    PathNode start; /**< pointer to start of list structure.  Not allowed to be changed. */
    PathNode current; /**< pointer to working node of list structure.  Can be changed. */
    int nItems; 	/**< Count of number of items in list. Cannot be changed without node removal. */

};

/*---------- Functions ----------*/

/*
 *Creates space to stored start of list.
 */
PathList createList()   {

    PathList NewList = (PathList) checkMalloc(malloc(sizeof(*NewList)));
    NewList->start = NewList->current = NULL;
    return NewList;
}

/*
 *Adds and initializes list members.
 */
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

/*
 * Accessor for start of list
 */
PathNode getStart(PathList list)	{

	return list->start;

}

/*
 * Accessor for current node in list
 */
PathNode getCurr(PathList list)	{

	return list->current;
}

/*
 * Accessor for current node row coordinate
 */
int getCurrRow(PathList list)	{
	
	return list->current->row;

}
/*
 * Accessor for current node column coordinate
 */

int getCurrCol(PathList list)	{

	return list->current->col;

}

/*
 *Setter to change working node
 */
void setCurr(PathList list, PathNode node)	{

	list->current = node;

}

/*
 * Move to next node in list !!Can this be guarded!!
 */ 
void nextNode(PathList list)	{
		list->current = list->current->next;
}

/*
 *Removes DEADENDS from list
 */
void cleanList(MazeMap maze, PathList list) {

    list->current = list->start;
    while(list->current->next != NULL)  {
        if(getBlockType(maze, list->start->row, list->start->col) == DEADEND)   {
            PathNode startTemp = list->start;
            list->current = list->start = list->start->next;
            free(startTemp);
			list->nItems--;
        } if(getBlockType(maze,list->current->next->row, list->current->next->col) == DEADEND)  {
                PathNode nextTemp = list->current->next->next;
                free(list->current->next);
                list->current->next = nextTemp;
				list->nItems--;
        } else {
            list->current = list->current->next;
        }
    }
}
