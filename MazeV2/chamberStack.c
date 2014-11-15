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

struct chamber {

    int top;
    int left;
    int bottom;
    int right;
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
		nextChamber->prev = chamberStack->stackTop;
		chamberStack->stackTop = nextChamber;	
		chamberStack->nChamber++;
}

void popChamber(ChStack chamberStack, int *top, int *bottom, int *left, int *right)	{

	*top = chamberStack->stackTop->top;
	*bottom = chamberStack->stackTop->left;
	*left = chamberStack->stackTop->bottom;
	*right = chamberStack->stackTop->right;
	Chamber temp = chamberStack->stackTop;
	chamberStack->stackTop = chamberStack->stackTop->prev;
	free(temp);
}

void setTop(ChStack chamberStack, int top)	{

	chamberStack->stackTop->top =  top;
}

void setLeft(ChStack chamberStack, int left)	{

	chamberStack->stackTop->left = left;

}

void setBottom(ChStack chamberStack, int bottom)	{

	chamberStack->stackTop->bottom = bottom;

}

void setRight(ChStack chamberStack, int right)	{

	chamberStack->stackTop->right =  right;

}


