//
// chamberStack.c
// Created by bc14627 on 2014/15/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/chamberStack.h"

/*---------- Symbolic Constants  -----------*/


struct chCoords	{

	int row;
	int col;

};
struct chamber {

    ChCoords topLeft;
    ChCoords topRight;
    ChCoords botLeft;
    ChCoords botRight;
	Chamber prev;

};

struct chStack {

    Chamber stackTop;
    int nChamber;

};

/*---------- Functions ----------*/

ChStack createChamberStack()	{

	ChStack newStack = (ChStack) malloc(sizeof(*newStack));
	newStack->stackTop = NULL;
	newStack->nChamber = 0;
	return newStack;
}

void addChamber(ChStack chamberStack)	{
		Chamber nextChamber = (Chamber) malloc(sizeof(*nextChamber));
		nextChamber->topLeft = (ChCoords) malloc(sizeof(*(nextChamber->topLeft)));
		nextChamber->topRight = (ChCoords) malloc(sizeof(*(nextChamber->topRight)));
		nextChamber->botLeft = (ChCoords) malloc(sizeof(*(nextChamber->botLeft)));
		nextChamber->botRight = (ChCoords) malloc(sizeof(*(nextChamber->botRight)));

		nextChamber->prev = chamberStack->stackTop;
		chamberStack->stackTop = nextChamber;	
		chamberStack->nChamber++;
}

void popChamber(ChStack chamberStack, 
	int *topLeftX, int *topLeftY, 
	int *topRightX, int *topRightY, 
	int *botLeftX, int *botLeftY, 
	int *botRightX,	int *botRightY)	{

	*topLeftX = chamberStack->stackTop->topLeft->col;
	*topLeftY = chamberStack->stackTop->topLeft->row;

	*topRightX = chamberStack->stackTop->topRight->col;
	*topRightY = chamberStack->stackTop->topRight->row;

	*botLeftX = chamberStack->stackTop->botLeft->col;
	*botLeftY = chamberStack->stackTop->botLeft->row;

	*botRightX = chamberStack->stackTop->botRight->col;
	*botRightY = chamberStack->stackTop->botRight->row;

	Chamber temp = chamberStack->stackTop;
	chamberStack->stackTop = chamberStack->stackTop->prev;
	free(temp);
}

void setTopLeft(ChStack chamberStack, int row, int col)	{

	chamberStack->stackTop->topLeft->row =  row;
	chamberStack->stackTop->topLeft->col = col;
}

void setBotLeft(ChStack chamberStack, int row, int col) {

	chamberStack->stackTop->botLeft->row = row;
	chamberStack->stackTop->botLeft->col = col;

}

void setBotRight(ChStack chamberStack, int row, int col){

	chamberStack->stackTop->botRight->row = row;
	chamberStack->stackTop->botRight->col = col;

}

void setTopRight(ChStack chamberStack, int row, int col){

	chamberStack->stackTop->topRight->row =  row;
	chamberStack->stackTop->topRight->col = col;

}


