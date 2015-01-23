/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "interpreter.h"

/*---------- Functions ----------*/
struct parseArr	{

	ParseNode *nArr;
	int items;
};

struct parseNode	{

	char *command;
	int val;
};

void createParseArr()	{

	ParseArr nPArr = (ParseArr) checkMalloc(malloc(sizeof(*nPArr)));
	nPArr->nArr = NULL;
	nPArr->items = 0;
	getParseArr(nPArr);
}

int addParseNode(char *cmd, int value)	{
	ParseArr pArr = getParseArr(NULL);
	if(pArr->nArr == NULL)	{
		pArr->nArr = (ParseNode*) checkMalloc(malloc(sizeof(*pArr->nArr)));
		pArr->items++;
	} else	{
		pArr->nArr = (ParseNode*) checkMalloc(realloc(pArr->nArr,++pArr->items*sizeof(*pArr->nArr[(pArr->items - 1)])));
	}
	pArr->nArr[(pArr->items - 1)] = (ParseNode) checkMalloc(malloc(sizeof(*pArr->nArr[(pArr->items - 1)])));
	pArr->nArr[(pArr->items - 1)]->val = value;
	pArr->nArr[(pArr->items - 1)]->command = (char*) checkMalloc(calloc(getTokenLength(cmd),sizeof(char)));
	strcat(pArr->nArr[(pArr->items - 1)]->command,cmd);
	return pArr->items;
}

ParseArr getParseArr(ParseArr cp)	{

	static ParseArr currPArr;

	if(cp != NULL)	{
		currPArr = cp;
	}
	return currPArr;
}

void readParseArray()	{
	ParseArr pArr = getParseArr(NULL);
	int i;
	for(i = 0; i < pArr->items; i++)	{
		moveInterpret(pCommand(pArr->nArr[i]),pVal(pArr->nArr[i]));		
	}
}

char* pCommand(ParseNode n)	{
	return n->command;
}

int pVal(ParseNode n)	{
	return n->val;
}

void freeParseArr()	{

	ParseArr pArr = getParseArr(NULL);
	int i;
	for(i = 0; i < pArr->items; i++)	{
		free(pArr->nArr[i]);
		pArr->nArr[i] = NULL;
	}

	free(pArr);
}

void moveInterpret(char *command, int value)	{
	if(!strcmp(command,FORWARD))	{
		moveTurtleForward(value);
		drawLine(getOriginX(),getOriginY(),getTargetX(),getTargetY());
	} else if(!strcmp(command,R_TURN))	{
		turnTurtleRight(value);
	} else if(!strcmp(command,L_TURN))	{
		turnTurtleLeft(value);
	}
}

/*---------- Testing Functions ----------*/

void interpreterUnitTests()	{

		enterSuite("Managing Parse Array");
		testVal(addParseNode(FORWARD,10),1,"Valid: One parse node has been added");
		testVal(addParseNode(FORWARD,20),2,"Valid: Two parse nodes have been added");
		leaveSuite();
}
