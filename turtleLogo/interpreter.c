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
	double val;
};

void createParseArr()	{

	ParseArr nPArr = (ParseArr) checkMalloc(malloc(sizeof(*nPArr)));
	nPArr->nArr = NULL;
	nPArr->items = 0;
	getParseArr(nPArr);
}

int addParseNode(char *cmd, int value)	{
	assert(cmd != NULL);
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

int getParseNodeNumber()	{
	return getParseArr(NULL)->items;
}

ParseArr getParseArr(ParseArr cp)	{

	static ParseArr currPArr;

	if(cp != NULL)	{
		currPArr = cp;
	}
	return currPArr;
}

/*void readParseArray()	{
	ParseArr pArr = getParseArr(NULL);
	int i;
	for(i = 0; i < pArr->items; i++)	{
		moveInterpret(pCommand(pArr->nArr[i]),pVal(pArr->nArr[i]));		
	}
}*/

char* pCommand(ParseNode n)	{
	if(n != NULL)	{
		return n->command;
	}
	return NULL;
}

int pVal(ParseNode n)	{
	if(n != NULL)	{
		return n->val;
	}

	ERROR("Attempt to access non existent symbol");
}

ParseNode getSpecParseNode(int n)	{
	ParseArr pArr = getParseArr(NULL);
	if(n<=pArr->items)	{
		return pArr->nArr[n-1];
	}

	return NULL;
}

void clearParseArr()	{

	ParseArr pArr = getParseArr(NULL);
	int i;
	if(pArr != NULL)	{
		for(i = 0; i < pArr->items; i++)	{
			free(pArr->nArr[i]);
		}
		pArr->items = 0;
	}
	pArr->nArr = NULL;
}

void freeParseArr()	{
	clearParseArr();
	ParseArr pArr = getParseArr(NULL);

	if(pArr != NULL)	{
		free(pArr);
	}
}

/*void moveInterpret(char *command, double value)	{
	if(!strcmp(command,FORWARD))	{
		moveTurtleForward(value);
		if(!TESTING)	{
			drawLine(getOriginX(),getOriginY(),getTargetX(),getTargetY());
		}
	} else if(!strcmp(command,R_TURN))	{
		turnTurtleRight(value);
	} else if(!strcmp(command,L_TURN))	{
		turnTurtleLeft(value);
	}
}*/

/*---------- Testing Functions ----------*/

void interpreterUnitTests()	{
	parseArrayTests();
}

void interpetingTest()	{
	addParseNode(FORWARD,10);
}

void parseArrayTests()	{

	enterSuite("Managing Parse Array");
	testVal(addParseNode(FORWARD,10),1,"Valid: One parse node has been added",EQUALS);
	testVal(addParseNode(FORWARD,20),2,"Valid: Two parse nodes have been added",EQUALS);
	testVal(pVal(getSpecParseNode(1)),10,"Valid: Parse node one value is 10",EQUALS);
	testVal(pVal(getSpecParseNode(2)),20,"Valid: Parse node two value is 20",EQUALS);
	clearParseArr();
	testVal(addParseNode(FORWARD,20),1,"Valid: One parse node added to cleared parse array",EQUALS);
	clearParseArr();
	testVal(checkForNull(pCommand(getSpecParseNode(1))),1,"Invalid: No parse nodes in array",EQUALS);	
	leaveSuite();
}
